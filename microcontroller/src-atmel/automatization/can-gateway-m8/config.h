#ifndef CONFIG_H
#define CONFIG_H

// util.[ch] defines
// #define F_CPU 16000000         // Oszillator-Frequenz in Hz

// uart.[ch] defines
#define UART_INTERRUPT
#define UART_BAUD_RATE 57600
#define UART_RXBUFSIZE 50
#define UART_TXBUFSIZE 50
#undef  UART_LEDS
#define UART_2_STOPBITS

// spi.[ch] defines
#define SPI_HARDWARE
#define SPI_PORT PORTB		//for slave select
#define SPI_PIN PINB		//for slave select
#define SPI_DDR DDRB

#define SPI_PIN_MOSI PB3
#define SPI_PIN_MISO PB4
#define SPI_PIN_SCK PB5
#define SPI_PIN_SS PB2		// for slave select

//interrupt pin of MCP2515 for non interrupt drive can
#define SPI_PIN_MCP_INT PD2
	

// can.[ch] defines
#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_HANDLEERROR
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt
#define F_MCP 16000000


// lap.[ch] defines


#endif
