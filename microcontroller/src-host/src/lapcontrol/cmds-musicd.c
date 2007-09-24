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
	can_message *msg;

	while (msg = (can_message*) can_get())
	{
		// incoming message from switch
		if (msg->addr_src == 0x01 && msg->port_src == 0x21
				&& msg->addr_dst == 0xff && msg->port_dst == 0x21)
		{
			if (msg->data[0] == 0x50 && msg->data[1] == 0x45 &&
					msg->data[2] == 0x51 && msg->data[3] == 0x00)
			{
						system ("mpc next");
						printf("foo!!\n");
			}
		}
	}
}
