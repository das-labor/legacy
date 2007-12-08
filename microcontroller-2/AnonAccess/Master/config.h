#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

#define DEBUG uart

//#define ATMEGA128
#define ATMEGA644
//#define ATMEGA32

//c uart.[ch] defines
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 38400
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 4)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 12)
#undef UART_LEDS

// i2c defines

#define I2C_EXTERNAL_PULLUP

#define HMAC_SHORTONLY

#define LITTLE_ENDIAN

#define KEYMIG_OFFSET 0
#define KEYMIG_SIZE (6*32)

#define TICKETDB_OFFSET (KEYMIG_OFFSET + KEYMIG_SIZE)
#define TICKETDB_SIZE 4096

#define FLMDB_OFFSET (TICKETDB_OFFSET + TICKETDB_SIZE)
#define FLMDB_SIZE 4096

#define TICKET_TIMEOUT (1000LL*60*60*24*30*3) /* timeout value in ms */ /* 90 days */

#endif

