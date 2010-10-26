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
#include "avr-asm-macros.S"
#include "uart_defs.h"
/******************************************************************************/

#if UART0_NI

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

#define BAUD (UART0_BAUD_RATE)
#include "setbaud_asm.inc"

/******************************************************************************/

.global uart0_init
uart0_init:
	ldi r24, UBRRH_VALUE
	STORE_IO UBRR0H, r24
	ldi r24, UBRRL_VALUE
	STORE_IO UBRR0L, r24
#if _SFR_IO_REG_P(UCSR0A)
  #if USE_2X
	sbi _SFR_IO_ADDR(UCSR0A), 1
  #else
	cbi _SFR_IO_ADDR(UCSR0A), 1
  #endif
#else 
	lds r24, _SFR_MEM_ADDR(UCSR0A)
  #if USE_2X
	ori r24, 0x02
  #else
	andi r24, ~0x02
  #endif
	sts _SFR_MEM_ADDR(UCSR0A), r24
#endif
	ldi r24, (UART0_PARATY<<4)|(UART0_STOPBITS<<3)|((UART0_DATABITS&3)<<1)
	STORE_IO UCSR0C, r24
	ldi r24, _BV(TXEN0)|_BV(RXEN0)|((UART0_DATABITS>>2)<<2)  
	STORE_IO UCSR0B, r24
	ret

/******************************************************************************/
	
.global uart0_putc
uart0_putc:
#if _SFR_IO_REG_P(UCSR0A)
	sbis _SFR_IO_ADDR(UCSR0A), UDRE0
	rjmp uart0_putc
#else
	lds r25, _SFR_MEM_ADDR(UCSR0A)
	sbrs r25, UDRE0
	rjmp uart0_putc
#endif
#if UART0_DATABITS == UART_DATABITS_9
#  if _SFR_IO_REG_P(UCSR0B)
    sbi UCSR0B, TXB80
	sbrs r25, 0
	cbi UCSR0B, TXB80
#  else
	lds r23, _SFR_MEM_ADDR(UCSR0B)
	bst r25, 0
	bld r23, TXB80
	sts _SFR_MEM_ADDR(UCSR0B), r23
#  endif
#endif
	STORE_IO UDR0, r24
	ret
	
/******************************************************************************/
	
.global uart0_getc
uart0_getc:
#if _SFR_IO_REG_P(UCSR0A)
	sbis _SFR_IO_ADDR(UCSR0A), RXC0
	rjmp uart0_putc
#else
	lds r25, _SFR_MEM_ADDR(UCSR0A)
	sbrs r25, RXC0
	rjmp uart0_getc
#endif

#if UART0_DATABITS == UART_DATABITS_9
	LOAD_IO r25, UCSR0B
	lsr r25
	andi r25, 1
#else
	clr r25
#endif	
	LOAD_IO r24, UDR0
	ret
	
/******************************************************************************/
	
.global uart0_dataavail
uart0_dataavail:
	LOAD_IO r24, UCSR0A
	andi r24, _BV(RXC0)
	clr r25
	ret

#endif /* UART0_NI */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#if UART1_NI

#ifndef UDRE1
# error "registernames for second UART not defined"
#endif

#ifndef UART1_PARATY
# warning "UART1: using default paraty: 'none'"
# define UART1_PARATY UART_PARATY_NONE
#endif

#ifndef UART1_STOPBITS
# warning "UART1: using default ammount of stop bits: '1'"
# define UART1_STOPBITS UART_STOPBITS_1
#endif

#ifndef UART1_DATABITS
# warning "UART1: using default ammount of data bits: '8'"
# define UART1_DATABITS UART_DATABITS_8
#endif

#ifdef BAUD
#  undef BAUD
#endif

#define BAUD (UART1_BAUD_RATE)

#include "setbaud_asm.inc"

/******************************************************************************/

.global uart1_init
uart1_init:
	ldi r24, UBRRH_VALUE
	STORE_IO UBRR1H, r24
	ldi r24, UBRRL_VALUE
	STORE_IO UBRR1L, r24
#if _SFR_IO_REG_P(UCSR1A)
  #if USE_2X
	sbi _SFR_IO_ADDR(UCSR1A), 1
  #else
	cbi _SFR_IO_ADDR(UCSR1A), 1
  #endif
#else 
	lds r24, _SFR_MEM_ADDR(UCSR1A)
  #if USE_2X
	ori r24, 0x02
  #else
	andi r24, ~0x02
  #endif
	sts _SFR_MEM_ADDR(UCSR1A), r24
#endif
	ldi r24, (UART1_PARATY<<4)|(UART1_STOPBITS<<3)|((UART1_DATABITS&3)<<1)
	STORE_IO UCSR1C, r24
	ldi r24, _BV(TXEN1)|_BV(RXEN1)|((UART1_DATABITS>>2)<<2)  
	STORE_IO UCSR1B, r24
	ret
	
/******************************************************************************/

.global uart1_putc
uart1_putc:
#if _SFR_IO_REG_P(UCSR1A)
	sbis _SFR_IO_ADDR(UCSR1A), UDRE1
	rjmp uart1_putc
#else
	lds r23, _SFR_MEM_ADDR(UCSR1A)
	sbrs r23, UDRE1
	rjmp uart1_putc
#endif
#if UART1_DATABITS == UART_DATABITS_9
#  if _SFR_IO_REG_P(UCSR1B)
    sbi UCSR1B, TXB81
	sbrs r25, 0
	cbi UCSR1B, TXB81
#  else
	lds r23, _SFR_MEM_ADDR(UCSR1B)
	bst r25, 0
	bld r23, TXB81
	sts _SFR_MEM_ADDR(UCSR1B), r23
#  endif
#endif
	STOREIO UDR1, r24
	ret
	
/******************************************************************************/
	
.global uart1_getc
uart1_getc:
#if _SFR_IO_REG_P(UCSR1A)
	sbis _SFR_IO_ADDR(UCSR1A), RXC1
	rjmp uart1_putc
#else
	lds r25, _SFR_MEM_ADDR(UCSR1A)
	sbrs r25, RXC1
	rjmp uart1_getc
#endif
#if UART1_DATABITS == UART_DATABITS_9
	LOAD_IO r25, UCSR1B
	lsr r25
	andi r25, 1
#else
	clr r25
#endif	
	LOAD_IO r24, UDR1
	ret
	
/******************************************************************************/

.global uart1_dataavail
uart1_dataavail:
	LOAD_IO r24, UCSR1A
	andi r24, _BV(RXC1)
	clr r25
	ret

#endif /* UART1_NI */
