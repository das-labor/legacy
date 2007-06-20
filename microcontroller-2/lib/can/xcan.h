#ifndef CAN_H
#define CAN_H

/*****************************************************************************
 * Simple CAN Library for use with avrx
 *
 */

/*****************************************************************************
 * Types
 */

typedef uint8_t can_addr_t;
typedef uint16_t can_channel_t;
typedef uint8_t can_subchannel_t;

typedef struct{
	can_addr_t       addr_src;
	can_addr_t       addr_dst;
	can_channel_t	 channel;
	can_subchannel_t subchannel;
	unsigned char dlc;
	unsigned char data[8];
}can_message_t;

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
