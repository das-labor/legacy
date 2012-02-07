#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "rfm12_config.h"
#include "rfm12.h"

#define DIR_STOP 0
#define DIR_FW 1
#define DIR_RW 2

#define PORT_EN PORTD
#define PIN_EN (_BV(PD7))
#define DDR_EN DDRD

#define DDR_M DDRC
#define PORT_M PORTC
#define PIN_FW (_BV(PC0))
#define PIN_RW (_BV(PC1))

#define MOTOR_FW() PORT_M |= PIN_FW; PORT_M &= ~(PIN_RW)
#define MOTOR_RW() PORT_M |= PIN_RW; PORT_M &= ~(PIN_FW)
#define MOTOR_ON() PORT_EN |= PIN_EN
#define MOTOR_OFF() PORT_EN &= ~(PIN_EN)


/* Lights: PD3: inside, PD6: forward, PD4: reverse */
#define LIGHTS_RW() PORTD &= ~(_BV(PD6)); PORTD |= (_BV(PD4))
#define LIGHTS_FW() PORTD &= ~(_BV(PD4)); PORTD |= (_BV(PD6))

volatile static uint8_t speed = 150;
volatile static uint8_t dir = 0;

ISR (TIMER0_OVF_vect)
{
	static uint8_t toggle = 0;
	uint8_t direction;

	if (dir == 0)
	{
		direction = DIR_FW;
		LIGHTS_FW();
	}
	else if (dir != 0)
	{
		direction = DIR_RW;
		LIGHTS_RW();
	}
	else
	{
		direction = DIR_STOP;
	}

	switch (direction)
	{
		case DIR_FW:
			MOTOR_FW();
		break;

		case DIR_RW:
			MOTOR_RW();
		break;

		case DIR_STOP:
		default:
			MOTOR_OFF();
		break;
	}
	
	toggle ^= 0x01;

	if (toggle)
	{
		MOTOR_ON();
		TCNT0 = 0xff - speed;
	} else
	{
		MOTOR_OFF();
		TCNT0 = speed;
	}
}


int main (void)
{
	uint8_t i = 0;
	uint8_t laststate = 0, ctrlstate, action = 0;
	uint8_t *rxbuf;
	unsigned char greeting[] = "LOK 2 ALIVE\n";
	
#if 0
	TCCR0 = _BV(CS01); /* clk/64 */
	TIMSK |= _BV(TOIE0);
	TCNT0 = 40;
#endif


	DDRB &= ~(_BV(PB2));
	DDRB |= _BV(PD3) | _BV(PD4) | _BV(PD5) | _BV(PD6);
	PORTB |= _BV(PB2);
	PORTD |= _BV(PD3) | _BV(PD5) | _BV(PD6);

	
	DDR_EN |= PIN_EN;
	DDR_M  |= (PIN_FW | PIN_RW);
	PORT_EN &= ~(PIN_EN);

	MOTOR_FW();
	
	TCCR2 = _BV(WGM20) | _BV(COM21) | _BV(CS20) ; /* p-corr. pwm, clear on up cnt, clk/64 */
//	TCCR2 = _BV(WGM20) | _BV(COM21); /* p-corr. pwm, clear on up cnt, clk/8 */

	for (i=0;i<4;i++)
		_delay_ms(250);
	
	rfm12_init();
	rfm12_init();
	sei();

	rfm12_tx (sizeof(greeting), 0, greeting);

	
	while (42)
	{
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			greeting[0] = '%';
                        if(rfm12_rx_len() >= 4)
			{
			#if 0
				if (!(rxbuf[0] == 0x00 && rxbuf[1] == 0x01) && !(rxbuf[0] == 0xff && rxbuf[1] == 0xff) && 0)
				{
					rfm12_rx_clear();
					continue;
				}
			#endif
				rxbuf = rfm12_rx_buffer();
                                ctrlstate = rxbuf[2];

                                switch (ctrlstate)
				{
					case '+': /* up */
						speed += 5;
					break;
					case '-': /* down */
						speed -= 5;
					break;
					
					case '0': /* buttons */
						speed = 0;
					
					default:
						//speed += 5;
						OCR2 = speed;
				}

				laststate = ctrlstate;
			}
			rfm12_rx_clear();

			rfm12_tx (sizeof(greeting), 0, greeting);
		}

		rfm12_tick();
	}
}
