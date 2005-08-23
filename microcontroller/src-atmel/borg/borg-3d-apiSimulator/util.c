#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void wait(unsigned int ms) {
	usleep(ms*1000);
}
