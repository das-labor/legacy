#ifndef CAN_H
#define CAN_H

/*****************************************************************************
 * Simple CAN Library
 *
 * #define CAN_INTERRUPT 1	     // set this to enable interrupt driven
 *                               // and buffering version
 * #define CAN_RX_BUFFER_SIZE 2	 // only used for Interrupt
 * #define CAN_TX_BUFFER_SIZE 2	 // only used for Interrupt
 */

/*****************************************************************************
 * Types
 */

#include <stdint.h>

typedef unsigned char can_addr_t;
typedef unsigned char can_port_t;
typedef uint16_t can_channel_t;
typedef uint8_t can_subchannel_t;

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[8];
} can_message_raw;

typedef struct {
	can_addr_t addr_src;
	can_addr_t addr_dst;
	can_port_t port_src;
	can_port_t port_dst;
	unsigned char dlc;
	unsigned char data[8];
} can_message;

typedef struct {
	can_channel_t channel;
	can_subchannel_t subchannel;
	can_addr_t addr_src;
	can_addr_t addr_dst;
	uint8_t dlc;
	uint8_t data[8];
} can_message_v2;

typedef enum {
	normal, mode_sleep, loopback, listenonly, config
} can_mode_t;

/*****************************************************************************
 * Global variables
 */
#ifdef CAN_HANDLEERROR
extern unsigned char can_error;
#endif

/*****************************************************************************
 * Management
 */

void can_init();
void can_setfilter();
void can_setmode( can_mode_t);
void can_setled(unsigned char led, unsigned char state);

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

// this is only needed for Interrupt driven Version
#ifndef CAN_INTERRUPT
// #  define can_free(m)
void can_free(can_message * msg);
#else
void can_free(can_message * msg);
#endif

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

// this is only needed for Interrupt driven Version
#ifndef CAN_INTERRUPT
// #  define can_free(m)
void can_free_raw(can_message_raw * msg);
#else
void can_free_raw(can_message_raw * msg);
#endif

/*****************************************************************************
 * Sending
 */

void can_transmit_v2(can_message_v2 *msg);

/*****************************************************************************
 * Receiving
 */

can_message_v2 *can_get_v2_nb();

void can_free_v2(can_message_v2 *msg);

#endif
