#include "uart.h"

uart_t  *uart0  =  (uart_t *)0xF0010000;  // Or wherever wb_uart is mounted

volatile static char rxbuf[UART_RXBUFSIZE];
volatile static char *volatile rxhead, *volatile rxtail;

/***************************************************************************
 * IRQ handling
 */
void irq_handler(uint32_t irl)
{
	int diff;

	switch (irl) {
	case 10:                                              /* uart0 rx */
		diff = rxhead - rxtail;
		if (diff < 0) diff += UART_RXBUFSIZE;
		if (diff < UART_RXBUFSIZE -1) {
			// buffer not full
			*rxhead = uart0->databuf;
			if (++rxhead == (rxbuf + UART_RXBUFSIZE)) rxhead = rxbuf;
		} else {
			uart0->databuf;   // read buffer to clear IRQ
		}
		break;
	};

	return;
}

/***************************************************************************
 * UART Functions
 */
void uart_init()
{
	// Setup Divisor register (Fclk / Baud)
	uart0->divisor = (FCPU/115200);
	
	// Initialize ring buffer
	rxhead = rxtail = rxbuf;

	// activate RX IRQ
	uart0->ucr = UART_RXIRQEN;
}

char  uart_getchar()
{
	char val;

	while( rxhead == rxtail )
		halt();

	val = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) rxtail = rxbuf;

	return val;
}

void uart_putchar(char c)
{
	while ( uart0->ucr & UART_TX_BUSY ) {
		uart0->ucr = UART_RXIRQEN | UART_TXIRQEN;
		halt();
	}
	
	uart0->databuf = c;
	uart0->ucr = UART_RXIRQEN;
}

void uart_putstr(char *str)
{
	char *c = str;
	while(*c) {
		uart_putchar(*c);
		c++;
	}
}

