#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main()
{
	uint32_t state, c;

	state = 0x01;
	for(;;) {
//		usleep( 1000);
		c = state & 0xff;
		putchar(c);
		fflush(stdout);
		fprintf( stderr, "%x\n", c );
		state++;
	}
}

