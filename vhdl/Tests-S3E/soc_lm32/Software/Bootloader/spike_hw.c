#include "spike_hw.h"

timer_t *timer0 = (timer_t *)0x80000000;
uart_t  *uart0  =  (uart_t *)0x80000100;
gpio_t  *gpio0  =  (gpio_t *)0x80000200;

uint32_t msec = 0;

volatile static char rxbuf[UART_RXBUFSIZE];
volatile static char *volatile rxhead, *volatile rxtail;

/***************************************************************************
 * IRQ handling
 */
void irq_handler(uint32_t irl)
{
	uint32_t tcr;
	int diff;

	switch (irl) {
	case 12:                                 /* timer0.0 (system tic) */
		tcr = timer0->tcr0;  // reset trig0
		msec++;
		break;
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
	case 6:                                               /* timer0.1 */
		break;
	case 4:                                               /* uart0 tx */
		break;
	};

	return;
}

/***************************************************************************
 * General utility functions
 */
void sleep(int msec)
{
	uint32_t tcr;

	// Use timer0.1
	timer0->compare1 = (FCPU/1000)*msec;
	timer0->counter1 = 0;
	timer0->tcr1 = TIMER_EN | TIMER_IRQEN;

	do {
		halt();
		tcr = timer0->tcr1;
	} while ( ! (tcr & TIMER_TRIG) );
}

void tic_init()
{
	// Setup timer0.0
	timer0->compare0 = (FCPU/1000);
	timer0->counter0 = 0;
	timer0->tcr0     = TIMER_EN | TIMER_AR | TIMER_IRQEN;
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

