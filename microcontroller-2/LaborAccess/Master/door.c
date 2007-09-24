#include <avr/io.h>
#include "avrx.h"

#include "door.h"

/* @brief actually turns step motor for door */

void door_init ()
{
	DOOR_DDR |= (DOOR_PIN0 | DOOR_PIN1 | DOOR_PIN2 | DOOR_PIN3);
}

void door_turn (uint8_t in_direction)
{
	static uint8_t mystate = 0x01, oldstate;
	
	oldstate = mystate;

	if (in_direction == DOOR_DIR_LEFT)
	{
		mystate++;

	} else if (in_direction == DOOR_DIR_RIGHT)
	{
		mystate--;
	}

	switch (mystate & 0x03)
	{
		case 0x00:
			DOOR_PORT &= ~(DOOR_PIN3 | DOOR_PIN1 | DOOR_PIN2);
			DOOR_PORT |= DOOR_PIN0;
		break;
		case 0x01:
			DOOR_PORT &= ~(DOOR_PIN3 | DOOR_PIN0 | DOOR_PIN2);
			DOOR_PORT |= DOOR_PIN1;
		break;
		case 0x02:
			DOOR_PORT &= ~(DOOR_PIN3 | DOOR_PIN1 | DOOR_PIN0);
			DOOR_PORT |= DOOR_PIN2;
		break;
		case 0x03:
			DOOR_PORT &= ~(DOOR_PIN0 | DOOR_PIN1 | DOOR_PIN2);
			DOOR_PORT |= DOOR_PIN3;
		break;
	}

}


AVRX_GCC_TASKDEF(door, 200, 5)
{
	TimerControlBlock doortimer;
	door_init();
	while (1)
	{
		door_turn (DOOR_DIR_RIGHT);
		AvrXDelay (&doortimer, DOOR_WAITCYCLES);
	}
}
