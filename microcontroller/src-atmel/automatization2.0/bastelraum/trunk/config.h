#ifndef _CONFIG_H
#define _CONFIG_H


// SPI

// SS Pin muss Ausgang sein wenn nicht als SC verwendet
#define MCP_CS        PB4
#define MCP_CS_BIT    4
#define MCP_CMD_PORT  PORTB
#define MCP_CS_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PINB
#define SPI_PIN_MCP_INT      PB2
//#define CAN_INTERRUPT


#define F_MCP F_CPU


#define LED_GRUEN   _BV(PA2)
#define LED_ROT _BV(PA3)
#define LED_BLAU  _BV(PA4)


#define TWI_ADDRESS 42

#define PORT_BASTEL 0x01

/* eeprom settings */
#define EEP_MY_ADDR 0x00
#define EEP_MOTION_TRESH 0x01


#endif // ifndef CONFIG_H

