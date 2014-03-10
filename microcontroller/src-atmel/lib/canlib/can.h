#ifndef _CAN_H
#define _CAN_H

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
 
#include <stdint.h>
#include "../config.h"

#ifdef CAN_RAW
	typedef uint16_t can_channel_t;
	typedef uint8_t can_subchannel_t;

	typedef struct
	{
		uint32_t id;
		uint8_t dlc;
		uint8_t data[8];
	} can_message;
#else
	typedef uint8_t can_addr;
	typedef uint8_t can_port;
	typedef
#ifdef OPTIMISED_LAP
		union {
		uint8_t can_msg_array[13];
#endif // OPTIMISED_LAP
		struct {
			can_port port_src;
			can_port port_dst;
			can_addr addr_src;
			can_addr addr_dst;

			uint8_t dlc;
			uint8_t data[8];
#ifdef OPTIMISED_LAP
		};
#endif // OPTIMISED_LAP
	} can_message;
#endif


/*****************************************************************************
 * Interrupt settings
 */
 
/*
  left as comment for reference only.
  if you are now getting these constants as undefined, please set them in your config.h !
  or use the even newer Makros like this in your config file:
  #define   ENABLE_CAN_INT()    GIMSK |= _BV(INT0)
  #define   DISABLE_CAN_INT()   GIMSK &= ~_BV(INT0)
  #define   SETUP_CAN_INT()     MCUCR |= _BV(ISC01)
  
  The interrupt is supposed to use falling edge trigger.
  
  here are the previous default values:
  
  #ifndef MCP_INT_VEC
    #define MCP_INT_VEC INT0_vect
  #endif
  #ifndef MCP_INT_MASK
    #define MCP_INT_MASK INT0
  #endif
  #ifndef MCP_INT_CTL
    #define MCP_INT_CTL ISC01
  #endif

*/

/*****************************************************************************
 * Global variables
 */
#ifdef CAN_HANDLEERROR
  extern uint8_t can_error;
#endif

/*****************************************************************************
 * Management
 */

extern void can_init(void);
extern void mcp_setled(uint8_t led, uint8_t state);
extern void mcp_setmode(uint8_t mode);

/*****************************************************************************
 * Sending
 */

extern can_message *can_buffer_get(void);
extern void can_transmit(can_message *msg);


/*****************************************************************************
 * Receiving
 */

extern can_message *can_get(void);
extern can_message *can_get_nb(void);
extern void can_free(can_message *msg);

#endif // _CAN_H
