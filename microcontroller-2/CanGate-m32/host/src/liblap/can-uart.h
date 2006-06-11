#ifndef CANUART_H
#define CANUART_H

/*****************************************************************************
 * Send encapsulated CAN packets via UART
 *
 * 
 */

#include "config.h"
#include "liblap.h"

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

/* nonblocking read from uart -- returns -1 if no complete msg arrived */
int  canu_get_nb(can_message_t *msg);

/* blocking read from uart */
void canu_get(can_message_t *msg);


/*****************************************************************************
 * transmit
 */
void canu_send(can_message_t *msg);

#endif
