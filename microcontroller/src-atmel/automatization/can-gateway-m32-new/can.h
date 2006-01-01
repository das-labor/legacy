#ifndef CAN_H
#define CAN_H

#include "config.h"

/*****************************************************************************
 * Simple CAN Library
 *
 * #define CAN_INTERRUPT 1	//set this to enable interrupt driven 
 *                                and buffering version
 * #define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
 * #define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt
 */

#include "config.h"

/*****************************************************************************
 * Types
 */

typedef struct{
	uint32_t id;
	uint8_t  rtr;
	uint8_t  dlc;
	uint8_t  data[8];
}can_message_t;

typedef enum { normal, mode_sleep, loopback, listenonly, config } can_mode_t ;


/*****************************************************************************
 * Global variables
 */
#ifdef CAN_HANDLEERROR
  unsigned char can_error;
#endif


/*****************************************************************************
 * Management
 */

void can_init();
void can_setfilter();
void can_setmode(can_mode_t);
void can_setled(unsigned char led, unsigned char state);

/*****************************************************************************
 * Sending
 */

can_message_t * can_buffer_get();
void can_transmit( can_message_t *msg );


/*****************************************************************************
 * Receiving
 */

can_message_t *can_get();
can_message_t *can_get_nb();

void can_free(can_message_t * msg);


#endif
