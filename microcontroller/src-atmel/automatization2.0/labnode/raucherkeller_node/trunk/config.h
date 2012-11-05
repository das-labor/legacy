#ifndef _CONFIG_H
#define _CONFIG_H


// SPI
#define MC_MOSI    PB5
#define MC_SCK     PB7

// Pin PB2 muss Ausgang sein wenn nicht als SS verwendet
#define MCP_CS_BIT		PB4
#define MCP_CS_PORT		PORTB

#define SPI_REG_PIN_MCP_INT  PINB
#define SPI_PIN_MCP_INT      PB2
//#define CAN_INTERRUPT


#define F_MCP F_CPU


#define LED_GRUEN   _BV(PA2)
#define LED_ROT _BV(PA3)
#define LED_BLAU  _BV(PA4)


#define TWI_ADDRESS 42

#define PORT_BASTEL 0x01

#define NUM_CHANNELS 4

#endif // ifndef CONFIG_H

