#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern void can_handler();
void switch_handler();

void read_can_addr();

static uint8_t myaddr;

#endif // _CAN_HANDLER
