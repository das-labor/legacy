#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "lib-host/can.h"
#include "cansole/client/termio.h"

int mytty;

void cansole_exit (int in_signal)
{
        printf ("\nCansole closing...\n");
        close (mytty);
	reset_keypress ();
        exit (in_signal);
}

cmd_cansole (int argc, char* argv[])
{
	uint8_t tmpchar = 0x00;
	can_message_raw *tmpmsg;
	can_message_v2 *txmsg, *rxmsg;

	if (argc != 3)
	{
		printf("ERROR: cmd_console needs 2 arguments, got %i.\n", argc-1);
		exit (-1);
	}

	signal (SIGINT, cansole_exit);
	signal (SIGKILL, cansole_exit);

	mytty = open ("/dev/tty", O_NONBLOCK | O_RDWR );

	txmsg = malloc(sizeof(can_message_v2));
	if (txmsg == NULL)
	{
		printf("ERROR: Could not allocate txmsg buffer!\n");
		exit(EXIT_FAILURE);
	}
	txmsg->channel = (uint8_t) strtol(argv[2],0,0);
	txmsg->subchannel = 1;
	txmsg->addr_src = 0x00;
	txmsg->addr_dst = (uint8_t) strtol (argv[1],0,0);
	txmsg->dlc = 1;

	set_keypress();
	printf("\n(echo test)\n");

	while (23)
	{
		ssize_t s;

		s = read(mytty, &tmpchar, 1);
		if (s == 1)
		{
			printf("got char: %x\n", tmpchar);
			txmsg->data[0] = tmpchar;
			can_transmit_v2 (txmsg);
		}

		rxmsg = can_get_v2_nb();
		if (rxmsg)
		{
			if ((rxmsg->addr_src == txmsg->addr_dst) &&
			  (rxmsg->channel == txmsg->channel) && (rxmsg->subchannel == 1))
			{
				write(mytty, rxmsg->data, rxmsg->dlc);
			}
		}
		usleep(1000);
	}
}
