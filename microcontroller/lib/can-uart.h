#ifndef CANUART_H
#define CANUART_H

/*****************************************************************************
 * Send encapsulated CAN packets via UART
 *
 * 
 */

#include "config.h"
#include "can.h"
#include "can-encap.h"

/*****************************************************************************
 * Connection management
 */

// Initialize CAN over UART on port serial
void canu_init(char *serial);

// syncronize line
void canu_reset();

/*****************************************************************************
 * rcv
 */

// Returns Message or 0 if there is no complete message.
rs232can_msg * canu_get_nb();


rs232can_msg * canu_get();

/*****************************************************************************
 * transmit
 */
void canu_transmit(rs232can_msg *msg);

#endif
