#ifndef CAN_H
#define CAN_H

#include <avr/boot.h>

/*****************************************************************************
 * Simple CAN Library
 *
 * #define CAN_INTERRUPT 1	//set this to enable interrupt driven 
 *                                and buffering version
 * #define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
 * #define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt
 */

#include "../config.h"

/*****************************************************************************
 * Types
 */

typedef uint8_t can_addr;
typedef uint8_t can_port;

typedef struct {
	can_addr      addr_src;
	can_addr      addr_dst;
	can_port      port_src;
	can_port      port_dst;
	uint8_t dlc;
	uint8_t data[8];
} can_message;

typedef enum { NORMAL, MODE_SLEEP, LOOPBACK, LISTENONLY, CONFIG } can_mode_t;

extern can_message Rx_msg, Tx_msg;


/*****************************************************************************
 * MCP
 */

extern void mcp_write(uint8_t reg, uint8_t data) __attribute__ ((noinline)) BOOTLOADER_SECTION;

/*****************************************************************************
 * Management
 */

extern void can_init(void) BOOTLOADER_SECTION;


/*****************************************************************************
 * Sending
 */

extern void can_transmit(void) BOOTLOADER_SECTION;


/*****************************************************************************
 * Receiving
 */

extern uint8_t can_get_nb(void) BOOTLOADER_SECTION;



#endif
