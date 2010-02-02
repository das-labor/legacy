#include <stdint.h>

#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H

#define DIR_TX 1
#define DIR_RX 0

typedef struct
{
	uint8_t  dir;
	uint32_t freq; 	 /* frequency in HZ */
	uint16_t bwidth; /* bandwidth */
	uint32_t bps;    /* bits per second */
	uint32_t len;    /* payload len */
	uint8_t  *data;  /* data ptr */
} tcp2air_t;

#endif
