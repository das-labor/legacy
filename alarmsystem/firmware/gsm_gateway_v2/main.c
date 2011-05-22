#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdlib.h>

#include "pinning.h"
#include "../../common/protocol.h"
#include "onewire.h"
#include "ds18x20.h"

#include "rfm12_config.h"
#include "rfm12.h"
#include "cellphone.h"
#include "uart.h"

#define SNOOZE_TICK   0x00
#define SNOOZE_30     0x01
#define SNOOZE_TOGGLE 0x02

#define LED_GREEN() \
	PORTA |= (_BV(PA6)); \
	PORTA &= ~(_BV(PA7))

#define LED_RED() \
	PORTA &= ~(_BV(PA6)); \
	PORTA |= (_BV(PA7))

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

	if (gtime.s % 10 == 0)
	{
		uint8_t txbuf[8];
		send_packet (ADDR_BCAST, CMD_ALIVE, 5, txbuf);
	}

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

static void send_temp ()
{
	#define NUM_SENSORS 1
	static uint8_t sensor_id[NUM_SENSORS][OW_ROMCODE_SIZE];
	static uint8_t nfound = 0;
	uint8_t txbuf[(NUM_SENSORS*2) + 4];
	uint16_t temp;
	static uint16_t oldtemp;
	uint8_t i;

	if (!nfound)
	{
		uint8_t dnum = OW_SEARCH_FIRST;
		
		ow_reset();
	
		while (dnum != OW_LAST_DEVICE && nfound < NUM_SENSORS)
		{
			DS18X20_find_sensor (&dnum, &sensor_id[nfound][0]);
			if (dnum == OW_PRESENCE_ERR || dnum == OW_DATA_ERR)
			{
				break;
			}

			nfound++;
		}

		if (!nfound)
		{
			LED_RED();
			return;
		}
	}
	for (i=0;i<nfound*2;i+=2)
	{
		if (DS18X20_conversion_in_progress ())
			return;

		DS18X20_read_decicelsius (&sensor_id[i][0], &temp);
		
		DS18X20_start_meas( DS18X20_POWER_PARASITE, NULL );

		if (oldtemp == temp)
			return;

		PORTB ^= _BV(PB0);
		oldtemp = temp;
		txbuf[i] = temp >> 8;
		txbuf[i+1] = temp & 0xff;
	}
	txbuf[++i] = nfound;
	txbuf[++i] = 0x73;
	txbuf[++i] = 0x37;
	send_packet (ADDR_BCAST, CMD_ALIVE, i, txbuf);
}

int main (void)
{
	uint8_t i;
	uint8_t txbuf[12];
	uint8_t t = 0;
	uint8_t last_tx = 0;
	
	// ow_set_bus (&PINC, &PORTC, &DDRC, PC0);
	
	timer0_init();

	wdt_enable(WDTO_2S);

	/* act led */
	DDRB |= _BV(PB0);
	PORTB |= _BV(PB0);
	
	/* DUO LED */
	DDRA |= (_BV(PA6) | _BV(PA7));
	LED_GREEN();
	
	/* BTN */
	DDRC &= ~(_BV(PC1));
	PORTC |= _BV(PC1);

	DDRD |= _BV(PD1);

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
	txbuf[0] = myaddr;
	send_packet (ADDR_BCAST, CMD_ALIVE, 5, txbuf);
	txbuf[0] = 0x00;
	

	while (23)
	{
		process_packet();
		if (!(PINC & _BV(PC1)))
		{
			snoozed (SNOOZE_TOGGLE);
			t = 0;
			while (!(PINC & _BV(PC1)))
			{
				t++;
				_delay_ms(100);
				wdt_reset();
			}

			if (t > 30 && t < 100) /* btn pressed for > 3s */
				uart_putstr ("ATD+4980012345678;\r\n");
			if (t > 100) /* btn pressed for > 10s */
			{
				/* reset device, wait for watchdog to kick in */
				while (23);
			}
		}
		
		if (gtime.s % 2 == 0 && gtime.s != last_tx)
		{
			send_temp();
			last_tx = gtime.s;
		}

		snoozed (SNOOZE_TICK);
		wdt_reset();
		rfm12_tick();
	}
}
