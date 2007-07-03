#ifndef CAN_H
#define CAN_H

#include "config.h"
#include <stdint.h>

/*****************************************************************************
 * Simple CAN Library
 *
 */

/*****************************************************************************
 * Types
 */

typedef struct{
	uint8_t		id[4];
	uint8_t		dlc;
	uint8_t		data[8];
} can_message_t;

typedef enum { normal, mode_sleep, loopback, listenonly, config } CanMode;


#endif



