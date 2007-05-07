#include "spike_hw.h"

volatile timer_t *timer0 = (volatile timer_t *) 0x80000000;
volatile uart_t  *uart0  = (volatile uart_t *)  0x80001000;
volatile gpio_t  *gpio0  = (volatile gpio_t *)  0x80002000;

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
	case 0:                                              /* uart0 tx */
		break;
	case 1:                                              /* uart0 rx */
		diff = rxhead - rxtail;
		if (diff < 0) 
			diff += UART_RXBUFSIZE;
		if (diff < UART_RXBUFSIZE -1) {
			// buffer not full
			*rxhead = uart0->databuf;
			if (++rxhead == (rxbuf + UART_RXBUFSIZE)) 
				rxhead = rxbuf;
		} else {
			uart0->databuf;   // read buffer to clear IRQ
		}
		break;
	case 2:                                   /* timer0.0 (system tic) */
		tcr = timer0->tcr0;  // reset trig0
		msec++;
		break;
	case 3:                                               /* timer0.1 */
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
	uart0->divisor = 10; // (FCPU/57600);
	
	// Initialize ring buffer
	rxhead = rxtail = rxbuf;

	// activate RX IRQ
	uart0->ucr = UART_RXIRQEN;
}

char  uart_getchar()
{
	char val;
	while ( rxhead == rxtail )
		uart0->ucr = UART_RXIRQEN;

	val = *rxtail;
	if (++rxtail == (rxbuf + UART_RXBUFSIZE)) 
		rxtail = rxbuf;

	return val;
}

void uart_putchar(char c)
{
	while (uart0->ucr & UART_TX_BUSY) {
		//uart0->ucr = UART_RXIRQEN; // | UART_TXIRQEN;
		//halt();
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

