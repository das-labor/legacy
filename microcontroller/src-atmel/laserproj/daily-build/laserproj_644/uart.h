#ifndef UART_H
#define UART_H
	
	/* uart.[ch] defines */
	//#define UART_INTERRUPT 1
	#define UART_BAUD_RATE 115200L
	#define UART_RXBUFSIZE 64
	#define UART_TXBUFSIZE 64
	#define UART_LINE_BUFFER_SIZE 40

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <util.h>
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

#endif
