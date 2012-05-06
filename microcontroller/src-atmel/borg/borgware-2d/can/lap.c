#include <stdio.h>
#include <string.h>

#include "can.h"
#include "lap.h"

/****************************************************************************
 * STUBS: LAP Core Services 
 */

// send ping to dst
void lap_ping(can_addr_t dst) {
	pdo_message *msg = (pdo_message *) can_buffer_get();

	msg->addr_src = 0;
	msg->addr_dst = dst;
	msg->port_src = PORT_MGT;
	msg->port_dst = PORT_MGT;
	msg->dlc = 1;
	msg->cmd = FKT_MGT_PING;

	can_transmit((can_message *) msg);
}

// send reset request to dst
void lap_reset(can_addr_t dst) {
	pdo_message *msg = (pdo_message *) can_buffer_get();

	msg->addr_src = 0;
	msg->addr_dst = dst;
	msg->port_src = PORT_MGT;
	msg->port_dst = PORT_MGT;
	msg->dlc = 1;
	msg->cmd = FKT_MGT_RESET;

	can_transmit((can_message *) msg);
}

/*
char *sdo_readbuf(lap_message *first_msg, 
		unsigned char int length, unsigned char &reallength)
{
	unsigned char len = first_message->data[0];
	char *buf = malloc(length);
	char *cur = buf;

	reallength = 0;
	while( reallength < length ) {
		can_message *msg = can_get();
		
		if ( msg->addr_src  != first_msg->addr_src  ||
		     msg->addr_dest != fisrt_msg->addr_dest ||
		     msg->port_src  != fisrt_msg->port_src  ||
		     msg->port_dest != fisrt_msg->port_dest )
		{
			return buf;
		}

		memcpy(cur, msg->data, msg->dlc);
		reallength+=msg->dlc;
		msg->flags = 0x00;
	}

	return buf;
}

// unsigned char sdo_sendpacket(lap_message *msg)
// {
// }
// 
// unsigned char sdo_sendpacket_nb(lap_message *msg)
// {
// }

unsigned char sdo_sendbuf(lap_message *fst_msg, unsigned char *buf, unsigned char len)
{
	while(len > 0) {
		can_message *msg = can_buffer_get();
		msg->addr_src  = fst_msg->addr_src;
		msg->addr_dest = fst_msg->addr_dest;
		msg->port_src  = fst_msg->port_src;
		msg->port_dest = fst_msg->port_dest;

		msg->dlc = len > 8 ? 8 : len;
		memcpy(msg->data, buf, msg->dlc);
		msg->flags = 0x01;
		can_transmit();
	}
	// XXX wait for ACK
}


unsigned char sdo_sendbuf_nb(lap_message *fst_msg, unsigned char *buf, unsigned char len)
{
	while(len > 0) {
		can_message *msg = can_buffer_get();
		msg->addr_src  = fst_msg->addr_src;
		msg->addr_dest = fst_msg->addr_dest;
		msg->port_src  = fst_msg->port_src;
		msg->port_dest = fst_msg->port_dest;

		msg->dlc = len > 8 ? 8 : len;
		memcpy(msg->data, buf, msg->dlc);
		msg->flags = 0x01;
		can_transmit();
	}
	// XXX wait for ACK
}
*/
