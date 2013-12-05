#include <stdint.h>

#include "config.h"
#include "io.h"
#include "uart/uart.h"
#include "uart_handler.h"


void uart_handler()
{
	char c;
	static uint8_t state = 0;
	if (uart_getc_nb(&c))
	{
		switch (state)
		{
			case 0:
				if (c == 'c')
				{
					state = 1;
				}
				else
					state = 0;
				break;
			case 1:
				set_leds(c);
				state = 2;
				break;
			case 2:
				if (c == '\n')
					state = 0;
				break;
		}
	}
}

void uart_send_status(uint8_t taster_status)
{
	uart_putc('t');
	uart_putc(taster_status);
	uart_putc('\n');
}

void uart_get_led_output()
{
	uart_putc('s');
	uart_putc('\n');
}

