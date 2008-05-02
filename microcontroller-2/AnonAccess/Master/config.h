#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

#define DEBUG uart

//#define ATMEGA128
//#define ATMEGA644
//#define ATMEGA32

//c uart.[ch] defines
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 9600
#define UART_RXBUFSIZE 32
#define UART_TXBUFSIZE 32
#define UART_LINE_BUFFER_SIZE 40

#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 24)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 30)

#undef UART_LEDS

/* i2c defines */
#define I2C_EXTERNAL_PULLUP

#define BOOTSTRAP_ACCOUNTS 200 /* accounts given "for free" */
#define NO_ANON_ADMINS 1

#define EEPROM_SEC_INIT_RWS 2 /*129*/

/* tamperdetection defines */
//#define TAMPER_DETECTION
#define TAMPER_SECT 1
#define TAMPER_PORT PORTB
#define TAMPER_DDR DDRB
#define TAMPER_PIN 2
#define TAMPER_PCMSK PCMSK1
#define TAMPER_PCINT PCINT1_vect

#define HMAC_SHORTONLY

#define LITTLE_ENDIAN

#define KEYMIG_OFFSET 0
#define KEYMIG_SIZE (9*32)

#define TICKETDB_OFFSET (KEYMIG_OFFSET + KEYMIG_SIZE)
#define TICKETDB_SIZE 4096

#define FLMDB_OFFSET (TICKETDB_OFFSET + TICKETDB_SIZE)
#define FLMDB_SIZE 4096

#define TICKET_TIMEOUT (1000LL*60*60*24*30*3) /* timeout value in ms */ /* 90 days */

#define PRINTER_ADDR 0x00


#endif

