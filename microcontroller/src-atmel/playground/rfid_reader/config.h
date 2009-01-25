#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

//c uart.[ch] defines
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 19200
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
//#define UART_LEDS 1


//c at-kbd.[ch] defines

#define KBD_DATA_DDR		DDRA
#define KBD_DATA_OUT_PORT	PORTA
#define KBD_DATA_IN_PORT	PINA
#define KBD_DATA_BIT		0

#define KBD_CLOCK_DDR		DDRB
#define KBD_CLOCK_OUT_PORT 	PORTB
#define KBD_CLOCK_IN_PORT 	PINB
#define KBD_CLOCK_BIT		2

#endif
