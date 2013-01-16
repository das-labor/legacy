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

typedef uint8_t can_addr;
typedef uint8_t can_port;
typedef uint16_t can_channel_t;
typedef uint8_t can_subchannel_t;

#ifdef CAN_RAW

	typedef struct
	{
		uint32_t id;
		uint8_t dlc;
		uint8_t data[8];
	} can_message;

#else

	typedef struct {
		can_addr      addr_src;
		can_addr      addr_dst;
		can_port      port_src;
		can_port      port_dst;
		uint8_t dlc;
		uint8_t data[8];
	} can_message;

#endif

typedef enum { NORMAL, MODE_SLEEP, LOOPBACK, LISTENONLY, CONFIG } can_mode_t ;


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
 * Global variables
 */
#ifdef CAN_HANDLEERROR
  extern uint8_t can_error;
#endif


/*****************************************************************************
 * Management
 */

void can_init();
void can_setfilter();
void can_setmode(can_mode_t);
void can_setled(uint8_t led, uint8_t state);


/*****************************************************************************
 * Sending
 */

can_message *can_buffer_get();
void can_transmit(can_message *msg);


/*****************************************************************************
 * Receiving
 */

can_message *can_get();
can_message *can_get_nb();
void can_free(can_message *msg);

#endif // _CAN_H
