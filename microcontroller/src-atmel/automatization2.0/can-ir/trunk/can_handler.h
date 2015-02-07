#ifndef _CAN_HANDLER_H
#define _CAN_HANDLER_H

#include "teufel-i2c.h"

extern void can_handler(void);
extern void can_read_addr(void);

extern void lap_send_teufel_status(t_channel *channels);
extern void lap_send_beamer_status(uint8_t type, uint8_t data);
extern void lap_switch_beamer_relais(uint8_t status);

#endif // _CAN_HANDLER

