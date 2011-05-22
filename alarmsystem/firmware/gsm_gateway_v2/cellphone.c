#include "cellphone.h"

volatile uint8_t at_bufpos = 0;
volatile char at_rx_buf[16];
volatile char at_expect_buf[16];

void cellphone_init()
{
	memset (at_rx_buf, 0, sizeof(at_rx_buf));
	memset (at_expect_buf, 0, sizeof(at_rx_buf));
	return;
}

void cellphone_tick()
{
	if (!uart_getc_nb(at_rx_buf + at_bufpos))
		return;
	
	if (at_bufpos == sizeof(at_rx_buf)-1)
		return;

	at_bufpos++;


}
