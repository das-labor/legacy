#ifndef CAN_H
#define CAN_H

#include "config.h"

/*****************************************************************************
 * Types
 */
 
typedef unsigned char can_addr;
typedef unsigned char can_port;
typedef uint16_t can_channel_t;
typedef uint8_t can_subchannel_t;

typedef struct {
	uint32_t id;
	uint8_t dlc;
	uint8_t data[8];
} can_message_raw;

typedef struct {
	can_addr      addr_src;
	can_addr      addr_dst;
	can_port      port_src;
	can_port      port_dst;
	unsigned char dlc;
	unsigned char data[8];
} can_message;

typedef enum { normal, mode_sleep, loopback, listenonly, config } can_mode_t ;

/*****************************************************************************
 * Global variables
 */
#ifdef CAN_HANDLEERROR
  extern unsigned char can_error;
#endif


#endif
