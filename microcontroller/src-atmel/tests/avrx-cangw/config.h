#ifndef CONFIG_H 
#define CONFIG_H 

#define F_CPU 16000000L
#define F_MCP F_CPU
#define CPUCLK F_CPU

// AvrX
#define TICKRATE   2500
#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)
#define TMC8_CK256 (1<<CS02)

// Serial.[ch] defines
#define BAUDRATE 57600L     // Debug monitor baudrate
#define UBRR_INIT (F_CPU/(16*BAUDRATE)-1)
#define TX0_BUFSZ 48
#define RX0_BUFSZ 10

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
#define SPI_REG_PIN_MCP_INT PIND
#define SPI_PIN_MCP_INT PD2
	
// Can.[ch] defines
#define TX_SIZE 4


#endif
