#ifndef CAN_H
#define CAN_H

#include "config.h"

/*****************************************************************************
 * Simple CAN Library for use with avrx
 *
 */

/*****************************************************************************
 * Types
 */

typedef unsigned char can_addr;
typedef unsigned char can_port;

typedef struct
{
	can_addr      addr_src;
	can_addr      addr_dst;
	can_port      port_src;
	can_port      port_dst;
	unsigned char dlc;
	unsigned char data[8];
} can_message_t;

typedef struct
{
	uint8_t in;
	uint8_t out;
	uint8_t size;
	can_message_t buf[10];
} can_fifo_t;

typedef enum { normal, mode_sleep, loopback, listenonly, config } can_mode_t;


/*****************************************************************************
 * Management
 */

void can_init();
void can_setfilter();
void can_setmode(can_mode_t);
void can_setled(unsigned char led, unsigned char state);

/*****************************************************************************
 * Global Message Buffer
 */

extern can_message_t rx_msg;

/*****************************************************************************
 * Receiving
 */

void can_get();

/*****************************************************************************
 * Sending
 */

uint16_t can_put(can_message_t *msg);

#endif
