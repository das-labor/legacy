#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/crc16.h>

#include "config.h"

volatile static uint8_t tx_seq[16];
volatile static uint8_t num_bytes = 0;

BIT_ISR
{
	static uint8_t bytec = 0;
	static uint8_t mask = 0xff;

	if (!mask) /* stop bit(s) */
	{

		if (bytec < num_bytes)
		{
			mask = 0xff; /* start bit */
			asm volatile("nop"); /* shortest code path - add some nops */
			asm volatile("nop");
			OUT0_1;
			return;
		}
		
		OUT0_1; /* intentionally twice to make delays close to symmetric */

		/* send extra stop bits at the end of the transmission */
		if (bytec < num_bytes + NUM_STOPBITS)
			return;
		
		/* start over */
		mask = 0xff;
		bytec = 0;
	} else if (mask == 0xff) /* start bit */
	{
		bytec++;
		mask = 0x80;
		OUT0_0;
	} else /* data bits */
	{
		if (tx_seq[bytec] & mask)
		{
			OUT0_1;
		} else
		{
			OUT0_0;
		}
		mask >>= 1;
	}
}

/* read eeprom, fill tx_seq
 */
void eeprom_init()
{
	uint16_t *crcptr;
	uint8_t i;

	eeprom_busy_wait();
	num_bytes = eeprom_read_byte ((const void*) 0x00);
	
	if (num_bytes > sizeof(tx_seq) - 2)
	{
		num_bytes = sizeof (tx_seq) - 2;
		tx_seq[0] = 'E';
		tx_seq[1] = 'S';
		tx_seq[2] = 'I';
		tx_seq[3] = 'Z';
		tx_seq[4] = 'E';
		return; /* unprogrammed */
	}
	eeprom_busy_wait();
	eeprom_read_block ((void*) &tx_seq[0], (const void*) 0x01, num_bytes);

	/* calc & gen crc */
	crcptr = (uint16_t *) &tx_seq[num_bytes];
	*crcptr = 0x00;
	
	for (i=0;i<num_bytes;i++)
	{
		*crcptr = _crc16_update (*crcptr, tx_seq[i]);
	}
	
	num_bytes += 2;
}

int main ()
{
	eeprom_init();
	PRESCALER_INIT;
	OUT0_INIT;
	TIMER_INIT;
	sei();

	while (23)
	{

	}
}
