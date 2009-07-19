#ifndef MCP2551_H
#define MCP2551_H

#include <stdint.h>

#include "../config.h"
#include "can.h"

/*****************************************************************************
 * Simple CAN Library
 *
 * #define CAN_INTERRUPT 1	//set this to enable interrupt driven 
 *                                and buffering version
 * #define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
 * #define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt
 */

/*****************************************************************************
 * Interrupt settings
 */
#ifdef CAN_INTERRUPT
  #ifndef MCP_INT_VEC
    #define MCP_INT_VEC INT0_vect
  #endif
  #ifndef MCP_INT_MASK
    #define MCP_INT_MASK INT0
  #endif
  #ifndef MCP_INT_FLAG
    #define MCP_INT_FLAG ISC01
  #endif
#endif

/*****************************************************************************
 * Management
 */

void mcp_init();
void can_setfilter();
void can_setmode(can_mode_t);
void can_setled(unsigned char led, unsigned char state);

/*****************************************************************************
 * Sending
 */

can_message_raw * can_buffer_get_raw();
void can_transmit_raw(can_message_raw *msg);

/*****************************************************************************
 * Receiving
 */

can_message_raw *can_get_raw();
can_message_raw *can_get_raw_nb();
void can_free_raw(can_message_raw * msg);


#endif
