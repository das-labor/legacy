#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.h"

#include "can.h"
#include "can-encap.h"
#include "cann.h"
#include "can-uart.h"

/***************************************************************************
 * Implement can.h for posix systems 
 *
 * Uses cann.[hc] for cand or can-uart.[hc] according to first call to 
 * can_init( ... );
 */


static cann_conn_t *conn;

/************************************************************************+**
 * Management 
 */

void can_init(cann_conn_t *aconn) {
	conn = aconn;
}

void can_setmode(can_mode_t mode)
{
	rs232can_msg rmsg;

	rmsg.len     = 1; 
	rmsg.cmd     = RS232CAN_SETMODE;
	rmsg.data[0] = (char)mode;

	if (conn)
		cann_transmit(conn, &rmsg);
	else
		canu_transmit(&rmsg);
}

void can_setfilter() {
	// XXX
}

/****************************************************************************
 * Sending
 */

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

can_message_raw* can_buffer_get_raw()
{
	can_message_raw *cmsg = malloc(sizeof(can_message_raw));
	if (cmsg == NULL)
	{
		printf("ERROR: can_buffer_get_raw malloc fail!\n");
		exit(EXIT_FAILURE);
	}
	else
		return cmsg;
}

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

	if (conn)
		cann_transmit(conn, &rmsg);
	else
		canu_transmit(&rmsg);
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
	free(cmsg);
	can_transmit_raw(newmsg);

	free(newmsg);
}

/****************************************************************************
 * reciving
 */

// XXX must free messages
	
//returns next can message
can_message * can_get_nb()
{
	rs232can_msg *rmsg;
	can_message_raw  *cmsg;
	can_message *retmsg;

	if (conn)
		rmsg = cann_get(conn);
	else
		rmsg = canu_get();

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
	
	cmsg = can_buffer_get_raw();
	rs232can_rs2can(cmsg, rmsg);
	free(rmsg);
	retmsg->addr_src = (uint8_t) (cmsg->id >> 8);
	retmsg->addr_dst = (uint8_t) (cmsg->id);
	retmsg->port_src = (uint8_t) ((cmsg->id >> 23) & 0x3f);
	retmsg->port_dst = (uint8_t) (((cmsg->id >> 16) & 0x0f) | ((cmsg->id >> 17) & 0x30));
	retmsg->dlc = cmsg->dlc;
	memcpy(retmsg->data, cmsg->data, cmsg->dlc);

	
	free (cmsg);
	return retmsg;
}

can_message *can_get()
{
	return can_get_nb();
}

//returns next can message, or 0 if no Message was received.
can_message_raw * can_get_raw_nb() {
	rs232can_msg *rmsg;
	can_message_raw  *cmsg;

	if (conn)
		rmsg = cann_get_nb(conn);
	else
		rmsg = canu_get_nb();

	if (!rmsg) return 0;

	cmsg = can_buffer_get_raw();
	rs232can_rs2can(cmsg, rmsg);

	return cmsg;

}

can_message_raw* can_v2_to_raw(can_message_v2 *in_message)
{
	can_message_raw *retmsg;
	retmsg = malloc(sizeof(can_message_raw));
	if (retmsg == NULL)
	{
		printf("ERROR: Could not allocate retmsg buffer!\n");
		exit(EXIT_FAILURE);
	}
	retmsg->id = 0x0000;
	
	retmsg->id |= ((uint32_t) in_message->channel << 20);
	retmsg->id |= ((uint32_t)in_message->subchannel << 18);
	retmsg->id |= ((uint16_t) in_message->addr_src << 8);
	retmsg->id |= in_message->addr_dst;
	retmsg->dlc = in_message->dlc;
	memcpy(retmsg->data, in_message->data, 8);

	return retmsg;
}

void can_transmit_v2(can_message_v2 *in_msg)
{
	can_message_raw *txmsg;

	txmsg = can_v2_to_raw (in_msg);
	can_transmit_raw (txmsg);
}


can_message_v2 * can_get_v2_nb()
{
	rs232can_msg *rmsg;
	can_message_raw  *cmsg;
	can_message_v2 *retmsg;

	if (conn)
		rmsg = cann_get_nb(conn);
	else
		rmsg = canu_get_nb();

	if (!rmsg) return 0;

	retmsg = malloc(sizeof(can_message_v2));
	if (retmsg == NULL)
	{
		printf("ERROR: Could not allocate retmsg buffer!\n");
		exit(EXIT_FAILURE);
	}
	
	cmsg = can_buffer_get_raw();
	rs232can_rs2can(cmsg, rmsg);

	retmsg->addr_src = (uint8_t)  (cmsg->id >> 8);
	retmsg->addr_dst = (uint8_t)  (cmsg->id);
	retmsg->channel = (uint16_t)  ((cmsg->id >> 20) & 0x1ff);
	retmsg->subchannel = (uint8_t)((cmsg->id >> 18 ) & 0x03);
	retmsg->dlc = cmsg->dlc;
	memcpy(retmsg->data, cmsg->data, 8);

	free (cmsg);
	return retmsg;
}



void can_free(can_message *msg){
	free(msg);
}

void can_free_v2(can_message_v2 *msg){
	free(msg);
}
