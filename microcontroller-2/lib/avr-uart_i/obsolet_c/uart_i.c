/* uart_i.c */
/*
    This file is part of the AVR-uart_i.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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
/**
 * \file     uart_i.c
 * \email    daniel.otte@rub.de
 * \author   Daniel Otte 
 * \date     2009-07-24
 * \license  GPLv3 or later
 * \ingroup  uart_i
 * \brief    implementation of interrupt based uart
 */

#include <stdlib.h>
#include <avr/interrupt.h>
#include "config.h"
#include "uart_i.h"

#define XON_VALUE  0x11
#define XOFF_VALUE 0x13

#if UART0_I

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

#if UART0_DATABITS == UART_DATABITS_9
# error "UART0: data bits==9 not supported"
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

uart0_ctx_t uart0_ctx;
uint8_t uart0_rxbuffer[UART0_RXBUFFER_SIZE];
uint8_t uart0_txbuffer[UART0_TXBUFFER_SIZE];

void uart0_init(void){
	circularbytebuffer_init2(UART0_RXBUFFER_SIZE, &(uart0_ctx.rxb), uart0_rxbuffer);
	circularbytebuffer_init2(UART0_TXBUFFER_SIZE, &(uart0_ctx.txb), uart0_txbuffer);
#if UART0_HOOK
	uart0_ctx.hook = NULL;
	uart0_ctx.hook_running = 0;
#endif
#if UART0_SWFLOWCTRL
	uart0_ctx.txon = 1;
	uart0_ctx.rxon = 1;
#endif
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
	UCSR0B = _BV(RXCIE0) | _BV(UDRIE0) | _BV(RXEN0) | _BV(TXEN0) ; /* enable TX and RX and interrupts */
	sei();
}

ISR(USART0_UDRE_vect){
	uint16_t x;
	x = circularbytebuffer_get_fifo(&(uart0_ctx.txb));
	if(x==0xffff){
		/* the transmit buffer is empty, disable interrupt */
		UCSR0B &= (uint8_t)~_BV(UDRIE0);
		return;
	}
#if UART0_SWFLOWCTRL
	while(!uart0_ctx.txon)
		;
#endif		
	UDR0 = x;
}

void uart0_putc (uint16_t c){
#if UART0_SWFLOWCTRL
	while(!uart0_ctx.txon)
		;
#endif	
	while(circularbytebuffer_cnt(&(uart0_ctx.txb))==UART0_TXBUFFER_SIZE)
		;
	cli();		
	circularbytebuffer_append((uint8_t)c, &(uart0_ctx.txb));
	sei();
	UCSR0B |= (uint8_t)_BV(UDRIE0);
}

ISR(USART0_RX_vect){
	uint16_t c;
	c = UDR0;
#if UART0_SWFLOWCTRL
	if(c==XON_VALUE){
		uart0_ctx.txon = 1;
		return;
	}
	if(c==XOFF_VALUE){
		uart0_ctx.txon = 0;
		return;
	}
#endif		
#if	UART0_HOOK
	if((!uart0_ctx.hook_running) && uart0_ctx.hook){
		uart0_ctx.hook_running=1;
		sei();
		do{
			uart0_ctx.hook(c);
		}while((c=circularbytebuffer_get_fifo(&(uart0_ctx.rxb)))!=0xffff);
		uart0_ctx.hook_running=0;
		return;
	}
#endif
	if(circularbytebuffer_cnt(&(uart0_ctx.rxb))==UART0_RXBUFFER_SIZE)
		return;
	circularbytebuffer_append(c, &(uart0_ctx.rxb));
#if UART0_SWFLOWCTRL
	if(circularbytebuffer_cnt(&(uart0_ctx.rxb))>UART0_THRESH_HIGH && uart0_ctx.rxon){
		uart0_ctx.rxon = 0;
		circularbytebuffer_push(XOFF_VALUE, &(uart0_ctx.txb));
		UCSR0B |= (uint8_t)_BV(UDRIE0);
	}
	if(circularbytebuffer_cnt(&(uart0_ctx.rxb))<UART0_THRESH_LOW && !uart0_ctx.rxon){
		uart0_ctx.rxon = 1;
		circularbytebuffer_push(XON_VALUE, &(uart0_ctx.txb));
		UCSR0B |= (uint8_t)_BV(UDRIE0);
	}
#endif		
}

uint16_t uart0_getc(void){
	uint8_t ret;
	while(circularbytebuffer_cnt(&(uart0_ctx.rxb))==0)
		;
	cli();	
	ret = circularbytebuffer_get_fifo(&(uart0_ctx.rxb));
	sei();
	return 	ret;
}

uint8_t uart0_dataavail(void){
	return circularbytebuffer_cnt(&(uart0_ctx.rxb));
}

#if	UART0_HOOK
void uart0_sethook(void(*fpt)(uint8_t)){
	uart0_ctx.hook = fpt;
}
#endif

#endif /* UART0_I */
