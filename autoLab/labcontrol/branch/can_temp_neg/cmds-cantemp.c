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

	char temperatur=0;

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
		    if(result->data[0] > 127) 
		      {
			temperatur = (char)((   (unsigned char)( ((result->data[0]) ^0xff)+1)) *(-1));
		      }
		    else
		      {
			temperatur = result->data[0];
		      }
		  }
		  if(mode == 1){
		    if(result->data[1] > 127) 
		      {
			temperatur = (char)((   (unsigned char)( ((result->data[1]) ^0xff)+1)) *(-1));
		      }
		    else
		      {
			temperatur = (char)result->data[1];
		      }
		  }
		  printf("Temp is %d\n", temperatur);
		  return;
		}
	}

argerror:
	debug(0, "cantemp <addr> [mode]");
};

