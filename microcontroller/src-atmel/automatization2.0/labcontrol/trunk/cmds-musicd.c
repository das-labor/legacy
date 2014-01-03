#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <lib-host/can.h>
#include <lib/lap.h>

#include "cmds-musicd.h"

void cmd_musicd(int argc, char **argv)
{
	int target_addr;
	uint32_t tmp, lastcommand;
	time_t lastexec, currentt;
	can_message *msg;

	time(&lastexec);

	while (msg = (can_message*) can_get())
	{
		// incoming message from switch
		if (msg->addr_src == 0x01 && msg->port_src == 0x21
				&& msg->addr_dst == 0xff && msg->port_dst == 0x21)
		{
			tmp = ((uint32_t) msg->data[0] << 24) |
				((uint32_t) msg->data[1] << 16) |
				((uint32_t) msg->data[2] << 8) |
				((uint32_t) msg->data[3]);
			
			time (&currentt);

			switch (tmp)
			{
				case 0x50455400:
						if (lastcommand == tmp && difftime(lastexec, currentt) < (double) 2) break;
						system ("mpc next > /dev/null");
						printf("next\n");
				break;
				case 0x50455100:
						if (lastcommand == tmp && difftime(lastexec, currentt) < 2) break;
						system ("mpc last > /dev/null");
						printf("last\n");
				break;
				case 0x50515100:
						system ("mpc volume -2 > /dev/null");
						printf("vol--\n");
				break;
				case 0x50515400:
						system ("mpc volume +2 > /dev/null");
						printf("vol++\n");
				break;
				case 0x50554400:
				case 0x50554100:
						if (lastcommand == tmp && difftime(lastexec, currentt) < 2) break;
						system ("mpc clear > /dev/null");
						system ("mpc add http://bombastix.soundbomb.net:8000/nectarine.ogg > /dev/null");
						system ("mpc play");
				break;
				case 0x50545400:
						if (lastcommand == tmp && difftime(lastexec, currentt) < 2) break;
						system ("mpc pause > /dev/null");
						system ("espeak -a 1000 \"nakka nakka nakka\"");
						system ("mpc play > /dev/null");
				break;
			}
			lastcommand = tmp;
			time(&lastexec);
		}
	}
}
