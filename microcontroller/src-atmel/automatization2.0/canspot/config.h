#ifndef CONFIG_H
#define CONFIG_H

#define MCP_CMD_PORT PORTB		//for slave select
#define MCP_CS PB4		// for slave select


// can.[ch] defines
#define F_MCP F_CPU

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_REG_PIN_MCP_INT PIND
#define SPI_PIN_MCP_INT PD2

#endif

