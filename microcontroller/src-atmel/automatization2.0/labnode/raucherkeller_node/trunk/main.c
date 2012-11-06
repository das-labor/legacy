#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "config.h"
#include "light.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"


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
	TCCR0 = _BV(CS02) | _BV(CS00);	//1:1024 prescaler
	TCNT0 = 0;
	time_cnt=countdown;
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

/*	TCCR0 = 0; //stop counting
	//TCCR0 = _BV(CS02)|_BV(CS00);	//1:1024 prescaler
	TCNT0 =0;
	TIMSK |= _BV(TOIE0);	//Overflow Interrupt enable
	OCR0 = 0;
	time_cnt = 0;*/
	
	//init light
	light_init();

	//initialize spi port
	spi_init();

	//initialize can communication
	can_init();

	read_can_addr();
	wdt_enable(WDTO_250MS); // 250 ms
}

extern uint8_t channels_active[NUM_CHANNELS];
int main(void)
{
	//system initialization
	init();

	//allow interrupts
	sei();

	//the main loop continuously handles can messages
	uint8_t test = 0, i;
	while (1)
	{	
		can_handler();
		wdt_reset();
		/*_delay_ms(10);
		if(test++ == 0)
			for(i = 0; i < NUM_CHANNELS; i++)
				enable_channel(i, ~channels_active[i]);*/
	}
}

