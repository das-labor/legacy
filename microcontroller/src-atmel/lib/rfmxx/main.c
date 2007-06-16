#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include "global.h"
#include "rf12.h"     
#include "config.h"
#include "uart.h"

//#define F_CPU 10000000UL
#include <util/delay.h>

void send(uint8_t i);
void receive(void);

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

//	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
//	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
	rf12_init();					// ein paar Register setzen (z.B. CLK auf 10MHz)
	//rf12_setfreq(RF12FREQ(433.92));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setfreq(RF12FREQ(434.0));	// Sende/Empfangsfrequenz auf 433,92MHz einstellen
	rf12_setbandwidth(4, 1, 4);		// 200kHz Bandbreite, -6dB Verstärkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);			// 19200 baud
	rf12_setpower(0, 6);			// 1mW Ausgangangsleistung, 120kHz Frequenzshift
	PORTC = 0xff;
//	while (1) asm volatile ("nop");

	for (;;)
	{
		if (!(PINB & _BV(PB0)))
		{
			send(tx);
			_delay_ms(50);
		}

		if (k<7)
			tx <<= 1;
		else
			tx >>= 1;
		k++;
		if (k==14) k=0;
		
		{
			receive();
		}
	}
}

void receive(void)
{
	uint16_t moo = 0x00;
	PORTC = rf12_rxdata(); return;
	moo = rf12_rxdata_nb();
	if (moo > 0xff)
		PORTC = (moo & 0x00ff);
}

void send(uint8_t i)
{
//	PORTC = i;
	rf12_txdata(i,1);
}

