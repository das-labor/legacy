#ifndef UART_H
#define UART_H

/**
 * UART Library
 *
 * #define F_CPU 16000000         // Oszillator-Frequenz in Hz
 * #define UART_INTERRUPT 1
 * #define UART_BAUD_RATE 19200
 * #define UART_RXBUFSIZE 16
 * #define UART_TXBUFSIZE 16
 * #define UART_LINE_BUFFER_SIZE 40
 * #define UART_LEDS             // LED1 and LED2 toggle on tx and rx interrupt
 *
 */

#include "config.h"

#include <inttypes.h>
#include <avr/pgmspace.h>

#define BOOTLOADER_SECTION    __attribute__ ((section (".bootloader")))

void uart_init() BOOTLOADER_SECTION;

void uart_putc(char c) BOOTLOADER_SECTION;
void uart_putstr(char * str) BOOTLOADER_SECTION;
void uart_putstr_P(PGM_P str) BOOTLOADER_SECTION;
void uart_hexdump(char *buf, int len) BOOTLOADER_SECTION;

char uart_getc() BOOTLOADER_SECTION;
char uart_getc_nb(char *c) BOOTLOADER_SECTION;		// returns 1 on success

//get one Cariage return terminated line
//echo charakters back on Uart
//returns buffer with zero terminated line on success, 0 pointer otherwise
char * uart_getline_nb() BOOTLOADER_SECTION;

uint16_t getword() BOOTLOADER_SECTION;


#endif
