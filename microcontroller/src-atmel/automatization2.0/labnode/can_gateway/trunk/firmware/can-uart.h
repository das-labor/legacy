#ifndef CANUART_H
#define CANUART_H

/*****************************************************************************
 * Send encapsulated CAN packets via UART
 *
 * 
 */

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
 * Memory Management
 */
rs232can_msg *canu_buffer_get();
void canu_free(rs232can_msg *);


/*****************************************************************************
 * rcv
 */

/* nonblocking read from uart -- returns 0 if no complete msg arrived */
rs232can_msg *canu_get_nb();

/* blocking read from uart */
rs232can_msg *canu_get();


/*****************************************************************************
 * transmit
 */
void canu_transmit(rs232can_msg *msg);

#endif
