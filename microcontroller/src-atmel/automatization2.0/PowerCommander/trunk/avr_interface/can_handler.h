#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern uint8_t myaddr;

extern void can_handler(void);
extern void can_send(uint8_t port, uint8_t *p);
extern void can_send2(uint8_t p);
extern void read_can_addr(void);

#endif // _CAN_HANDLER

