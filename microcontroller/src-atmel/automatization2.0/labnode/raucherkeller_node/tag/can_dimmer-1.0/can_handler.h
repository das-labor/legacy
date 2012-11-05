#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern void can_handler();

void can_send(uint8_t port, uint8_t *p);

void read_can_addr();

extern uint8_t myaddr;

#endif // _CAN_HANDLER

