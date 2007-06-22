#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "can.h"
#include "../../../../microcontroller-2/cansole/client/termio.h"

FILE *mytty;

void cansole_exit (int in_signal)
{
        printf ("\nCansole closing...\n");
        fclose (mytty);
	reset_keypress ();
        exit (in_signal);
}

cmd_cansole (int argc, char* argv[])
{
	uint8_t tmpchar = 0x00;
	can_message_raw *tmpmsg;
	can_message_v2 *txmsg;
	
	if (argc != 4)
	{
		printf("ERROR: cmd_console needs 3 arguments, got %i.\n", argc-1);
		exit (-1);
	}

	signal (SIGINT, cansole_exit);
	signal (SIGKILL, cansole_exit);

	mytty = fopen ("/dev/tty", "rw");
	fcntl(0, F_SETFL, O_NONBLOCK);
	tmpmsg = malloc(sizeof(can_message_raw));
	txmsg = malloc(sizeof(can_message_v2));

	set_keypress();
	printf("\n(echo test)\n");

	while (0x17)
	{
		tmpchar = getc(mytty);
		printf("got char: %x\n", tmpchar);
		if (tmpchar != 0xff)
		{
			printf("got char: %x\n", tmpchar);

			txmsg->channel = (uint8_t) atoi(argv[1]);
			txmsg->subchannel = (uint8_t) atoi(argv[2]);
			txmsg->addr_src = 0x00;
			txmsg->addr_dst = (uint8_t) atoi (argv[0]);
			can_transmit_raw (tmpmsg);
		}

		usleep (100);
	}
}
