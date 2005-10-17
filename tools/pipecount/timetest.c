#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main (){
	short int i=0;
	usleep (100000);
	while (!++i);
	printf ("Clocks_per_sec=%d\ntime=(%d) %8.8X\n", CLOCKS_PER_SEC, sizeof(clock_t),clock());
	
	return 0;
}
