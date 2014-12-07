#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "config.h"
#include "io.h"
#include "uart/uart.h"
#include "uart_handler.h"


static volatile uint16_t tickscounter = 0;
ISR(TIMER0_OVF_vect)
{
	tickscounter++;
}

static void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	TCCR0 = _BV(CS02); // clk / 256 - 125000 
	TIMSK = _BV(TOIE0);

	uart_init();
	io_init();

	sei(); // turn on interrupts
	wdt_enable(WDTO_250MS);
	uart_get_led_output();
}

int main(void)
{
	init(); // system initialization

	while (1)
	{
		uart_handler();
		if (tickscounter > 6) // 20 mal pro sec
		{
			tickscounter = 0;
			input_handler();
		}
		wdt_reset();
	}
}
