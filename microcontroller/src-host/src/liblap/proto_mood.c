#include "can.h"
#include "lap.h"
#include "proto_mood.h"

unsigned char mood_set_mood( can_addr dst, unsigned char val1, unsigned char val2, unsigned char val3, unsigned char val4 )
{	
	pdo_message * msg  = (pdo_message *) can_buffer_get();
       	
	msg->port_dst = PORT_MOOD;
	msg->port_src = PORT_MGT;
	msg->addr_dst = dst;
	msg->addr_src =	0x00;
	msg->dlc      = 5;
	msg->cmd      = FKT_MOOD_SET;
	msg->data[0]  = val1;
	msg->data[1]  = val2;
	msg->data[2]  = val3;
	msg->data[3]  = val4;


	can_transmit((can_message *)msg);
}



