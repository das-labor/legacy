#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xlap.h"
#include "xcan.h"


TimerControlBlock switchtimer;

AVRX_GCC_TASKDEF(licht_task, 60, 6)
{
    can_message_t msg = {0x23, 0x02, 0x01, 0x01};


    msg.dlc = 4;
    msg.data[0] = 0x00;
    msg.data[1] = 0x00;
    msg.data[2] = 0x02;
    msg.data[3] = 0x00;
    AvrXDelay(&switchtimer, 500);
    can_put(&msg);

    while (1)
    {
        if (!(PINB & _BV(PB1)))
        {
            PORTC |= _BV(PC5);
            PORTC |= _BV(PC4);
            PORTC |= _BV(PC3);

            AvrXDelay(&switchtimer, 500);

            PORTC &= ~(_BV(PC5)); //LED5 AUS
            PORTC &= ~(_BV(PC4)); //LED5 AUS
            PORTC &= ~(_BV(PC3)); //LED5 AUS

            msg.data[0] = 0x00;
            msg.data[1] = 0x00;

			if (klight_stat == 0)
			{
				msg.data[2] = 0x01;
				PORTC |= _BV(PC3);
				PORTC &= ~_BV(PC4);
				klight_stat = 1;
			}
			else
			{
				msg.data[2] = 0x00;
				PORTC |= _BV(PC4);
				PORTC &= ~_BV(PC3);
				klight_stat = 0;
			}
			can_put(&msg);
			AvrXDelay(&switchtimer, 3000);
        }
		AvrXDelay(&switchtimer, 1);
    }
}

