#ifndef CONFIG_H
#define CONFIG_H

#define SPI_SOFTWARE

#define MCP_CS_PORT PORTB		//for slave select
#define MCP_CS_BIT PB4		// for slave select

#define SPI_PIN_MOSI PB5
#define SPI_PIN_MISO PB6
#define SPI_PIN_SCK PB7
#define SPI_DDR DDRB
#define SPI_PIN PINB
#define SPI_PORT PORTB

// can.[ch] defines
#define F_MCP F_CPU

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_REG_PIN_MCP_INT PIND
#define SPI_PIN_MCP_INT PD2

#endif // CONFIG_H

