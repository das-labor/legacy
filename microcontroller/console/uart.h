#ifndef UART_H
#define UART_H

#include <avr/pgmspace.h>

void uart_init();
void uart_putc(char c);
char uart_getc();
void uart_putstr(char * str);

char * uart_getline_nb();
void uart_putstr_P(PGM_P str);

#endif
