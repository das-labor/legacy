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

typedef struct{
	can_addr      addr_src;
	can_addr      addr_dst;
	can_port      port_src;
	can_port      port_dst;
	uint8_t dlc;
	uint8_t data[8];
}can_message;

typedef enum { normal, mode_sleep, loopback, listenonly, config } can_mode_t;

extern can_message Rx_msg, Tx_msg;


/*****************************************************************************
 * MCP
 */

void mcp_write(uint8_t reg, uint8_t data) BOOTLOADER_SECTION;

/*****************************************************************************
 * Management
 */

void can_init() BOOTLOADER_SECTION;
void can_setmode(can_mode_t) BOOTLOADER_SECTION;
#define BFPCTRL 0x0C


/*****************************************************************************
 * Sending
 */

can_message * can_buffer_get()BOOTLOADER_SECTION;
void can_transmit()BOOTLOADER_SECTION;


/*****************************************************************************
 * Receiving
 */

uint8_t can_get_nb()BOOTLOADER_SECTION;

// this is only needed for Interrupt driven Version
#ifndef CAN_INTERRUPT
#  define can_free(m)
#else
  void can_free(can_message * msg);
#endif




#endif
