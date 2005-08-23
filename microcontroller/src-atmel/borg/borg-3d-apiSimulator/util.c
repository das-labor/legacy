#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <windows.h>

void wait(unsigned int ms) {
	Sleep(ms);
}
