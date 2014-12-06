#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config.h"

#include "lib-host/can.h"
#include "lib-host/can-encap.h"
#include "lib-host/can-tcp.h"
#include "lib-host/can-uart.h"
#include "lib/can_message_converters.h"

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

void can_init_posix(cann_conn_t *aconn) {
	conn = aconn;
}

void can_setmode(can_mode_t mode)
{
	rs232can_msg rs232_msg;

	rs232_msg.len     = 1;
	rs232_msg.cmd     = RS232CAN_SETMODE;
	rs232_msg.data[0] = (char)mode;

	if (conn)
		cann_transmit(conn, &rs232_msg);
	else
		canu_transmit(&rs232_msg);
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

//transmit a raw rs232can message
void can_transmit_raw_gateway_message(rs232can_msg *rmsg)
{
	if (rmsg == NULL)
	{
		printf("ERROR: can_transmit_raw_gateway_message got NULL!\n");
		exit(EXIT_FAILURE);
	}

	if (conn)
		cann_transmit(conn, rmsg);
	else
		canu_transmit(rmsg);
}

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
	can_transmit_raw_gateway_message(&rs232_msg);
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
		
	can_transmit_raw(&raw_msg);
}

/****************************************************************************
 * reciving
 */

rs232can_msg * can_get_raw_gateway_message_nb()
{
	if (conn)
		return cann_get_nb(conn);
	else
		return canu_get_nb();
}

// XXX must free messages

//returns next can message
can_message *can_get_nb()
{
	rs232can_msg *rs232_msg;
	can_message_raw  raw_msg;
	can_message *can_msg;

	if (conn)
		rs232_msg = cann_get(conn);
	else
		rs232_msg = canu_get();

	if (!rs232_msg || rs232_msg->cmd != RS232CAN_PKT)
	{
		free(rs232_msg);
		return 0;
	}

	can_msg = malloc(sizeof(can_message));
	if (can_msg == NULL)
	{
		printf("ERROR: Could not allocate retmsg buffer!\n");
		exit(EXIT_FAILURE);
	}

	can_message_raw_from_rs232can_msg(&raw_msg, rs232_msg);
	can_message_from_can_message_raw(can_msg, &raw_msg);
	
	if (conn)
		free(rs232_msg);

	return can_msg;
}

can_message *can_get()
{
	return can_get_nb();
}

//returns next can message, or 0 if no Message was received.
can_message_raw * can_get_raw_nb() {
	rs232can_msg *rs232_msg;
	can_message_raw  *cmsg;

	if (conn)
		rs232_msg = cann_get_nb(conn);
	else
		rs232_msg = canu_get_nb();

	if (!rs232_msg) return 0;

	cmsg = can_buffer_get_raw();
	can_message_raw_from_rs232can_msg(cmsg, rs232_msg);

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
	rs232can_msg *rs232_msg;
	can_message_raw  *cmsg;
	can_message_v2 *retmsg;

	if (conn)
		rs232_msg = cann_get_nb(conn);
	else
		rs232_msg = canu_get_nb();

	if (!rs232_msg) return 0;

	retmsg = malloc(sizeof(can_message_v2));
	if (retmsg == NULL)
	{
		printf("ERROR: Could not allocate retmsg buffer!\n");
		exit(EXIT_FAILURE);
	}

	cmsg = can_buffer_get_raw();
	can_message_raw_from_rs232can_msg(cmsg, rs232_msg);

	retmsg->addr_src = (uint8_t)  (cmsg->id >> 8);
	retmsg->addr_dst = (uint8_t)  (cmsg->id);
	retmsg->channel = (uint16_t)  ((cmsg->id >> 20) & 0x1ff);
	retmsg->subchannel = (uint8_t)((cmsg->id >> 18 ) & 0x03);
	retmsg->dlc = cmsg->dlc;
	memcpy(retmsg->data, cmsg->data, 8);

	free (cmsg);
	return retmsg;
}

void can_free(can_message *msg)
{
	free(msg);
}

void can_free_v2(can_message_v2 *msg)
{
	free(msg);
}
