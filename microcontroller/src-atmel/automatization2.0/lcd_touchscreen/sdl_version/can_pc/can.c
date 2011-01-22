
#include <stdint.h>

#include "can.h"

uint8_t myaddr = 0;

void can_transmit(can_message *msg){
	printf("Can msg\r\n");
}