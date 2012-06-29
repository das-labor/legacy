#ifndef _CONFIG_H
#define _CONFIG_H


// SPI

// SS Pin muss Ausgang sein wenn nicht als CS verwendet
#define MCP_CS_BIT   PB4
#define MCP_CS_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT


#define F_MCP F_CPU


// TWI_ADDRESS
#include "../include/PowerCommander.h"


#define LED_GRUEN   _BV(PA2)
#define LED_ROT _BV(PA3)
#define LED_BLAU  _BV(PA4)



#endif // ifndef CONFIG_H
