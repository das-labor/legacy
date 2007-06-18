#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "global.h"
#include "rf12.h"     
#include "config.h"
#include "uart.h"

#include <util/delay.h>

void send(uint8_t i);
void receive(void);


void dump_registers (void)
{
	uint8_t i, k;
	uint16_t rxbuf;

	printf("\r\n");
	for (i=0;i<0xff;i++)
	{
		rxbuf = rf12_trans((i));
		if (rxbuf != 0x809c)
		{
			printf("%2x = %x\t", i, rxbuf);
			if (k == 7)
			{
				printf("\r\n");
			}
			k = (k == 7) ? 0 : k+1;
		}
	}
	rxbuf = rf12_trans(0x0000 | i);
	printf("%i = %x\t", i, rxbuf);
	printf("\r\n");
}

int main(void)
{
	unsigned char i, tx=0x01, k=0;
	uint16_t rxbuf = 0x0000;
	DDRC = 0xff;
	DDRD &= ~(_BV(PD2));
	DDRB &= ~(_BV(PB0));
	PORTB |= _BV(PB0);

	uart_init();
	uart_putstr("Hallo\r\n");
	fdevopen(uart_putc, 0);

	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
	rf12_setfreq(RF12FREQ(434.0));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setbandwidth(4, 1, 4);	// 200kHz Bandbreite, -6dB Verstärkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);			// 19200 baud
	rf12_setpower(0, 6);			// 1mW Ausgangangsleistung, 120kHz Frequenzshift
	PORTC = 0xff;

	for (;;)
	{
		if (k<7)
			tx <<= 1;
		else
			tx >>= 1;
		k++;
		if (k==14) k=0;
		
		receive();
		if (!(PINB & _BV(PB0)))
		{
			send(tx);
			_delay_ms(100);
		}
		rxbuf = rf12_trans(0xCA83);
	}
}

void receive(void)
{
	uint16_t moo = 0x00;
//	moo = rf12_rxdata();
	moo = rf12_rxdata_nb(0x5440);
	if (moo > 0x00ff)
		PORTC = (uint8_t) (moo & 0x00ff);
}

void send(uint8_t i)
{
//	PORTC = i;
	rf12_txdata(i,1);
}

