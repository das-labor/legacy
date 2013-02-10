#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern void can_handler();

extern void can_send(uint8_t port, uint8_t *p);

extern void can_send2(uint8_t p);

extern void read_can_addr();

extern uint8_t myaddr;

#endif // _CAN_HANDLER
