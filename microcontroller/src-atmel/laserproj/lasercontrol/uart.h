#ifndef UART_H
#define UART_H

#include <inttypes.h>
#include <avr/pgmspace.h>
#include "config.h"

void uart_init();

void uart_putc(unsigned char c);
void uart_putstr(char * str);
void uart_putstr_P(PGM_P str);
void uart_hexdump(unsigned char *buf, int len);

char uart_getc();
char uart_getc_nb(char *c);		// returns 1 on success

//get one Cariage return terminated line
//echo charakters back on Uart
//returns buffer with zero terminated line on success, 0 pointer otherwise
char * uart_getline_nb();

char uart_rxbuffer_notempty(void);	//returns 1 if queue is not empty, otherwise 0
#if UART_RXBUFSIZE < 255
uint8_t uart_getrxqueuelength(void); //returns queue length
#else
uint16_t uart_getrxqueuelength(void); //returns queue length
#endif

#endif
