#ifndef SPIKEHW_H
#define SPIKEHW_H

#define PROMSTART 0x00000000
#define BRAMSTART 0x10000000
#define RAMSTART  0x80000000
#define RAMSIZE   (64*1024*1024)
#define RAMEND    (RAMSTART + RAMSIZE)

#define FCPU      50000000
/// #define FCPU      500000

#define UART_RXBUFSIZE 32

// 32 Bit
typedef unsigned int  uint32_t;
typedef signed   int   int32_t;

// 8 Bit
typedef unsigned char  uint8_t;
typedef signed   char   int8_t;

/***************************************************************************
 * GPIO0
 */
typedef struct {
	volatile uint32_t iport;
	volatile uint32_t oport;
} gpio_t;


/***************************************************************************
 * TIMER0
 */
#define TIMER_EN     0x08
#define TIMER_AR     0x04
#define TIMER_IRQEN  0x02
#define TIMER_TRIG   0x01

typedef struct {
	volatile uint32_t tcr0;
	volatile uint32_t compare0;
	volatile uint32_t counter0;
	volatile uint32_t tcr1;
	volatile uint32_t compare1;
	volatile uint32_t counter1;

} timer_t;

/***************************************************************************
 * UART0
 */
#define UART_RX_FULL 1
#define UART_TX_BUSY 2
#define UART_RXIRQEN 4
#define UART_TXIRQEN 8

typedef struct {
	volatile uint32_t ucr;      // status register
	volatile uint32_t databuf;  // 1 byte rx buffer
} uart_t;

/***************************************************************************
 * Spike peripheral components
 */
extern uart_t  *uart0;
extern timer_t *timer0;
extern gpio_t  *gpio0;

/***************************************************************************
 * Routines
 */
void irq_enable();
void irq_disable();
void halt();
void jump(uint32_t addr);

/* TIMER */
void sleep(int msec);
void tic_init();
void tic_disable();

/* UART */
void uart_init();
void uart_putchar(char c);
void uart_putstr(char *str);
char uart_getchar();

#endif // SPIKEHW_H
