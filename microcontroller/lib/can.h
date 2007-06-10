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

typedef unsigned char can_addr;
typedef unsigned char can_port;

typedef struct{
	can_addr      addr_src;
	can_addr      addr_dst;
	can_port      port_src;
	can_port      port_dst;
	unsigned char dlc;
	unsigned char data[8];
}can_message;

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

can_message * can_buffer_get();
void can_transmit( can_message *msg );


/*****************************************************************************
 * Receiving
 */

can_message *can_get();
can_message *can_get_nb();

// this is only needed for Interrupt driven Version
#ifndef CAN_INTERRUPT
// #  define can_free(m)
  void can_free(can_message * msg);
#else
  void can_free(can_message * msg);
#endif



#endif
