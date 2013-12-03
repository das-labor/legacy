#include <stdint.h>

#include "config.h"
#include "io.h"
#include "uart/uart.h"
#include "uart_handler.h"
#include "Bastelcmd.h"

uint8_t eingang_status = 0;
static uint8_t led_status = 0;


static void uart_send_led_output(void)
{
	uart_putc('c');
	uart_putc(led_status);
	uart_putc('\n');
}


void uart_handler()
{
	char c;
	static uint8_t state = 0;
	if (uart_getc_nb(&c))
	{
		switch (state)
		{
			case 0:
				switch (c)
				{
					case 's':
						state = 2;
						uart_send_led_output();
						break;
					case 't':
						PORTB |= _BV(PB0);
						state = 1;
						break;
					default:
						state = 0;
				}
				break;
			case 1:
				eingang_status = (uint8_t) c;
				state = 2;
				break;
			case 2:
				if (c == '\n')
					state = 0;
				break;
		}
	}
}

void uart_set_led_output(uint8_t led, uint8_t val)
{
	if (val)
		led_status |=  (1 << led);
	else
		led_status &= ~(1 << led);
	uart_send_led_output();
}

void uart_set_taster_led(uint8_t sreg)
{
	if (sreg)
	{
		led_status |= _BV(RGBLED_R);
		led_status &= ~_BV(RGBLED_G);
	}
	else
	{
		led_status |= _BV(RGBLED_G);
		led_status &= ~_BV(RGBLED_R);
	}
	uart_send_led_output();
}
