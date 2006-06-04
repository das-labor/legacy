#include "can.h"
#include "lap.h"
#include "proto_borg.h"

unsigned char borg_scroll_borg( can_addr addr,char *src)
{	
	pdo_message *msg;
	char * dst;
	// dst
	msg = (pdo_message *)can_buffer_get();
	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = 0x23;
	msg->port_dst = 0x23;
	msg->dlc      = 1;
	msg->cmd      = 2;

	dst = msg->data;
	while( (*src) && (msg->dlc < 8)) {
		*dst++ = *src++;
		msg->dlc++;
	};
		
	// first packet
	can_transmit((can_message*)msg);

	while(*src) {
		msg = (pdo_message *)can_buffer_get();

		msg->addr_src = 0x00;
		msg->addr_dst = addr;
		msg->port_src = 0x23;
		msg->port_dst = 0x23;
		msg->dlc      = 1;
		msg->cmd      = 3;

		dst = msg->data;
		while( (*src) && (msg->dlc < 8)) {
			*dst++ = *src++;
			msg->dlc++;
		};
		
		can_transmit((can_message*)msg);
	}

	return;
}
unsigned char borg_fire_borg(can_addr addr){
	pdo_message *msg;
	msg = (pdo_message *)can_buffer_get();

	msg->addr_src = 0x00;
	msg->addr_dst = addr;
	msg->port_src = 0x23;
	msg->port_dst = 0x23;
	msg->dlc      = 2;
	msg->cmd      = FKT_BORG_MODE;
	msg->data[0]  = 1;
	can_transmit((can_message*)msg);

	return;


} 



