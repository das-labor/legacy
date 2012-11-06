#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "config.h"
#include "light.h"
#include "switch.h"
#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

void init(void)
{
	DDRB |= _BV(PB0); // LED out
	
	//init light
	light_init();
	
	//switch init
	init_switch();

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
	
	//tell status
	send_status();

	//the main loop continuously handles can messages
	while (1)
	{	
		can_handler();
		switch_handler();
		wdt_reset();
	}
}

