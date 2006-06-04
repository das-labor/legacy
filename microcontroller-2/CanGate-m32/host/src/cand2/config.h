#ifndef CONFIG_H
#define CONFIG_H

// util.[ch] defines
// #define F_CPU 16000000         // Oszillator-Frequenz in Hz

// uart.[ch] defines
#define UART_BAUD_RATE 115200
#define UART_HOST

// spi.[ch] defines
#define SPI_HARDWARE 1
#define SPI_PORT PORTB		//for slave select
#define SPI_PIN PINB		//for slave select
#define SPI_DDR DDRB

#define SPI_PIN_MOSI PB5
#define SPI_PIN_MISO PB6
#define SPI_PIN_SCK PB7
#define SPI_PIN_SS PB4		// for slave select

//interrupt pin of MCP2515 for non interrupt drive can
#define SPI_PIN_MCP_INT PD2
	

// can.[ch] defines
#define CAN_INTERRUPT 1		//set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt


// lap.[ch] defines


#endif
