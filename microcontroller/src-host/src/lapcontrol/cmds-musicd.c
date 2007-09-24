#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <can.h>
#include <lap.h>

#include "cmds-musicd.h"

void cmd_musicd(int argc, char **argv)
{
	int target_addr;
	uint32_t tmp;
	can_message *msg;

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

			switch (tmp)
			{
				case 0x50455400:
						system ("mpc next > /dev/null");
						printf("next\n");
						usleep(500);
				break;
				case 0x50455100:
						system ("mpc last > /dev/null");
						printf("last\n");
						usleep(500); //filter duplicates
				break;
				case 0x50515100:
						system ("mpc volume -2 > /dev/null");
						printf("vol--\n");
				break;
				case 0x50515400:
						system ("mpc volume +2 > /dev/null");
						printf("vol++\n");
				break;
			}
		}
	}
}
