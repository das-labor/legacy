#include "spike_hw.h"

volatile timer_t  *timer0 = (volatile timer_t *)  0x80000000;
volatile uart_t   *uart0  = (volatile uart_t *)   0x80001000;
volatile gpio_t   *gpio0  = (volatile gpio_t *)   0x80002000;
volatile uint32_t *sram0  = (volatile uint32_t *) 0xB0000000;

uint32_t msec = 0;

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
		//halt();
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
	uart0->ier = 0x00;  // Interrupt Enable Register
	uart0->lcr = 0x03;  // Line Control Register:    8N1
	uart0->mcr = 0x00;  // Modem Control Register


	// Setup Divisor register (Fclk / Baud)
	//uart0->div = (FCPU/(57600*16));
}

char uart_getchar()
{   
	while (! (uart0->lsr & UART_DR)) {
	}
	return uart0->rxtx;
}

void uart_putchar(char c)
{
	while (! (uart0->lsr & UART_THRE)) {
	}
	uart0->rxtx = c;
}

void uart_putstr(char *str)
{
	char *c = str;
	while(*c) {
		uart_putchar(*c);
		c++;
	}
}

