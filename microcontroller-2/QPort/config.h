#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

#define DEBUG uart

#define ERROR_HANDLING

//c uart.[ch] defines
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 9600
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 3)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 6)

#undef UART_LEDS
/*
#define UART_HWFLOWCONTROL
#define UART_RTS_PORT PORTA
#define UART_RTS_DDR DDRA
#define UART_CTS_PIN PINA
#define UART_CTS_DDR DDRA
#define UART_RTS_BIT 0
#define UART_CTS_BIT 1
*/

/* #define QP_SERVER   */
/* #define QP_TERMINAL */
#define QP_MSG_CHECK_HASH /* or MSG_CHECK_HMAC */
#define QP_XON_XOFF_SUPPORT
#define QP_HASHKEYS

#ifdef QP_TERMINAL
	#define DISPLAY
#endif

#endif

