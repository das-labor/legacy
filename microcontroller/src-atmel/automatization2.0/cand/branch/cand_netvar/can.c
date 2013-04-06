#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib-host/can.h"
#include "lib-host/can-encap.h"
#include "lib/can_message_converters.h"

uint8_t myaddr;

//returns pointer to the next can TX buffer
can_message* can_buffer_get()
{
	can_message *cmsg = malloc(sizeof(can_message));
	if (cmsg == NULL)
	{
		printf("ERROR: can_buffer_get malloc fail!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		memset(cmsg, 0 , sizeof(can_message));
		return cmsg;
	}
}


extern void transmit_message_to_network_and_can(rs232can_msg *msg);

//transmit a can message
void can_transmit_raw(can_message_raw *cmsg)
{
	rs232can_msg rs232_msg;

	if (cmsg == NULL)
	{
		printf("ERROR: can_transmit_raw got NULL!\n");
		exit(EXIT_FAILURE);
	}

	rs232can_msg_from_can_message_raw(&rs232_msg, cmsg);
	transmit_message_to_network_and_can(&rs232_msg);
}

void can_transmit(can_message *cmsg)
{
	if (cmsg == NULL)
	{
		printf("ERROR: cmsg is NULL!\n");
		exit(EXIT_FAILURE);
	}
	can_message_raw raw_msg;
	can_message_raw_from_can_message(&raw_msg, cmsg);
	
	free(cmsg);
	
	can_transmit_raw(&raw_msg);
}
