#ifndef CONFIG_H
#define CONFIG_H

// Bootloader defines
#define EEPR_ADDR_NODE 0x00

//#define TOGGLE_MCP_LED
//#define TOGGLE_PORT_LED
#define PORT_LED PORTB
#define DDR_LED DDRB
#define BIT_LED  PB0

// spi.[ch] defines
#define SPI_HARDWARE
#define SPI_PORT PORTB		//for slave select

#define SPI_PIN_MOSI 5
#define SPI_PIN_MISO 6
#define SPI_PIN_SCK 7
#define SPI_PIN_SS 4		// for slave select

//interrupt pin of MCP2515 for non interrupt drive can
#define SPI_PIN_MCP_INT          2
#define SPI_REG_PIN_MCP_INT  PORTD
	

// can.[ch] defines
#define F_MCP F_CPU

#define PORT_SDO_CMD 21
#define PORT_SDO_DATA 22

#define JUMP_OPCODE "jmp"

#define XMEGA

#endif
