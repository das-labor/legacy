#ifndef SPIKEHW_H
#define SPIKEHW_H

#define PROMSTART 0x00000000
#define RAMSTART  0x00000800
#define RAMSIZE   (2*1024)
#define RAMEND    (RAMSTART + RAMSIZE)

#define SRAM_START 0xB0000000
#define SRAM_SIZE  0x00080000

#define FCPU         3125000
//(50000000/16)
/// #define FCPU      500000

#define UART_RXBUFSIZE 32

// 32 Bit
typedef unsigned int  uint32_t;
typedef signed   int   int32_t;

// 8 Bit
typedef unsigned char  uint8_t;
typedef signed   char   int8_t;

/***************************************************************************
 * SPARC Interrupt & PowerDown 
 */
void irq_enable();
void irq_disable();
void irq_mask();
void halt();
void jump(uint32_t addr);

void sleep();
void tic_init();


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
	volatile uint32_t divisor;  // 16 bit divisor (Fclk / Baud)
	volatile uint32_t databuf;  // 1 byte rx buffer
} uart_t;

/***************************************************************************
 * Spike peripheral components
 */
extern volatile uart_t  *uart0;
extern volatile timer_t *timer0;
extern volatile gpio_t  *gpio0;

void uart_init();
void uart_putchar(char c);
void uart_putstr(char *str);
char uart_getchar();

#endif // SPIKEHW_H
