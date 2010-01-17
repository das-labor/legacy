#ifndef CONFIG_H
#define CONFIG_H

// util.[ch] defines
// #define F_CPU 16000000         // Oszillator-Frequenz in Hz

// Bootloader defines
//location of can Address in eeprom
#define EEPR_ADDR_NODE 0x00

//the node number this device is supposed to be
#define CAN_ADDR 0x31

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
#define SPI_REG_PIN_MCP_INT PIND
	

// can.[ch] defines
#define F_MCP F_CPU

#define PORT_SDO_CMD 21
#define PORT_SDO_DATA 22




// lap.[ch] defines


#endif
