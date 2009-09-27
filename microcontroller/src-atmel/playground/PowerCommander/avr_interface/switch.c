#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"

#include "twi_master/twi_master.h"

#define PIN_SWITCH PIND
#define BIT_SWITCH PD0

TimerControlBlock switchtimer;


AVRX_GCC_TASKDEF(switchtask, 60, 7)
{
	can_message_t msg = {0x02, 0x00, 0x00, 0x00};
	msg.dlc = 2;
	uint8_t stat_haupt = 0;

  	while (1)
  	{
		if (!(PINA & _BV(PA0)) && stat_haupt == 1)
		{
			msg.data[0] = 0x23;
			msg.data[1] = 0x42; //NULLBYTE
			can_put(&msg);
			
		    if (!TWIM_Start(15, TWIM_WRITE))
            {
            	TWIM_Stop();
            }
            else
            {
                    TWIM_Write(0x02);	//x03|0x00|0x00|0x00 aus
                    TWIM_Write(0x00);
                    TWIM_Write(0x00);
                    TWIM_Write(0x00);
                    TWIM_Stop();
            }
            stat_haupt = 0;
            AvrXDelay(&switchtimer, 1000);
		}
		if ((PINA & _BV(PA0)) && stat_haupt == 0)
		{
			msg.data[0] = 0x42;
			msg.data[1] = 0x23; //NULLBYTE
			can_put(&msg);
			
		    if (!TWIM_Start(15, TWIM_WRITE))
            {
            	TWIM_Stop();
            }
            else
            {
                    TWIM_Write(0x02);	//x03|0x00|0x01|0x00 an
                    TWIM_Write(0x00);
                    TWIM_Write(0x01);
                    TWIM_Write(0x00);
                    TWIM_Stop();
            }
            stat_haupt = 1;
            AvrXDelay(&switchtimer, 1000);
		}
  	}
}
