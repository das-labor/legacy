#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "config.h"

#include "dimmer.h"

#include "can/can.h"
#include "netvar/can_handler.h"
#include "can/spi.h"
#include "netvar/netvar.h"


static uint16_t time_cnt;

//simple timer counting down
ISR(TIMER0_OVF_vect) {
	if(time_cnt)
			time_cnt--;
	else
			TCCR0=0;	//stop counting
}

void start_counter(uint16_t countdown)
{
	TCCR0 = _BV(CS02)|_BV(CS00);	//1:1024 prescaler
	TCNT0 = 0;
	time_cnt = countdown;
}

uint8_t get_counter_status(void)
{
		if(time_cnt)
			return 1;
		return 0;
}

void init(void)
{
	DDRB |= _BV(PB0); // LED out
	//DDRD |= _BV(PD5); // EVG: 0-10V

	TCCR0 = 0; //stop counting
	//TCCR0 = _BV(CS02)|_BV(CS00);	//1:1024 prescaler
	TCNT0 =0;
	TIMSK |= _BV(TOIE0);	//Overflow Interrupt enable
	OCR0 = 0;
	time_cnt = 0;

	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();

	read_can_addr();
	wdt_enable(WDTO_250MS); // 250 ms
}

int main(void)
{
	//system initialization
	init();

/*	
	DDRA |= (1<<PA4)|(1<<PA5);
	DDRC |= (1<<PC4)|(1<<PC5);
	PORTA |= (1<<PA4)|(1<<PA5);
	PORTC |= (1<<PC4)|(1<<PC5);
	*/
	dimmer_init();

	//enable all channels, set maximum brightness
	set_dimmer(0, 127);
	set_dimmer(1, 127);
	set_dimmer(2, 127);
	set_dimmer(3, 127); //neon tube is inverted


	//allow interrupts
	sei();

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		netvar_handle_events();
		wdt_reset();
	}
}

