/* uart_ni.c */
/*
    This file is part of the AVR-uart_ni.
    Copyright (C) 2009 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"

#include <avr/io.h>
#include "uart_ni.h"

#if UART0

#ifndef UART0_PARATY
# warning "UART0: using default paraty: 'none'"
# define UART0_PARATY UART_PARATY_NONE
#endif

#ifndef UART0_STOPBITS
# warning "UART0: using default ammount of stop bits: '1'"
# define UART0_STOPBITS UART_STOPBITS_1
#endif

#ifndef UART0_DATABITS
# warning "UART0: using default ammount of data bits: '8'"
# define UART0_DATABITS UART_DATABITS_8
#endif


#ifdef UDR
# define OLD_UART
# ifdef UDR0
#  error "can not decide which registernames to use, UDR and UDR0 are defined"
# endif
#endif

#ifdef OLD_UART
# define UCSR0A UCSRA
# define UCSR0B UCSRB
# define UCSR0C UCSRC
# define UBRR0H UBRRH
# define UBRR0L UBRRL
# define UDR0   UDR
# define TXEN0  TXEN
# define RXEN0  RXEN
# define UDRE0  UDRE
# define RXC0   RXC
# define TXB80  TXB8
# define RXB80  RXB8
#endif

void uart0_init(void){
	#define BAUD UART0_BAUD_RATE
	#include <util/setbaud.h>	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
	UCSR0A |= _BV(U2X0);
	#else
	UCSR0A &= ~_BV(U2X0);
	#endif
 	UCSR0C = (UART0_PARATY<<4)|(UART0_STOPBITS<<3)|((UART0_DATABITS&3)<<1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | ((UART0_DATABITS>>2)<<2); /* enable TX and RX */
}

void uart0_putc(uint16_t c){
	while(!(UCSR0A & _BV(UDRE0))) /* wait for UDR0 to be empty */
     ;
#if UART0_DATABITS == UART_DATABITS_9	 
	UCSR0B |= (c>>8)&1 
#endif	
	UDR0 = (uint8_t)c;
}

uint16_t uart0_getc(void){
	uint16_t c;
	while(!(UCSR0A & _BV(RXC0))) /* wait for data to arrive */
     ;
#if UART0_DATABITS == UART_DATABITS_9
	c = (UCSR0B>>1)&1
	c <<= 8;
	c |= UDR0; 
	return c;
#else
	return UDR0;
#endif		
}

uint16_t uart0_getc(void){
	uint16_t c;
	while(!(UCSR0A & _BV(RXC0))) /* wait for data to arrive */
     ;
#if UART0_DATABITS == UART_DATABITS_9
	c = (UCSR0B>>1)&1
	c <<= 8;
	c |= UDR0; 
	return c;
#else
	return UDR0;
#endif		

uint8_t uart0_dataavail(void){
	return(UCSR0A & _BV(RXC0));
}

#endif /* UART0 */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#if UART1

void uart1_init(void){
	#define BAUD UART0_BAUD_RATE
	#include <util/setbaud.h>	
	UBRR1H = UBRRH_VALUE;
	UBRR1L = UBRRL_VALUE;
	#if USE_2X
	UCSR1A |= _BV(U2X1);
	#else
	UCSR1A &= ~_BV(U2X1);
	#endif
 	UCSR1C = (UART1_PARATY<<4)|(UART1_STOPBITS<<3)|((UART1_DATABITS&3)<<1);
	UCSR1B = _BV(RXEN1) | _BV(TXEN1) | ((UART1_DATABITS>>2)<<2); /* enable TX and RX */
}

void uart1_putc(uint16_t c){
	while(!(UCSR1A & _BV(UDRE1))) /* wait for UDR0 to be empty */
     ;
#if UART1_DATABITS == UART_DATABITS_9	 
	UCSR1B |= (c>>8)&1 
#endif	
	UDR1 = (uint8_t)c;
}

uint16_t uart1_getc(void){
	uint16_t c;
	while(!(UCSR1A & _BV(RXC1))) /* wait for data to arrive */
     ;
#if UART1_DATABITS == UART_DATABITS_9
	c = (UCSR1B>>1)&1
	c <<= 8;
	c |= UDR1; 
	return c;
#else
	return UDR1;
#endif		
}

uint8_t uart1_dataavail(void){
	return(UCSR1A & _BV(RXC1));
}

#endif /* UART1 */
