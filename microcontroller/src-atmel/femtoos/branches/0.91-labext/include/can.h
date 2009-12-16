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
 
#ifdef USE_FEMTOOS
#include "femtoos_code.h"
#else
#include <stdint.h>
#include "config.h"
#endif

typedef Tuint08 can_addr;
typedef Tuint08 can_port;
typedef Tuint16 can_channel_t;
typedef Tuint08 can_subchannel_t;

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
  extern Tuint08 can_error;
#endif


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

/*****************************************************************************
 * Sending
 */

//can_message_raw * can_buffer_get_raw();
//void can_transmit_raw(can_message_raw *msg);


/*****************************************************************************
 * Receiving
 */

//can_message_raw *can_get_raw();
//can_message_raw *can_get_raw_nb();
//void can_free_raw(can_message_raw * msg);



#endif // CAN_H
