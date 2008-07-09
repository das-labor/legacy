#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "motor.h"
#include "../lib/rfm12/rfm12.h"

int main (void)
{
	uint32_t ticker = 1;
	uint8_t alivesign[] = "sixvolts alive\r\n";
	uint8_t *buf, rxlen;

	motor_init();

//	motor_ctrl_direct (RB_LEFT | RB_DIR_FW);
//	motor_ctrl_direct (RB_RIGHT | RB_DIR_FW);
	
	rfm12_init();
	sei();

	while (42)
	{
		if (ticker > 0xAffff)
		{
			rfm12_tx (sizeof(alivesign), 0, alivesign);
			ticker = 0;
		}

		
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

			buf = rfm12_rx_buffer();
			rfm12_rx_clear();
			
			if (!rfm12_rx_len()) continue;

			switch (buf[0])
			{
				/* motor control */
				case 'w':
					motor_ctrl_direct (RB_LEFT | RB_DIR_FW);
					motor_ctrl_direct (RB_RIGHT | RB_DIR_FW);
					rfm12_tx (1, 0, buf);
				break;
				case 'y':
					motor_ctrl_direct (RB_LEFT | RB_DIR_RW);
					motor_ctrl_direct (RB_RIGHT | RB_DIR_RW);
					rfm12_tx (1, 0, buf);
				break;
				case 'a':
					motor_ctrl_direct (RB_LEFT | RB_DIR_RW);
					motor_ctrl_direct (RB_RIGHT | RB_DIR_FW);
					rfm12_tx (1, 0, buf);
				break;
				case 's':
					motor_ctrl_direct (RB_LEFT | RB_DIR_FW);
					motor_ctrl_direct (RB_RIGHT | RB_DIR_RW);
					rfm12_tx (1, 0, buf);
				break;
				case ' ':
					motor_ctrl_direct (RB_LEFT);
					motor_ctrl_direct (RB_RIGHT);
					rfm12_tx (1, 0, buf);
				break;
			}
			ticker = 0;
		} else
		{
			ticker++;
		}
		rfm12_tick();
	}
}
