#include "can.h"
#include "lap.h"
#include "proto_lampe.h"

unsigned char lampe_set_lampe( unsigned char lampe, unsigned char value)
{
	pdo_message *msg = (pdo_message *)can_buffer_get();

	msg->addr_src = 0x01;
	msg->port_src = PORT_LAMPE;
	msg->addr_dst = 0x02;
	msg->port_dst = PORT_LAMPE;

	msg->dlc = 4;
	msg->fkt_id = FKT_LAMPE_SET;
}



