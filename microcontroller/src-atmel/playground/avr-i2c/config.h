#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

#define DEBUG uart

//c uart.[ch] defines
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 38400
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
#undef UART_LEDS

// i2c defines

//#define I2C_EXTERNAL_PULLUP

#endif
