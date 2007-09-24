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
		if (msg->addr_src == 0x01)
		{
			if (msg->data[0] == 'm')
			{
				switch (msg->data[1])
				{
					case '0x00':
						system ("mpc next");
					break;
				}
			}
		}
	}
}
