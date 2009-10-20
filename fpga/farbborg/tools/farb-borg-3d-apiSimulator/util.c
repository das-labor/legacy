#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef _WIN32
#	include <windows.h>
#endif

extern volatile unsigned int speed;
extern int curFrame;

void myWait(unsigned int ms) {
    curFrame++;
#ifdef _WIN32
    while (speed == 0)
        Sleep(10);
	Sleep((ms*100)/speed);
#else
	while (speed == 0)
        usleep(10000); // pause
	usleep(ms*10*(10000/speed));
#endif
}
