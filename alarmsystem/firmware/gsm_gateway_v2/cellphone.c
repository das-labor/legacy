#include "cellphone.h"

volatile uint8_t at_bufpos = 0;
volatile char at_rx_buf[16];
volatile char at_expect_buf[16];

void cellphone_dial (uint8_t in_num)
{
	uint8_t i = NUMBER_OFFSET, num = 0, tmp;
	
	while (num != in_num)
	{
		eeprom_busy_wait();
		tmp = eeprom_read_byte (i); /* length field */
		if (tmp >= NUMBER_MAXLEN || tmp == 0x00)
			return;
		i += tmp;
		num++;
	}
	
	uart_putstr_P (AT_DIAL_PREFIX);

	for (;i<tmp;i++)
	{
		eeprom_busy_wait();
		uart_putc (eeprom_read_byte(i));
	}
	uart_putstr_P (AT_DIAL_SUFFIX);
}

void cellphone_hangup ()
{
	uart_putstr_P (AT_HANGUP);
}

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
