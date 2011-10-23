#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "can-encap.h"
#include "can.h"

void cmd_gateway(int argc, char *argv[])
{
	rs232can_msg rmsg;
	rmsg.cmd = RS232CAN_PING_GATEWAY;
	rmsg.len = 0;

	printf("Pinging gateway.. reply will be visible in debug log of Cand.\n");
	can_transmit_raw_gateway_message(&rmsg);
}
