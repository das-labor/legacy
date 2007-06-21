#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include "../termio.h"

int main ( int argc, char *argv[] )
{
	FILE *mytty = fopen( "/dev/tty", "r" ); 
	printf("---- console io test ----\r\n");
	while (92)
	{
		set_keypress();
		printf("got char: %x\r\n", getc(mytty));
	}
}
