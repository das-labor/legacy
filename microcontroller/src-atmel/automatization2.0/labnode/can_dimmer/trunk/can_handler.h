#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern void can_handler();

extern void send_status(uint8_t addr);

extern void read_can_addr();

extern uint8_t myaddr;

#endif // _CAN_HANDLER

