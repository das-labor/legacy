/* config.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CONFIG_H_
#define CONFIG_H_

//Art des Atmels und dessen Takt
#define F_CPU 16000000ul

/* set by Makefile */
//#define ATMEGA128
//#define ATMEGA644
//#define ATMEGA32


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Definitionen fuer SPI
#define SPI_HARDWARE 1
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_DDR DDRB
#define SPI_PIN_MOSI PB5
#define SPI_PIN_MISO PB6
#define SPI_PIN_SCK PB7
#define SPI_PIN_SS PB4

#define DEBUG uart

#define ERROR_HANDLING

//c uart.[ch] defines
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 9600
#define UART_RXBUFSIZE 128
#define UART_TXBUFSIZE 128
#define UART_LINE_BUFFER_SIZE 32

#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 20)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 32)

#undef UART_LEDS

#define HWRND_PIN 7 /* ADC7 */

//Festlegen der PORT-Nummer fuer das Keypad
#define COLL1 0
#define COLL2 1
#define COLL3 2
#define COLL4 3
#define ROW1 4
#define ROW2 5
#define ROW3 6
#define ROW4 7

//Festlegen der PORTS fuer das Keypad
#define PORT_ROWS PORTD
#define DDR_ROWS DDRD
#define PORT_COLLS PORTB
#define DDR_COLLS DDRB
#define PIN_COLLS PINB

//Festlegen der PORTS für das LCD
#define PORT_LCD PORTA
#define DDR_LCD  DDA
#define DDRC_LCD DDRA
#define PIN_LCD  PINA

//Festlegen der Port Nummern für das LCD
/* for the small LCD
#define ENABLE_LCD 5
#define RS_LCD     4
*/
#define LCD_RS      7
#define LCD_RW      6
#define LCD_ENABLE1 5
#define LCD_ENABLE2 4

#define LCD_WIDTH 27
#define LCD_HEIGHT 4

//config for persistentCounter 2
#define PERCNT_BPC 50
#define PERCNT_POLICY_INC
#define PERCNT_RESETONERROR

#define GAMES

#define QPTINY_AUTOKEYING
//#define QPTINY_CRYPTO_OFF
//#define QPTINY_STREAMAUTH 1

#endif


