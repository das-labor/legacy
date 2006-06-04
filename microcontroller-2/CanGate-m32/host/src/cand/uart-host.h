#ifndef UART_HOST_H
#define UART_HOST_H

/**
 * UART Library
 *
 * #define UART_BAUD_RATE 19200
 *
 */

#include "config.h"

#include <sys/select.h>

void uart_init(char *sport);

void uart_putc(char c);
void uart_putstr(char * str);

char uart_getc();
char uart_getc_nb(char *c);		// returns 1 on success

/* UART-Host specific */
int uart_fd;
#endif
