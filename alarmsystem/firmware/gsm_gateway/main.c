#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>

#include "pinning.h"
#include "../../common/protocol.h"

#include "rfm12_config.h"
#include "rfm12.h"
#include "cellphone.h"
#include "uart.h"

volatile uint8_t myaddr = 0x00;

void timer0_init()
{
	/* 
		CLK/64
		=> 976 ticks, 244 measurements per second per ADC
	*/
	TCCR0 = (_BV(CS01) | _BV(CS00));
	TIMSK |= _BV(TOIE0);
}


void send_packet (uint8_t in_dst, uint8_t in_cmd, uint8_t in_len, uint8_t *in_data)
{
	uint8_t txbuf[16], i, k=0;

	txbuf[F_T0]  = PROTO_TYPE0;
	txbuf[F_T1]  = PROTO_TYPE1;
	txbuf[F_SRC] = myaddr;
	txbuf[F_DST] = in_dst;
	txbuf[F_CMD] = in_cmd;
	
	for (i=F_DATA;i<(sizeof(txbuf)-PACKET_MINLEN);i++)
	{
		if (i >= in_len + PACKET_MINLEN) break;
		txbuf[i] = in_data[k];
		k++;
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
		case CMD_MOTION_TRESH_SET:
			mtd_set_treshold(rxbuf[F_CMD+1]);
		break;
		case CMD_TESTDIAL:

		break;
	}
	rfm12_rx_clear();
}

ISR(TIMER0_OVF_vect) 
{
	motiond_tick();
}


int main (void)
{
	uint8_t i;
	uint8_t txbuf[12];
	uint16_t t = 0, tmp;

	uart_init();	
	rfm12_init();
	
	for (i=0;i<4;i++)
		_delay_ms(250);

	eeprom_busy_wait();
	myaddr = eeprom_read_byte(0);

	rfm12_init();
	rfm12_rx_clear();
	wdt_enable(WDTO_120MS);
	sei();
	
	_delay_ms (250);
	uart_putstr ("ATD+4917649333333\n");

	while (23)
	{
		t++;
		/* alive signal */
		if (t == 0x9fff)
		{
			send_packet (ADDR_BCAST, CMD_ALIVE, 5, txbuf);
			t = 0;
		}
		process_packet();
		
		if (!(PINC & _BV(PC0)))
			send_packet (ADDR_BCAST, CMD_PWRFAIL, 2, txbuf);

		wdt_reset();
		rfm12_tick();
	}
}
