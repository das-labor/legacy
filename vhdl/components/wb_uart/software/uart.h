#ifndef UART_H
#define UART_H

#define FCPU      50000000
#define UART_RXBUFSIZE 32

// 32 Bit
typedef unsigned int  uint32_t;
typedef signed   int   int32_t;

// 8 Bit
typedef unsigned char  uint8_t;
typedef signed   char   int8_t;

/***************************************************************************
 * UART
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

extern uart_t  *uart0;

void uart_init();
void uart_putchar(char c);
void uart_putstr(char *str);
char uart_getchar();

#endif // UART_H
