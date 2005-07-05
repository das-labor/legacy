#ifndef UART_H
#define UART_H

void uart_init();
void uart_putc(char c);
char uart_getc();
void uart_putstr(char * str);

#endif
