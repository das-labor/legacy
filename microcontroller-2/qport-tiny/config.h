#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         /* Oszillator-Frequenz in Hz */

#define DEBUG uart

/*c uart.[ch] defines */
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 9600
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 4)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 12)

#undef UART_LEDS

#define HMAC_SHORTONLY
#define QPTINY_AUTOKEYING


#define DISPLAY_TIMEOUT 1000




#endif

