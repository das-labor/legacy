#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern uint8_t myaddr;

extern void can_handler(void);
extern void send_status(void);
extern void read_can_addr(void);

#endif // _CAN_HANDLER

