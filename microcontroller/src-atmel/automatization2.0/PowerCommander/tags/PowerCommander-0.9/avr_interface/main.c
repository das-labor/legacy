/* -*- Mode: C; tab-width: 2 -*- */


#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "twi_master/twi_master.h"
#include "can/can.h"
#include "can/can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

#include "../include/PowerCommander.h"

#include "switch.h"


static uint16_t tickscounter;
static uint16_t tastercounter_vortrag;
static uint8_t tastercounter_lounge;
static uint8_t outdata[4];


ISR(TIMER1_OVF_vect)
{
	
	//	andere interrupts aus!
	
	cli();
	 
		// alle 9.8 ticks sollte sollte was vom schalter kommen
		// also sagen wir, dass wenn sich nach 32ticks nichts am schalter
		// getan hat, dann wurde er los gelassen. Wir koennen den 
		// Counter fuer die eingaben auf null setzen

	if ((tickscounter & 0x001F) == 0) // alle 32 ticks ... 0.032 sekunden
	{
		if (!(PINB & _BV(PB2)))
		{
			tastercounter_vortrag++;
		}
		if (!(PIND & _BV(PD3)))
		{
			tastercounter_lounge++;
		}
		if (tastercounter_vortrag != 0)
		{
			if (PINB & _BV(PB2))
			{
						outdata[0]=C_VIRT;
						outdata[1]=VIRT_VORTRAG;
						outdata[2]=F_SW_TOGGLE;
						outdata[3]=0x00;
						twi_send(outdata);
						tastercounter_vortrag = 0;
			}
		}
		if (tastercounter_lounge != 0)
		{
			if (PIND & _BV(PD3))
			{
						outdata[0]=C_SW;
						outdata[1]=SWL_LOUNGE;
						outdata[2]=F_SW_TOGGLE;
						outdata[3]=0x00;
						twi_send(outdata);
						tastercounter_lounge = 0;
			}
		}
	}
	 
	// ueberlaeufe sind ok!
	
	tickscounter++;
	
	// und alle interrupts wieder auf go!
	
	sei();
}


void init(void)
{

//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)
	
	
	DDRA |= _BV(PA2) | _BV(PA3) | _BV(PA4); // Status LED G R B
	DDRA &= ~(_BV(PA0) | _BV(PA1)); // Eingänge HS, rcd

	DDRB &= ~_BV(PB2); // Eingang Lounge Taster
	DDRD &= ~_BV(PD3); // Eingang Vortrag Taster
	
	// wird auf dem dev board gebraucht
//	PORTB |= _BV(PB2);
//	PORTD |= _BV(PD3);

/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init(100000))
	{
		while (1);
	}

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	
	read_can_addr();
	TCCR1A |= _BV(WGM10) ; // FastPWM
	TCCR1B |= _BV(WGM12) | _BV(CS11) | _BV(CS11); // FastPWM bit 2, clk/64

	TCNT1 = 0;   // pwm timer clear
	OCR1A = 0;   // pwm timer compare target
	OCR1B = 0;   // pwm timer compare target
	
	TIMSK |= _BV(TOIE1);							// Enable Timer1 Overflow Interrupt
	
	//turn on interrupts
	sei();
}
	 
int main(void)
{

	//system initialization
	init();

	//the main loop continuously handles can messages
	while (1)
	{
		PORTA &= ~_BV(PA4);
		can_handler();
		switch_handler();
	}
	return 1;
};
