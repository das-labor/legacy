#ifndef RS232CAN_H
#define RS232CAN_H

/*****************************************************************************
 * Send encapsulated CAN packets via UART
 */

#include "config.h"
#include "can.h"
#include "can-encap.h"

// syncronize line
void rs232can_reset();

rs232can_msg * rs232can_get_nb();
void rs232can_transmit(rs232can_msg *msg);

#endif
