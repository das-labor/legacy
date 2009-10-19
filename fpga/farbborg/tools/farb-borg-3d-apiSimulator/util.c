#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef _WIN32
#	include <windows.h>
#endif

extern volatile unsigned int speed;

void myWait(unsigned int ms) {
#ifdef _WIN32
	Sleep(ms);
#else
	usleep(ms*10*(10000/speed));
#endif
}
