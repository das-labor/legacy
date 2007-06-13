/* This is an example & test for librfmxx. It sends out data and "displays" received
 * data by useing an entire port for it.
 *
 *
 */

#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>

#include "rfm_config.h"
#include "rfm12.h"

#define MYMODE_RX 0x01
#define MYMODE_TX 0x02


void main ( void )
{
	uint8_t mymode = MYMODE_RX;
	uint16_t i = 0, tmp = 0;

	rfm_init_rx();

	while (42)
	{
		if (PORTB & _BV(PB0) && i = 0x0000)
		{
			mymode |= MYMODE_TX;
			i = 0xffff;
			rfm_init_tx();
			PORTC |= _BV(PC0);
			rfm_tx (0xAA);
		}

		if (i>0) i--;
		
		// switch back to rx
		if (i == 0 && mymode & MYMODE_TX)
		{
			mymode |= MYMODE_RX;
			rfm_init_rx();
			PORTC = 0x00;
		}
		
		// blink around while in tx mode
		if (i & 0x0fff) PORTC ^= _BV(PC0);

		tmp = rfm_rx_nb();

		if (tmp > 0x00ff)
		{
			PORTB = (tmp & 0x00ff);
		}
	}
}
