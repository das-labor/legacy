#include "ook.h"

volatile static rfmusb_ook_t tx_rq;
volatile static uint8_t tx_bits[RFMUSB_OOK_MAXLEN];

/* copy usb buffer to local tx_rq */
void ook_queue_msg (uint8_t *in_msg)
{
	memcpy ((uint8_t *) &tx_rq, in_msg, sizeof(rfmusb_ook_t));
	if (tx_rq.len > RFMUSB_OOK_MAXLEN*8)
		tx_rq.len = RFMUSB_OOK_MAXLEN*8;

	if (tx_rq.len == 0)
		return;

	memcpy ((uint8_t *) tx_bits, in_msg + sizeof(rfmusb_ook_t), tx_rq.len / 8);
}

/* transmit an ook message if there's one in the buffer */
void ook_tick ()
{
	if (!tx_rq.count)
		return;
	
	tx_rq.count--;
	ook_send (tx_rq.len, tx_rq.delay, (uint8_t *) tx_bits, tx_rq.type);
}

/* 1us - timer */
inline void ook_delay (uint16_t in_delay)
{
	uint16_t tdelay = in_delay << 1;
	tdelay += in_delay >> 1; /* + 0.5 */
	TCCR1B = 0x00;      /* stop timer */
	TCNT1 =  1;         /* set value to 1 */
	TCCR1B = _BV(CS11); /* normal mode, clk/8 => 250k cycles/s */

	while (TCNT1 < tdelay)
		asm volatile ("nop");
}

void ook_send (uint8_t in_bits, uint16_t in_delay, uint8_t *in_data, uint8_t in_type)
{
	uint8_t msk = 0x80;
	uint8_t bytes = in_bits >> 3, current_byte;
	
	rfm12_ask_tx_mode(1);
	rfm12_tx_off();
	
	for (current_byte = 0; current_byte < bytes;)
	{
		if (in_type == OOK_2722)
		{
			/* leading 1 */
			rfm12_tx_on();
			ook_delay (in_delay);
		}

		if (in_data[current_byte] & msk)
		{
			rfm12_tx_on();
		} else
		{
			rfm12_tx_off();
		}
		ook_delay (in_delay); 
		
		if (in_type == OOK_2722) /* add a 0 at the end */
		{
			rfm12_tx_off();
			ook_delay (in_delay); 
		}

		msk >>= 1;
		if (msk == 0x00)
		{
			current_byte++;
			msk = 0x80;
		}
		if (in_bits == 0)
			break;
		in_bits--;
	}
	
	if (in_type == OOK_2722) /* trailing 1 */
	{
		rfm12_tx_on();
		ook_delay (in_delay);
		rfm12_tx_off();
		
		ook_delay (in_delay * 12);
	}

	rfm12_ask_tx_mode(0);
}
