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

#define SNOOZE_TICK   0x00
#define SNOOZE_30     0x01
#define SNOOZE_TOGGLE 0x02

#define LED_GREEN() \
	PORTC |= (_BV(PC4)); \
	PORTC &= ~(_BV(PC5))

#define LED_RED() \
	PORTC &= ~(_BV(PC4)); \
	PORTC |= (_BV(PC5))

volatile uint8_t myaddr = 0x00;
volatile static uint8_t ignore_alert = 0;

typedef struct timer_s
{
	uint8_t s;
	uint16_t ms;
	uint8_t m;
	uint8_t h;
} timer_t;

volatile static timer_t gtime;

void timer0_init()
{
	TCCR0 = (_BV(CS02) | _BV(CS00)); /* CLK / 1024 */
	TIMSK |= _BV(TOIE0);
	TCNT0 = 248; /* next overflow in 8 cycles */
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

void snoozed (uint8_t in_rq)
{
	static uint8_t dst_minute = 0xff;
	
	if (in_rq == SNOOZE_30 || (in_rq == SNOOZE_TOGGLE && dst_minute == 0xff))
	{
		dst_minute = (gtime.m + 30) % 60;
		ignore_alert = 1;
		LED_RED();
		return;
	}
	
	if (in_rq == SNOOZE_TOGGLE)
	{
		ignore_alert = 0;
		dst_minute = 0xff;
		LED_GREEN();
		return;
	}

	if (dst_minute != gtime.m || dst_minute == 0xff)
		return;
	
	LED_GREEN();
	dst_minute = 0xff;
	ignore_alert = 0;
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
			txbuf[4] = rxbuf[PACKET_MINLEN]++;
			rfm12_rx_clear();
			rfm12_tick();
			send_packet (rxbuf[F_SRC], CMD_PONG, 5, txbuf);
		return;
		case CMD_MOTION:
		case CMD_PWRFAIL:
			if (ignore_alert == 1)
				break;
			uart_putstr ("ATD+4917649333333;\r\n");
		break;
		case CMD_STATE_SET:
			switch (rxbuf[F_DATA])
			{
				case STATE_CALM:
					
				break;
			}
		break;
		default:
			break;
	}
	rfm12_rx_clear();
}

ISR(TIMER0_OVF_vect) 
{
	TCNT0 = 248; /* next overflow in 8 cycles (ca. 1ms) */
	gtime.ms++;
	
	if (!(gtime.ms % 42))
		gtime.ms++;
	
	if (gtime.ms < 1000)
		return;

	gtime.ms = 0;
	gtime.s++;

	if (gtime.s < 60)
		return;

	gtime.s = 0;
	gtime.m++;

	if (gtime.m < 60)
		return;

	gtime.h++;
	gtime.m = 0;

	if (gtime.h < 24)
		return;

	gtime.h = 0;
}


int main (void)
{
	uint8_t i;
	uint8_t txbuf[12];
	uint8_t t = 0;
	
	timer0_init();

	wdt_enable(WDTO_2S);
	/* FSK */
	DDRD |= _BV(PD4);
	PORTD |= _BV(PD4);
	
	/* DUO LED */
	DDRC |= (_BV(PC4) | _BV(PC5));
	LED_GREEN();
	
	/* BTN */
	DDRC &= ~(_BV(PC3));
	PORTC |= _BV(PC3);

	uart_init();
	rfm12_init();
	
	for (i=0;i<4;i++)
		_delay_ms(250);

	eeprom_busy_wait();
	myaddr = eeprom_read_byte(0);

	rfm12_init();
	rfm12_rx_clear();
	sei();
	
	//uart_putstr ("ATD+4917649333333;\r\n");
	_delay_ms(250);
	//txbuf[0] = 0xb0;
	send_packet (ADDR_BCAST, CMD_ALIVE, 1, txbuf);
	txbuf[0] = 0x00;
	

	while (23)
	{
		process_packet();
		if (!(PINC & _BV(PC3)))
		{
			snoozed (SNOOZE_TOGGLE);
			t = 0;
			while (!(PINC & _BV(PC3)))
			{
				t++;
				_delay_ms(100);
				wdt_reset();
			}

			if (t > 30 && t < 100) /* btn pressed for > 3s */
				uart_putstr ("ATD+4917649333333;\r\n");
			if (t > 100) /* btn pressed for > 10s */
			{
				/* reset device, wait for watchdog to kick in */
				while (23);
			}
		}
		snoozed (SNOOZE_TICK);
		wdt_reset();
		rfm12_tick();
	}
}
