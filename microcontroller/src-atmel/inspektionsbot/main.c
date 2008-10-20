#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "motor.h"
#include "rfm12.h"
#include "eyes.h"

#include <util/delay.h>



ISR (TIMER0_OVF_vect)
{
	motor_pwm();
}


void timer_init ( void )
{
	TCCR0 |= ( _BV(CS00) );
	TIMSK |= _BV(TOIE0);
}

int main (void)
{
	uint32_t ticker = 1;
	uint8_t alivesign[] = "INSPEKTIOON!\r\n";
	uint8_t *buf, rxlen;
	uint8_t txbuf[10];

	DDRC |= _BV(PC2);

	motor_init();
	timer_init();


//	motor_ctrl (RB_LEFT | RB_DIR_FW);
//	motor_ctrl (RB_RIGHT | RB_DIR_FW);
	
	_delay_ms (250);

	rfm12_init();
	sei();
	
	_delay_ms (250);

	rfm12_tx (sizeof(alivesign), 0, alivesign);

	while (42)
	{
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{

			buf = rfm12_rx_buffer();
			if (rfm12_rx_type() != 0x69)
			{
				rfm12_rx_clear();
				continue;
			}
			
			rfm12_rx_clear();

			if (!rfm12_rx_len()) continue;

			if (buf[0] & 0x04)
			{
				motor_ctrl (RB_LEFT | RB_DIR_RW);
				motor_ctrl (RB_RIGHT | RB_DIR_FW);
			}
			
			if (buf[0] & 0x01)
			{
				motor_ctrl (RB_LEFT | RB_DIR_FW);
				motor_ctrl (RB_RIGHT | RB_DIR_FW);
			}
			
			if (buf[0] & 0x08)
			{
				motor_ctrl (RB_LEFT | RB_DIR_FW);
				motor_ctrl (RB_RIGHT | RB_DIR_RW);
			}
			
			if (buf[0] & 0x02)
			{
				motor_ctrl (RB_LEFT | RB_DIR_RW);
				motor_ctrl (RB_RIGHT | RB_DIR_RW);
			}
			
			if (buf[0] == 0x00)
			{
				motor_init();
			}
			
			if (buf[0] & 0x10)
			{
				PORTC ^= _BV(PC2);
			}

		}
		
		eyes_tick();
		rfm12_tick();

		if (ticker == 0x5fff)
		{
			txbuf[0] = 0x4D;
			txbuf[1] = 0xC0;
			txbuf[2] = (uint8_t) (get_adcval(0) >> 8);
			txbuf[3] = (uint8_t) (get_adcval(0));
			txbuf[4] = (uint8_t) (get_adcval(1) >> 8);
			txbuf[5] = (uint8_t) (get_adcval(1));
			rfm12_tx (8, 0, txbuf);
			ticker = 0;
		}
#ifdef KRUDDELCODE
		if (get_adcval (0) > 0xff || get_adcval(1) > 0xff) /* right */
		{
			uint8_t nakka[2];

			nakka[0] = motor_getstate (0);
			nakka[1] = motor_getstate (1);

			motor_ctrl (RB_LEFT | RB_DIR_RW);
			motor_ctrl (RB_RIGHT | RB_DIR_FW);
			
			_delay_ms(1);
			
			motor_init();
		}
#endif

		ticker++;
	}
}
