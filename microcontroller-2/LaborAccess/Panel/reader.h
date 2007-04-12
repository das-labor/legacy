
#include <avr/io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "types.h"

#include "config.h"

typedef struct{
	MessageControlBlock mcb;
	uint8_t state;
}ReaderMsg_t;

extern MessageQueue ReaderMsgInQueue;

extern ReaderMsg_t ReaderMsgOut;


#define DDR_POWER DDRA
#define PORT_POWER PORTA
#define BIT_POWER 2


#define CARD_POWER_ON() PORT_POWER &= ~(1<<BIT_POWER)
#define CARD_POWER_OFF() PORT_POWER |= (1<<BIT_POWER)


AVRX_EXTERNTASK(reader);
