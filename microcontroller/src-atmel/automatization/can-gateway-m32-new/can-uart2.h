#ifndef CANUART_H
#define CANUART_H

/*****************************************************************************
 * Send encapsulated CAN packets via UART
 *
 * 
 */

#include "config.h"
#include "can.h"
#include "cantun.h"

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
cantun_msg_t *canu_buffer_get();
void canu_free(cantun_msg_t *);


/*****************************************************************************
 * rcv
 */

/* nonblocking read from uart -- returns 0 if no complete msg arrived */
cantun_msg_t *canu_get_nb();

/* blocking read from uart */
cantun_msg_t *canu_get();


/*****************************************************************************
 * transmit
 */
void canu_transmit(cantun_msg_t *msg);

#endif
