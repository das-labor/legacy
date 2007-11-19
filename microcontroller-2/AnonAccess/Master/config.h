#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

#define DEBUG uart

//#define ATMEGA128
#define ATMEGA644
//#define ATMEGA32

//c uart.[ch] defines
//#define UART_INTERRUPT 1
#define UART_BAUD_RATE 38400
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
#undef UART_LEDS

// i2c defines

#define I2C_EXTERNAL_PULLUP

#define HMAC_SHORTONLY

#define LITTLE_ENDIAN

#define TICKETDB_OFFSET 0
#define TICKETDB_SIZE 4096

#define FLMDB_OFFSET TICKETDB_SIZE
#define FLMDB_SIZE 4096

#define TICKET_TIMEOUT (1000LL*60*60*24*30*3) /* timeout value in ms */ /* 90 days */

#endif

