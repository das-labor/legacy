#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>

#include "pinning.h"
#include "../../common/protocol.h"

#include "motion.h"
#include "rfm12_config.h"
#include "rfm12.h"

uint8_t myaddr = 0x00;

void fill_adcvals (uint8_t *in_buf)
{
	in_buf[0] = get_adc(0) >> 8;
	in_buf[1] = get_adc(0) & 0xff;
	in_buf[2] = get_adc(1) >> 8;
	in_buf[3] = get_adc(1) & 0xff;
}

void send_packet (uint8_t in_dst, uint8_t in_cmd, uint8_t in_len, uint8_t *in_data)
{
	uint8_t txbuf[16], i;

	txbuf[F_T0]  = PROTO_TYPE0;
	txbuf[F_T1]  = PROTO_TYPE0;
	txbuf[F_SRC] = in_dst;
	txbuf[F_DST] = in_dst;
	txbuf[F_CMD] = in_cmd;
	
	for (i=F_DATA+1;i<(sizeof(txbuf)-PACKET_MINLEN);i++)
	{
		if (i >= in_len) break;
		txbuf[i] = in_data[i];
	}

	rfm12_tx (in_len + PACKET_MINLEN, RFM12_PROTO_TYPE, txbuf);
}

void process_packet ()
{
	uint8_t *rxbuf;
	uint8_t rxlen, txbuf[8];

	if (rfm12_rx_status() != STATUS_COMPLETE)
		return;
	
	rxlen = rfm12_rx_len();
	if (rxlen < PACKET_MINLEN || rfm12_rx_type() != RFM12_PROTO_TYPE)
	{
		rfm12_rx_clear();
		return;
	}

	rxbuf = rfm12_rx_buffer();
	if (!(rxbuf[F_T0] == PROTO_TYPE0 && rxbuf[F_T1] == PROTO_TYPE1 &&
		(rxbuf[F_DST] == myaddr || rxbuf[F_DST] == ADDR_BCAST)))
	{
		rfm12_rx_clear();
		return;
	}
	
	PORTB ^= 0x03;

	switch (rxbuf[F_CMD])
	{
		case CMD_RESET:
			rfm12_rx_clear();
			wdt_enable (WDTO_15MS);
			while(23)
			{
				asm volatile ("nop");
			}
		break;
		case CMD_PING:
			fill_adcvals (txbuf);
			txbuf[4] = rxbuf[PACKET_MINLEN]++;
			rfm12_rx_clear();
			send_packet (rxbuf[F_SRC], CMD_PONG, 5, txbuf);
		return;
		case CMD_STATE_SET:
			if (rxbuf[F_CMD+1] & MTD_STATE_ENABLED)
			{
				mtd_enable();
			} else
			{
				mtd_disable();
			}
		break;
	}
	rfm12_rx_clear();
}


int main (void)
{
	uint8_t i;
	uint8_t txbuf[12];
	uint16_t t = 0;

	DDRB |= 0x03;
	PORTB = 0x01;

	rfm12_init();
	
	for (i=0;i<4;i++)
		_delay_ms(250);

	eeprom_busy_wait();
	myaddr = eeprom_read_byte(0);

	motion_set_refval();
	rfm12_init();
	rfm12_rx_clear();
	wdt_enable(WDTO_120MS);
	mtd_enable();
	sei();

	while (23)
	{
		t++;
		/* alive signal */
		if (t == 0x4fff)
		{
			PORTB ^= 0x03;
			fill_adcvals (txbuf);
			send_packet (ADDR_BCAST, CMD_ALIVE, 4, txbuf);
			motion_set_refval();
			t = 0;
		}
		process_packet();
		motiond_tick();
		if (motion_check())
		{
			fill_adcvals (txbuf);
			send_packet (ADDR_BCAST, CMD_MOTION, 4, txbuf);
		}
		wdt_reset();
		rfm12_tick();
	}
}
