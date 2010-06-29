
#include "lib/can.h"

#ifndef _LOCAL_CAN_H
#define _LOCAL_CAN_H

typedef struct{
	can_addr      addr_dst;
	can_addr      addr_src;
	can_port      port_dst;
	can_port      port_src;
	unsigned char dlc;
	unsigned char data[8];
}can_message_match;

#endif

