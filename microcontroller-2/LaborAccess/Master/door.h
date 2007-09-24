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

#define DOOR_ADCPORT PORTA
#define DOOR_ADCDDR DDRA
#define DOOR_ADCPIN (_BV(PA1))
#define DOOR_SENSORTRESHOLD 850

// counterclockwise
#define DOOR_DIR_LEFT  0x10

// clockwise
#define DOOR_DIR_RIGHT 0x20

/* time in ms to wait between steps */
#define DOOR_WAITCYCLES 2

#define DOOR_STATE_MESSED     0x00
#define DOOR_STATE_CLOSED     0x01
#define DOOR_STATE_OPEN       0x02
#define DOOR_STATE_TURN_RIGHT 0x04
#define DOOR_STATE_TURN_LEFT  0x05
#define DOOR_STATE_INIT DOOR_STATE_MESSED

/* time in ms to wait before timing out the optical sensor */
#define DOOR_OPTOTIMEOUT 2200

/* ticks for mark-landing (should be greater than actual amount of ticks)  */
#define DOOR_SPINTICKS 150

/* total number of ticks before door routine stops
 * regardless of state and current position.
 * needed in cases where the mark is over the sensor and blocking or in other
 * (yet unknown) cases
 * */
#define DOOR_MAXTICKS 2600


void door_turn (uint8_t in_direction);
#endif
