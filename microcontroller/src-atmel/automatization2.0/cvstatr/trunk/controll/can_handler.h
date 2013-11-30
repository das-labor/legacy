#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

extern uint8_t myaddr;

extern void can_handler(void);
extern void read_can_addr(void);

extern void can_send_stat(uint8_t stat_sw, uint8_t change);
extern void can_send_temp_data(uint8_t *data);

#endif // _CAN_HANDLER

