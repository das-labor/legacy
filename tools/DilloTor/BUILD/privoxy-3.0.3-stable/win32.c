const char win32_rcs[] = "$Id: win32.c,v 1.9.2.2 2002/08/27 18:03:40 oes Exp $";
/*********************************************************************
 *
 * File        :  $Source: /cvsroot/ijbswa/current/Attic/win32.c,v $
 *
 * Purpose     :  Win32 User Interface initialization and message loop
 *
 * Copyright   :  Written by and Copyright (C) 2001-2002 members of
 *                the Privoxy team.  http://www.privoxy.org/
 *
 *                Written by and Copyright (C) 1999 Adam Lock
 *                <locka@iol.ie>
 *
 *                This program is free software; you can redistribute it 
 *                and/or modify it under the terms of the GNU General
 *                Public License as published by the Free Software
 *                Foundation; either version 2 of the License, or (at
 *                your option) any later version.
 *
 *                This program is distributed in the hope that it will
 *                be useful, but WITHOUT ANY WARRANTY; without even the
 *                implied warranty of MERCHANTABILITY or FITNESS FOR A
 *                PARTICULAR PURPOSE.  See the GNU General Public
 *                License for more details.
 *
 *                The GNU General Public License should be included with
 *                this file.  If not, you can view it at
 *                http://www.gnu.org/copyleft/gpl.html
 *                or write to the Free Software Foundation, Inc., 59
 *                Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Revisions   :
 *    $Log: win32.c,v $
 *    Revision 1.9.2.2  2002/08/27 18:03:40  oes
 *    Fixed stupid typo
 *
 *    Revision 1.9.2.1  2002/08/21 17:59:27  oes
 *    Sync win32_blurb[] with our standard blurb
 *
 *    Revision 1.9  2002/03/31 17:19:00  jongfoster
 *    Win32 only: Enabling STRICT to fix a VC++ compile warning.
 *
 *    Revision 1.8  2002/03/26 22:57:10  jongfoster
 *    Web server name should begin www.
 *
 *    Revision 1.7  2002/03/24 12:03:47  jongfoster
 *    Name change
 *
 *    Revision 1.6  2002/03/16 21:53:28  jongfoster
 *    VC++ Heap debug option
 *
 *    Revision 1.5  2002/03/04 23:47:30  jongfoster
 *    - Rewritten, simpler command-line pre-parser
 *    - not using raise(SIGINT) any more
 *
 *    Revision 1.4  2001/11/30 21:29:33  jongfoster
 *    Fixing a warning
 *
 *    Revision 1.3  2001/11/16 00:46:31  jongfoster
 *    Fixing compiler warnings
 *
 *    Revision 1.2  2001/07/29 19:32:00  jongfoster
 *    Renaming _main() [mingw32 only] to real_main(), for ANSI compliance.
 *
 *    Revision 1.1.1.1  2001/05/15 13:59:08  oes
 *    Initial import of version 2.9.3 source tree
 *
 *
 *********************************************************************/


#include "config.h"

#ifdef _WIN32

#include <stdio.h>

#include "project.h"
#include "jcc.h"
#include "miscutil.h"

/* Uncomment this if you want to build Win32 as a console app */
/* #define _WIN_CONSOLE */

#ifndef STRICT
#define STRICT
#endif
#include <windows.h>

#include <stdarg.h>
#include <process.h>

#if defined(_WIN32) && defined(_MSC_VER) && defined(_DEBUG)
/* Visual C++ Heap debugging */
#include <crtdbg.h>
#endif /* defined(_WIN32) && defined(_MSC_VER) && defined(_DEBUG) */

#include "win32.h"

const char win32_h_rcs[] = WIN32_H_VERSION;

const char win32_blurb[] =
"Privoxy version " VERSION " for Windows\n"
"Copyright (C) 2000-2002 the Privoxy Team (" HOME_PAGE_URL ")\n"
"Based on the Internet Junkbuster by Junkbusters Corp.\n"
"This is free software; it may be used and copied under the\n"
"GNU General Public License: http://www.gnu.org/copyleft/gpl.html .\n"
"This program comes with ABSOLUTELY NO WARRANTY OF ANY KIND.\n";

#ifdef _WIN_CONSOLE

int hideConsole     = 0;

#else

HINSTANCE g_hInstance;
int g_nCmdShow;

static void  __cdecl UserInterfaceThread(void *);

#endif


/*********************************************************************
 *
 * Function    :  WinMain
 *
 * Description :  M$ Windows "main" routine:
 *                parse the `lpCmdLine' param into main's argc and argv variables,
 *                start the user interface thread (for the systray window), and
 *                call main (i.e. patch execution into normal startup).
 *
 * Parameters  :
 *          1  :  hInstance = instance handle of this execution
 *          2  :  hPrevInstance = instance handle of previous execution
 *          3  :  lpCmdLine = command line string which started us
 *          4  :  nCmdShow = window show value (MIN, MAX, NORMAL, etc...)
 *
 * Returns     :  `main' never returns, so WinMain will also never return.
 *
 *********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   int i;
   int res;
   int argc = 1;
   const char *argv[3];
   char szModule[MAX_PATH+1];
#ifndef _WIN_CONSOLE
   HANDLE hInitCompleteEvent = NULL;
#endif


#if defined(_WIN32) && defined(_MSC_VER) && defined(_DEBUG)
#if 0
   /* Visual C++ Heap debugging */

   /* Get current flag*/
   int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

   /* Turn on leak-checking bit */
   tmpFlag |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF;

   /* Turn off CRT block checking bit */
   tmpFlag &= ~(_CRTDBG_CHECK_CRT_DF | _CRTDBG_DELAY_FREE_MEM_DF);

   /* Set flag to the new value */
   _CrtSetDbgFlag( tmpFlag );
#endif
#endif /* defined(_WIN32) && defined(_MSC_VER) && defined(_DEBUG) */

   /*
    * Cheat in parsing the command line.  We only ever have at most one
    * paramater, which may optionally be specified inside double quotes.
    */

   if (lpCmdLine != NULL)
   {
      /* Make writable copy */
      lpCmdLine = strdup(lpCmdLine);
   }
   if (lpCmdLine != NULL)
   {
      chomp(lpCmdLine);
      i = strlen(lpCmdLine);
      if ((i >= 2) && (lpCmdLine[0] == '\"') && (lpCmdLine[i - 1] == '\"'))
      {
         lpCmdLine[i - 1] = '\0';
         lpCmdLine++;
      }
      if (lpCmdLine[0] == '\0')
      {
         lpCmdLine = NULL;
      }
   }

   GetModuleFileName(hInstance, szModule, MAX_PATH);
   argv[0] = szModule;
   argv[1] = lpCmdLine;
   argv[2] = NULL;
   argc = ((lpCmdLine != NULL) ? 2 : 1);

#ifndef _WIN_CONSOLE
   /* Create a user-interface thread and wait for it to initialise */
   hInitCompleteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   g_hInstance = hInstance;
   g_nCmdShow = nCmdShow;
   _beginthread(UserInterfaceThread, 0, &hInitCompleteEvent);
   WaitForSingleObject(hInitCompleteEvent, INFINITE);
   DeleteObject(hInitCompleteEvent);
#endif

#ifdef __MINGW32__
   res = real_main( argc, argv );
#else
   res = main( argc, argv );
#endif

   return res;

}

#endif

/*********************************************************************
 *
 * Function    :  InitWin32
 *
 * Description :  Initialise windows, setting up the console or windows as appropriate.
 *
 * Parameters  :  None
 *
 * Returns     :  N/A
 *
 *********************************************************************/
void InitWin32(void)
{
   WORD wVersionRequested;
   WSADATA wsaData;

#ifdef _WIN_CONSOLE
   SetProcessShutdownParameters(0x100, SHUTDOWN_NORETRY);
   if (hideConsole)
   {
      FreeConsole();
   }
#endif
   wVersionRequested = MAKEWORD(2, 0);
   if (WSAStartup(wVersionRequested, &wsaData) != 0)
   {
#ifndef _WIN_CONSOLE
      MessageBox(NULL, "Cannot initialize WinSock library", "Privoxy Error", 
         MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);  
#endif
      exit(1);
   }

}


#ifndef _WIN_CONSOLE
#include <signal.h>
#include <assert.h>

#include "win32.h"
#include "w32log.h"


/*********************************************************************
 *
 * Function    :  UserInterfaceThread
 *
 * Description :  User interface thread.  WinMain will wait for us to set
 *                the hInitCompleteEvent before patching over to `main'.
 *                This ensures the systray window is active before beginning
 *                operations.
 *
 * Parameters  :
 *          1  :  pData = pointer to `hInitCompleteEvent'.
 *
 * Returns     :  N/A
 *
 *********************************************************************/
static void __cdecl UserInterfaceThread(void *pData)
{
   MSG msg;
   HANDLE hInitCompleteEvent = *((HANDLE *) pData);

   /* Initialise */
   InitLogWindow();
   SetEvent(hInitCompleteEvent);

   /* Enter a message processing loop */
   while (GetMessage(&msg, (HWND) NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   /* Cleanup */
   TermLogWindow();

   /* Time to die... */
   exit(0);

}


#endif


/*
  Local Variables:
  tab-width: 3
  end:
*/
