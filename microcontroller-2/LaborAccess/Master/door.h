#ifndef DOOR_H
#define DOOR_H

#include "avrx.h"
AVRX_EXTERNTASK(door);

#define DOOR_PORT PORTC
#define DOOR_DDR  DDRC

#define DOOR_PIN0 (_BV(PC2))
#define DOOR_PIN1 (_BV(PC3))
#define DOOR_PIN2 (_BV(PC4))
#define DOOR_PIN3 (_BV(PC5))

// counterclockwise
#define DOOR_DIR_LEFT  0x00

// clockwise
#define DOOR_DIR_RIGHT 0x01

/* time in ms to wait between steps */
#define DOOR_WAITCYCLES 10

void door_turn (uint8_t in_direction);
#endif
