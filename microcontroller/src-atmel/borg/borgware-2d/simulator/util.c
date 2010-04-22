#ifdef _WIN32
	#include <windows.h>
#else
	#define _XOPEN_SOURCE 500
#endif

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <setjmp.h>
#include "joystick.h"

extern jmp_buf newmode_jmpbuf;

void wait(unsigned int ms) {
	if (waitForFire) {
		if (JOYISFIRE) {
			longjmp(newmode_jmpbuf, 43);
		}
	}
	
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}
