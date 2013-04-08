#ifndef CONFIG_H
#define CONFIG_H

// Bootloader defines
#define EEPR_ADDR_NODE 0x00

//#define TOGGLE_MCP_LED
#define TOGGLE_PORT_LED
#define PORT_LED PORTC
#define BIT_LED      2

// spi.[ch] defines
#define SPI_HARDWARE
#define SPI_PORT PORTD

#define SPI_PIN_MOSI 5
#define SPI_PIN_MISO 6
#define SPI_PIN_SCK 7
#define SPI_PIN_SS 4		// for slave select

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_PIN_MCP_INT          1
#define SPI_REG_PIN_MCP_INT  PORTD


// can.[ch] defines
#define F_MCP ((F_CPU) / 2)

#define PORT_SDO_CMD 21
#define PORT_SDO_DATA 22

#define JUMP_OPCODE "jmp"

#define XMEGA

#endif
