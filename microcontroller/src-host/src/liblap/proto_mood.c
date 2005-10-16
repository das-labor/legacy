#include "can.h"
#include "lap.h"
#include "proto_mood.h"

struct moodvals{
uint8_t v1;
uint8_t v2;
uint8_t v3;
uint8_t v4;
};
unsigned char mood_set_mood( can_addr dst, unsigned char mood,unsigned char val1, unsigned char val2, unsigned char val3, unsigned char val4 )
{	
	pdo_message * msg  = (pdo_message *) can_buffer_get();
       	
	msg->port_dst = PORT_MOOD;
	msg->port_src = PORT_MGT;
	msg->addr_dst = dst;
	msg->addr_src =	0x00;
	msg->dlc      = 6;
	msg->cmd      = FKT_MOOD_SET;
	msg->data[0]  = mood;
	msg->data[1]  = val1;
	msg->data[2]  = val2;
	msg->data[3]  = val3;
	msg->data[4]  = val4;


	can_transmit((can_message *)msg);
}


int mood_get_mood(can_addr dst,int mood,int moodled){
	int mv;
	pdo_message * msg  = (pdo_message *) can_buffer_get();
	msg->port_dst = PORT_MOOD;
	msg->port_src = PORT_MGT;
	msg->addr_dst = dst;
	msg->addr_src =	0x00;
	msg->dlc      = 2;
	msg->cmd      = FKT_MOOD_GET;
	msg->data[0]  = mood;
	can_transmit((can_message *)msg);
	
	pdo_message * rmsg =(pdo_message*) can_get();
	while(rmsg->addr_src != dst && rmsg->addr_dst != 0x00 && rmsg->cmd != FKT_MOOD_GET){
		mv=msg->data[moodled];
	}
	return  mv;
}
