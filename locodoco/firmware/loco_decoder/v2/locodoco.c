#include "locodoco.h"

volatile static locodoco_addr_t myaddress = 0x00;

void locodoco_init ()
{
	eeprom_read_block ((void*) &myaddress, EEP_MYADDRESS, sizeof(locodoco_addr_t));
}

void locodoco_tick ()
{
	uint8_t rxlen;
	uint8_t *rxbuf;

	if (rfm12_rx_status() != STATUS_COMPLETE)
		return;
	
	rxlen = rfm12_rx_len();
	rxbuf = rfm12_rx_buffer();

	if (rxlen < LPROTO_MINLEN)
		goto out_clear;
	if (rfm12_rx_type() != LPROTO_T0)
		goto out_clear;
	if (LPROTO_PREFIX(rxbuf) != LPROTO_T1)
		goto out_clear;

	/* finally! ... a valid locodoco packet
	 */
	
	switch (LPROTO_TYPE(rxbuf))
	{
		case LPROTO_TYPE_RESET:
			if (LPROTO_ADDR(rxbuf) != myaddress)
				goto out_clear;
			wdt_enable (WDTO_15MS);
			rfm12_rx_clear();
			while (23); /* wait for watchdog reset */
		break;
		case LPROTO_TYPE_PING:
			LPROTO_TYPE(rxbuf) = LPROTO_TYPE_PONG;
			rfm12_rx_clear();

			if (LPROTO_ADDR(rxbuf) != myaddress)
				return;
			rfm12_tx (LPROTO_MINLEN, LPROTO_T0, rxbuf);
		break;
		case LPROTO_PWM_SET:
			
		break;
	}
out_clear:
	rfm12_rx_clear();
}
