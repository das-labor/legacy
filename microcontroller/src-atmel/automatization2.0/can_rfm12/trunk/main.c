#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"
#include "rfm12_config.h"
#include "rfm12.h"

void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)
	
	//TCCR0B = _BV(CS01) | _BV(CS00); /* clk / 64 */
	//TIMSK0 = _BV(TOIE0);

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	
	read_can_addr();

	rfm12_init();

	//turn on interrupts
	sei();
	wdt_enable(WDTO_250MS);
}

int main(void)
{
	//system initialization
	init();
	uint8_t bla = 0;
	uint8_t *bufcontents;

	//the main loop continuously handles can messages
	while (1)
	{
		can_handler();
		rfm12_tick();
		rfm12_poll();
		if (rfm12_rx_status() == STATUS_COMPLETE)
		{
			bufcontents = rfm12_rx_buffer();
			switch (rfm12_rx_type())
			{
				case 0x23:
					can_send(bufcontents);
					break;
			}
			if (!bla)
			{
				can_setled(0, 1);
				bla = 1;
			} else
			{
				can_setled(0, 0);
				bla = 0;
			}
			rfm12_rx_clear();
		}
		wdt_reset();
	}
}

