#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern uint8_t myaddr;

extern void can_handler(void);
extern void read_can_addr(void);

extern void can_send_output_status(void);
extern void can_send_input_stat(uint8_t pos, uint8_t val);

#endif // _CAN_HANDLER

