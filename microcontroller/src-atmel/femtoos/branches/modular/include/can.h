
#ifndef CAN_H
#define CAN_H

/*****************************************************************************
 * Simple CAN Library
 *
 * #define CAN_INTERRUPT 1	//set this to enable interrupt driven 
 *                                and buffering version
 * #define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
 * #define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt
 */

/*****************************************************************************
 * Types
 */
 
#include "femtoos_code.h"

#ifdef CAN_THREAD
typedef Tuint08 can_addr;
typedef Tuint08 can_port;

typedef struct {
	can_addr      addr_src;
	can_addr      addr_dst;
	can_port      port_src;
	can_port      port_dst;
	Tuint08 dlc;
	Tuint08 data[8];
} can_message;

typedef enum { normal, mode_sleep, loopback, listenonly, config } can_mode_t ;

/*****************************************************************************
 * Management
 */

void can_init();
void can_setfilter();
void can_setmode(can_mode_t);
void can_setled(Tuint08 led, Tuint08 state);


/*****************************************************************************
 * Sending
 */

can_message * can_buffer_get();
void can_transmit(can_message *msg);


/*****************************************************************************
 * Receiving
 */

can_message *can_get();
can_message *can_get_nb();
void can_free(can_message * msg);
void can_handler();


#endif // CAN_THREAD
#endif // CAN_H
