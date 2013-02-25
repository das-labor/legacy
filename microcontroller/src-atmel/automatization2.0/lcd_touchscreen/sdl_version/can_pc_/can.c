#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "can.h"
#include "cann.h"

uint8_t myaddr = 0;

static cann_conn_t *cann_conn;


//transmit a can message
void can_transmit_raw(can_message_raw *cmsg)
{
	rs232can_msg rmsg;
	
	if (cmsg == NULL)
	{
		printf("ERROR: can_transmit_raw got NULL!\n");
		exit(EXIT_FAILURE);
	}

	rs232can_can2rs(&rmsg, cmsg);

	cann_transmit(cann_conn, &rmsg);
}

void can_transmit(can_message *cmsg)
{
	if (cmsg == NULL)
	{
		printf("ERROR: cmsg is NULL!\n");
		exit(EXIT_FAILURE);
	}
	can_message_raw *newmsg = malloc(sizeof(can_message_raw));
	memset(newmsg, 0, sizeof(can_message_raw));
	if (newmsg == NULL)
	{
		printf("ERROR: Could not allocate newmsg buffer!\n");
		exit(EXIT_FAILURE);
	}

	newmsg->id = ((cmsg->port_src & 0x3f) << 23) | ((cmsg->port_dst & 0x30) << 17) |
		((cmsg->port_dst & 0x0f) << 16) | (cmsg->addr_src << 8) | (cmsg->addr_dst);
	newmsg->dlc = cmsg->dlc;
	memcpy(newmsg->data, cmsg->data, cmsg->dlc);

	can_transmit_raw(newmsg);
	free(newmsg);
}

//returns next can message
can_message * can_get_nb()
{
	rs232can_msg *rmsg;
	can_message_raw  cmsg;
	can_message *retmsg;

	rmsg = cann_get_nb(cann_conn);
	
	if (!rmsg) 
	{
		free(rmsg);
		return 0;
	}
	
	retmsg = malloc(sizeof(can_message));
	if (retmsg == NULL)
	{
		printf("ERROR: Could not allocate retmsg buffer!\n");
		exit(EXIT_FAILURE);
	}
	
	rs232can_rs2can(&cmsg, rmsg);
	free(rmsg);
	retmsg->addr_src = (uint8_t) (cmsg.id >> 8);
	retmsg->addr_dst = (uint8_t) (cmsg.id);
	retmsg->port_src = (uint8_t) ((cmsg.id >> 23) & 0x3f);
	retmsg->port_dst = (uint8_t) (((cmsg.id >> 16) & 0x0f) | ((cmsg.id >> 17) & 0x30));
	retmsg->dlc = cmsg.dlc;
	memcpy(retmsg->data, cmsg.data, cmsg.dlc);

	return retmsg;
}

can_message *can_get()
{
	return can_get_nb();
}

void can_init() {
	cann_conn = cann_connect("homeroute", "2342");
}

