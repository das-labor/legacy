#ifndef __LIBLAP_H
#define __LIBLAP_H

#include <stdint.h>

/*****************************************************************************
 * Simple CAN Library
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


int can_open(char *host);



#endif
