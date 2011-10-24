#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "can-encap.h"
#include "can.h"

void cmd_gateway(int argc, char *argv[])
{
	rs232can_msg rmsg, *rmsg_in;
	unsigned int i = 0;

	//create and send gateway ping request
	printf("Pinging gateway.. ");
	rmsg.cmd = RS232CAN_PING_GATEWAY;
	rmsg.len = 0;
	can_transmit_raw_gateway_message(&rmsg);

	//anticipate reply and timeout after 2s
	while((rmsg_in = can_get_raw_gateway_message_nb() == NULL) && i++ < 2000) usleep(1000);
	printf((i<2000)?"and got a reply!\n":"but timed out after 2 seconds :-(\n");
}
