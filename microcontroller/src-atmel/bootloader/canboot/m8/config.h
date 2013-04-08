#ifndef CONFIG_H
#define CONFIG_H

// Bootloader defines
#define EEPR_ADDR_NODE 0x00

#define TOGGLE_MCP_LED
//#define TOGGLE_PORT_LED
//#define PORT_LED PORTB
//#define DDR_LED DDRB
//#define BIT_LED  PB0

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
#define SPI_PIN_MCP_INT PD2        //int0 pin
#define SPI_REG_PIN_MCP_INT PIND   //int0 pin


// can.[ch] defines
#define F_MCP F_CPU

#define PORT_SDO_CMD 21
#define PORT_SDO_DATA 22

#define JUMP_OPCODE "rjmp"
//#define TURN_OFF_WATCHDOG

#endif
