#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <can.h>
#include <lap.h>

#include "cmds-cantemp.h"





void cmd_cantemp(int argc, char *argv[]) 
{
	unsigned char addr;
	can_message *msg;
	msg = can_buffer_get();
	can_message *result;

	unsigned int mode = 0;

	unsigned int scanned = 0;

	if (argc < 2 || argc > 3)
		goto argerror;
	scanned = sscanf(argv[1], "%x", (unsigned int*)&addr);
	if (argc == 3)
		scanned += sscanf(argv[2], "%x", (unsigned int*)&mode);

	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = 0x00;
	msg->port_dst = 0x10;
	msg->dlc      = 1;
	msg->data[0]  = 0;


	can_transmit(msg);

	for (;;)
	{
		result = can_get();

		if (result->addr_src == addr)
		{
		  if(mode == 0){
			printf("Temp is %d\n", result->data[0]);
			return;
		  }
		  if(mode == 1){
		    printf("Temp is %d\n", result->data[1]);
		  }
		  return;
		}
	}

argerror:
	debug(0, "cantemp <addr> [mode]");
};

