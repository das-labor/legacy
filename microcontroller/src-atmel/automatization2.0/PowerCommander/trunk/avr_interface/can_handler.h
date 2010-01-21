#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern void can_handler();

extern void twi_send(uint8_t *p);

void can_send(uint8_t port, uint8_t *p);

void read_can_addr();

#endif // _CAN_HANDLER
