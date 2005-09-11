#include "can.h"
#include "lap.h"
#include "proto_lampe.h"

unsigned char lampe_set_lampe( can_addr dst, unsigned char lampe, unsigned char value)
{	
	pdo_message * msg  = (pdo_message *) can_buffer_get();
       	
	msg->port_dst = PORT_LAMPE;
	msg->port_src = PORT_MGT;
	msg->addr_dst = dst;
	msg->addr_src =	0x00;
	msg->dlc      = 0x03;
	msg->cmd      = FKT_LAMPE_SET;
	msg->data[0]  = lampe;
	msg->data[1]  = value;	

	can_transmit((can_message *)msg);
}



