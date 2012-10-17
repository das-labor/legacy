/**
 * \defgroup winsimulator Simulation of the Borg API for the Win32 platform.
 */
/*@{*/

/**
 * This is a native Win32 port of the Borgware-2D API simulator. Although the
 * OpenGL based simulator is in fact platform independent, there are some
 * obstacles regarding Cygwin's OpenGL support.
 *
 * Earlier versions of Cygwin used to ship bindings to Win32's native OpenGL
 * libraries. Unfortunately some of those native components (GLUT in particular)
 * weren't maintained for years so it was decided to cease support for them.
 *
 * The reasons are explained in more detail at
 * http://cygwin.com/ml/cygwin/2012-05/msg00276.html
 *
 * The OpenGL bindings which are now shipped with Cygwin require a running
 * X-Server which I consider clumsy to use on a Windows platform (especially for
 * a small application like this simulator). So I decided to write a native
 * Win32 application to free Windows developers from the hassles of rolling out
 * a complete X11 setup.
 *
 * The native simulator feels like the OpenGL based one, with the exception that
 * you can't rotate the matrix (I'm using the plain GDI32 API for the graphics).
 *
 * @file winmain.c
 * @brief Simulator for the Win32 platform.
 * @author Christian Kroll
 */

#include <windows.h>
#include <setjmp.h>
#include "../config.h"
#include "../display_loop.h"

/** Number of bytes per row. */
#define LINEBYTES ((NUM_COLS + 1) / 8)

/** The width (in pixels) of the margin around a LED. */
#define LED_MARGIN 1
/** The diameter (in pixels) of a LED. */
#define LED_DIAMETER 14
/** The extend of the whole LED including its margin. */
#define LED_EXTENT (2 * LED_MARGIN + LED_DIAMETER)

/** Width of the canvas. */
#define WND_X_EXTENTS (NUM_COLS * LED_EXTENT)
/** Height of the canvas. */
#define WND_Y_EXTENTS (NUM_ROWS * LED_EXTENT)


/* string constants */
LPCSTR g_strWindowClass = "BorgSimulatorWindowClass";
LPCSTR g_strWindowTitle = "Borg Simulator";

LPCSTR g_strError = "Error";
LPCSTR g_strErrorRegisterWindow = "Error: Could not register window class.";
LPCSTR g_strErrorCreateWindow = "Error: Could not create window.";


/** Event object for the multimedia timer (wait() function). */
HANDLE g_hWaitEvent;


/** Fake port for simulating joystick input. */
volatile unsigned char fakeport;
/** Flag which indicates if wait should jump to the menu if fire is pressed. */
volatile unsigned char waitForFire;
/** The simulated frame buffer of the borg. */
volatile unsigned char pixmap[NUMPLANE][NUM_ROWS][LINEBYTES];
/** Jump buffer which leads directly the menu. */
extern jmp_buf newmode_jmpbuf;

/* forward declarations */
LRESULT CALLBACK simWndProc(HWND hWnd,
                            UINT msg,
                            WPARAM wParam,
                            LPARAM lParam);


/**
 * Registers a window class (necessary for creating a window).
 * @param lpwc Pointer to WNDCLASS struct.
 * @param hInstance Handle of the instance where this window class belongs to.
 * @return TRUE if successful, otherwise FALSE.
 */
BOOL simRegisterWindowClass(WNDCLASSA *const lpwc,
                            HINSTANCE hInstance)
{
	lpwc->style = 0;
	lpwc->lpfnWndProc = simWndProc;
	lpwc->cbClsExtra = 0;
	lpwc->cbWndExtra = 0;
	lpwc->hInstance = hInstance;
	lpwc->hIcon = LoadIcon(NULL, IDI_WINLOGO);
	lpwc->hCursor = LoadCursor(NULL, IDC_ARROW);
	lpwc->hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	lpwc->lpszMenuName = NULL;
	lpwc->lpszClassName = g_strWindowClass;

	return (RegisterClassA(lpwc) != 0);
}


/**
 * Creates a new window and makes it visible.
 * @param lphWnd Pointer to window handle.
 * @param hInstance Handle of the instance where this window belongs to.
 * @param nCmdShow Flag for showing the window minimized, maximized etc.
 * @return TRUE if successful, otherwise FALSE.
 */
BOOL simCreateWindow(HWND *lphWnd,
                     HINSTANCE hInstance,
                     int nCmdShow)
{
	/* create window and retrieve its handle */
	*lphWnd = CreateWindow(g_strWindowClass, g_strWindowTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WND_X_EXTENTS * 2, WND_Y_EXTENTS * 2, HWND_DESKTOP,
		NULL, hInstance, NULL);

	/* mske it visible */
	if (*lphWnd != NULL)
	{
		ShowWindow(*lphWnd, nCmdShow);
		UpdateWindow(*lphWnd);
		return TRUE;
	}

	return FALSE;
}


/**
 * Draws the LED matrix on the given device context.
 * @param hdc The device context where the LED matrix should be drawn on.
 */
void simDrawMatrix(HDC hdc)
{
	COLORREF colorLed;
	HBRUSH hBrushLed;
	HGDIOBJ hGdiOld;
	unsigned int c, p, x, y, absX;
	int left, right, top, bottom;
	static unsigned char const shl_map[8] =
		{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	/* clear background */
	FloodFill(hdc, 0, 0, RGB(0, 0, 0));

	/* go through every plane */
	for (p = 0; p < NUMPLANE; ++p)
	{
		/* create and select red brush into device context */
		colorLed = RGB((255.0 / NUMPLANE) * (p + 1), 0, 0);
		hBrushLed = CreateSolidBrush(colorLed);
		hGdiOld = SelectObject(hdc, hBrushLed);

		/* translate pixmap into LEDs */
		for (y = 0; y < NUM_ROWS; ++y)
		{
			for (c = 0; c < LINEBYTES; ++c)
			{
				for (x = 0; x < 8; ++x)
				{
					if (pixmap[p][y][c] & shl_map[x])
					{
						// eventually draw a LED, mirroring its coordinates
						absX = (c * 8 + x) * LED_EXTENT + LED_MARGIN;
						left = WND_X_EXTENTS - absX;
						right = WND_X_EXTENTS - absX - LED_DIAMETER + 1;
						top = y * LED_EXTENT + LED_MARGIN;
						bottom = top + LED_DIAMETER - 1;
						Ellipse(hdc, left, top, right, bottom);
					}
				}
			}
		}

		/* dispose that brush */
		DeleteObject(SelectObject(hdc, hGdiOld));
	}
}


/**
 * Retrieves device context from given window, creates a compatible memory
 * device context for double buffering and hands that thing over to
 * simDrawMatrix().
 * @param hWnd The window where the LED-Matrix should be displayed.
 */
void simDisplay(HWND hWnd)
{
	RECT rect;
	HDC hdc;
	HDC hMemDc;
	HBITMAP hBmp;
	HBITMAP hOldBmp;

	/* retrieve window dimensions */
	if (GetClientRect(hWnd, &rect))
	{
		int const cx = rect.right - rect.left;
		int const cy = rect.bottom - rect.top;

		/* retrieve device context */
		if ((hdc = GetDC(hWnd)) != NULL)
		{
			/* make window contents scalable */
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, WND_X_EXTENTS, WND_Y_EXTENTS, NULL);
			SetViewportExtEx(hdc, cx, cy, NULL);

			/* create memory device context for double buffering */
			hMemDc = CreateCompatibleDC(hdc);
			if (hMemDc != NULL)
			{
				/* contents of the memory DC should be scaled as well */
				SetMapMode(hMemDc, MM_ANISOTROPIC);
				SetWindowExtEx(hMemDc, WND_X_EXTENTS, WND_Y_EXTENTS, NULL);
				SetViewportExtEx(hMemDc, cx, cy, NULL);

				/* create a bitmap to be associated with the memory DC... */
				hBmp = CreateCompatibleBitmap(hdc, cx, cy);
				if (hBmp != NULL)
				{
					/* ...and selct that into that DC */
					hOldBmp = (HBITMAP)SelectObject(hMemDc, hBmp);

					/* finally *sigh* draw the LED matrix */
					simDrawMatrix(hMemDc);

					/* and blit that into the window DC */
					BitBlt(hdc, 0, 0, cx, cy, hMemDc, 0, 0, SRCCOPY);

					/* clean up */
					DeleteObject(SelectObject(hMemDc, hOldBmp));
				}
				DeleteDC(hMemDc);
			}
			ReleaseDC(hWnd, hdc);
		}
		InvalidateRect(hWnd, &rect, FALSE);
	}
}

/**
 * Message handler for the main window.
 * @param hWnd The window whose messages should be processed.
 * @param msg The message fired from the operating system.
 * @param wParam First message parameter.
 * @param lParam Second message parameter.
 */
LRESULT CALLBACK simWndProc(HWND hWnd,
                            UINT msg,
                            WPARAM wParam,
                            LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	LPMINMAXINFO lpminmax;

	switch (msg)
	{
	/* enforce minimum window size */
	case WM_GETMINMAXINFO:
		lpminmax = (LPMINMAXINFO)lParam;
		lpminmax->ptMinTrackSize.x = WND_X_EXTENTS * 2;
		lpminmax->ptMinTrackSize.y = WND_Y_EXTENTS * 2;
		break;

	/* paint window contents */
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hdc != NULL)
		{
			simDisplay(hWnd);
			EndPaint(hWnd, &ps);
		}
		break;

	/* map key presses to fake joystick movements */
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
		case 'Q':
			PostQuitMessage(0);
			break;

		case VK_SPACE:
			fakeport |= 0x01;
			break;

		case 'A':
			fakeport |= 0x02;
			break;

		case 'D':
			fakeport |= 0x04;
			break;

		case 'S':
			fakeport |= 0x08;
			break;

		case 'W':
			fakeport |= 0x10;
			break;

		default:
			return DefWindowProcA(hWnd, msg, wParam, lParam);
			break;
		}
		break;

	/* map key releases to fake joystick movements */
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_SPACE:
			fakeport &= ~0x01;
			break;

		case 'A':
			fakeport &= ~0x02;
			break;

		case 'D':
			fakeport &= ~0x04;
			break;

		case 'S':
			fakeport &= ~0x08;
			break;

		case 'W':
			fakeport &= ~0x10;
			break;

		default:
			return DefWindowProcA(hWnd, msg, wParam, lParam);
			break;
		}
		break;

	/* refresh the LED matrix every 40 ms */
	case WM_TIMER:
		simDisplay(hWnd);
		break;

	/* quit application */
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	/* Windows' default handler */
	default:
		return DefWindowProcA(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}


/**
 * Entry point for starting the the display loop in a thread.
 * @param lpParam Free style arguments for the thread function (not used here).
 * @return Always zero.
 */
DWORD WINAPI simLoop(LPVOID lpParam)
{
	display_loop();
	return 0;
}


/**
 * Wait function which utilizes multimedia timers and thread synchronization
 * objects. Although this is much more complicated than calling the Sleep()
 * function, it is also much more precise.
 * @param ms The requested delay in milliseconds.
 */
void wait(int ms)
{
	TIMECAPS tc;
	MMRESULT mmresult;
	MMRESULT mmTimerEventId;
	UINT uResolution;

	/* check if fire button is pressed (and if it is, jump to the menu) */
	if (waitForFire) {
		if (fakeport & 0x01) {
			longjmp(newmode_jmpbuf, 43);
		}
	}

	/* retrieve timer resolution capabilities of the current system */
	mmresult = timeGetDevCaps(&tc, sizeof(tc));
	if (mmresult == TIMERR_NOERROR)
	{
		/* retrieve best resolution and configure timer services accordingly */
		uResolution = min(max(tc.wPeriodMin, 0), tc.wPeriodMax);
		mmresult = timeBeginPeriod(uResolution);
		if (mmresult == TIMERR_NOERROR)
		{
			/* actually retrieve a multimedia timer */
			mmTimerEventId = timeSetEvent(ms, uResolution, g_hWaitEvent, NULL,
				TIME_ONESHOT | TIME_CALLBACK_EVENT_PULSE);
			if (mmTimerEventId != NULL)
			{
				/* now halt until that timer pulses our wait event object */
				WaitForSingleObject(g_hWaitEvent, INFINITE);

				/* relieve the timer from its duties */
				timeKillEvent(mmTimerEventId);
			}

			/* relax timer service constraints */
			timeEndPeriod (uResolution);
		}
	}
}


/**
 * Main function of the windows simulator.
 * @param hInstance Instance handle given by the operating system.
 * @param hPrevInstance This parameter has no meaning in Win32.
 * @param lpCmdLine Pointer to a null terminated command line string.
 * @param nCmdShow Flags for showing the window minimized, maximized and so on.
 * @return Exit code, always 0 here.
 */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG msg;
	HANDLE hThread;
	UINT_PTR uTimerId;
	
	/* regster window class (with nice black background!) */
	if (simRegisterWindowClass(&wc, hInstance))
	{
		/* actually create the window and make it visible */
		if (simCreateWindow(&hWnd, hInstance, nCmdShow))
		{
			/* event handle for multimedia timer (for the wait() function) */
			g_hWaitEvent = CreateEventA(NULL, FALSE, FALSE, "Local\\WaitEvent");
			if (g_hWaitEvent != NULL)
			{
				/* start the display loop thread */
				hThread = CreateThread(NULL, 0, simLoop, NULL, 0, NULL);
				if (hThread != NULL)
				{
					/* ensure that the display loop stays responsive */
					SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

					/* issue a UI timer message every 40 ms (roughly 25 fps) */
					/* NOTE: this has nothing to do with the multimedia timer */
					uTimerId = SetTimer(hWnd, 23, 40, NULL);
					if (uTimerId != NULL)
					{
						/* standard Windows(R) message loop */
						/* (runs as long as the window hasn't been closed) */
						while (GetMessageA(&msg, NULL, 0, 0))
						{
							TranslateMessage(&msg);
							DispatchMessageA(&msg);
						}

						/* remove that UI timer */
						KillTimer(hWnd, uTimerId);
					}

					/* stop the display loop */
					TerminateThread(hThread, 0);
				}

				/* relieve wait event object from its duties */
				CloseHandle(g_hWaitEvent);
			}

			return msg.wParam;
		}
		MessageBoxA(HWND_DESKTOP, g_strErrorCreateWindow, g_strError, MB_OK);
	}
	MessageBoxA(HWND_DESKTOP, g_strErrorRegisterWindow, g_strError, MB_OK);

	return 0;
}

/*@}*/
