//Art des Atmels und dessen Takt
#define F_CPU 16000000ul

/* set by Makefile */
//#define ATMEGA128
//#define ATMEGA644
//#define ATMEGA32


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Definitionen fuer SPI
#define SPI_HARDWARE 1
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_DDR DDRB
#define SPI_PIN_MOSI PB5
#define SPI_PIN_MISO PB6
#define SPI_PIN_SCK PB7
#define SPI_PIN_SS PB4

#define DEBUG uart

#define ERROR_HANDLING

//c uart.[ch] defines
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 9600
#define UART_RXBUFSIZE 40
#define UART_TXBUFSIZE 40
#define UART_LINE_BUFFER_SIZE 32

#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 20)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 32)

#undef UART_LEDS

#define HWRND_PIN 7 /* ADC7 */

//Festlegen der PORT-Nummer fuer das Keypad
#define COLL1 0
#define COLL2 1
#define COLL3 2
#define COLL4 3
#define ROW1 4
#define ROW2 5
#define ROW3 6
#define ROW4 7

//Festlegen der PORTS fuer das Keypad
#define PORT_ROWS PORTD
#define DDR_ROWS DDRD
#define PORT_COLLS PORTB
#define DDR_COLLS DDRB
#define PIN_COLLS PINB

//Festlegen der PORTS für das LCD
#define PORT_LCD PORTA
#define DDR_LCD DDA
#define DDRC_LCD DDRA
#define PIN_LCD PINA

//Festlegen der Port Nummern für das LCD
#define ENABLE_LCD 5
#define RS_LCD 4

#define LCD_WIDTH 20
#define LCD_HEIGHT 4

//config for persistentCounter 2
#define PERCNT_BPC 50
#define PERCNT_POLICY_INC
#define PERCNT_RESETONERROR




