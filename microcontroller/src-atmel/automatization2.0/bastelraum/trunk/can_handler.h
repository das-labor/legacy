#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern uint8_t myaddr;

extern void can_handler(void);
extern void can_read_addr(void);

extern void can_send_output_status(void);

#endif // _CAN_HANDLER

