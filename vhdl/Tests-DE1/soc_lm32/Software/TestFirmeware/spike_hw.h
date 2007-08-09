#ifndef SPIKEHW_H
#define SPIKEHW_H

#define FCPU         25000000

// 32 Bit
typedef unsigned int  uint32_t;
typedef signed   int   int32_t;

// 8 Bit
typedef unsigned char  uint8_t;
typedef signed   char   int8_t;

// Assembler-Helper-Functions
extern void irq_enable();
extern void irq_disable();
extern void irq_mask();
extern void halt();
extern void jump(uint32_t addr);
extern uint32_t get_r0();
extern uint32_t get_sp();
extern uint32_t get_gp();
extern uint32_t get_cfg();
extern uint32_t get_cycles();
extern uint32_t test_sp(uint8_t* adr, uint32_t value);

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
#define UART_DR   0x01                    // Data Ready
#define UART_OE   0x02                    // Overrun Error
#define UART_PE   0x04                    // Parity Error
#define UART_FE   0x08                    // Parity Error
#define UART_BI   0x10                    // Break Interrupt
#define UART_THRE 0x20                    // Transmit Holf Reg Empty
#define UART_TEMT 0x40                    // Transmit 

typedef struct {
   volatile uint32_t rxtx;
   volatile uint32_t ier;
   volatile uint32_t iir;
   volatile uint32_t lcr;
   volatile uint32_t mcr;
   volatile uint32_t lsr;
   volatile uint32_t msr;
   volatile uint32_t div;
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

extern volatile timer_t  *timer0;
extern volatile uart_t   *uart0; 
extern volatile gpio_t   *gpio0; 
extern volatile uint32_t *sram0; 

#endif // SPIKEHW_H
