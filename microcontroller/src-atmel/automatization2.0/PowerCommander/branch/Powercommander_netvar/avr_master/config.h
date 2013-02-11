#ifndef _CONFIG_H
#define _CONFIG_H


// SPI

// SS Pin muss Ausgang sein wenn nicht als CS verwendet
#define MCP_CS_BIT   PB4
#define MCP_CS_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
#define CAN_INTERRUPT
#define CAN_RX_BUFFER_SIZE 4
#define CAN_TX_BUFFER_SIZE 4
#define MCP_INT_VEC INT0_vect
#define ENABLE_CAN_INT()    GIMSK |= _BV(INT0)
#define DISABLE_CAN_INT()   GIMSK &= ~_BV(INT0)
#define SETUP_CAN_INT()     MCUCR |= _BV(ISC01)

#define F_MCP F_CPU


// TWI_ADDRESS
#include "../include/PowerCommander.h"


#define LED_ROT   _BV(PA3)
#define LED_GRUEN _BV(PA2)
#define LED_BLAU  _BV(PA4)

#define TESTBOARD

#define NUM_INPUTS 7

#define HAUPTSCHALTER	(_BV(PA0))
#define POWER_OK	(_BV(PC2))
#define TASTER_LOUNGE	(_BV(PB2))
#define TASTER_VORTRAG	(_BV(PD3))
#define RCD_SERVER	(_BV(PD6))
#define RCD_POWER	(_BV(PD7))
#define RCD_LICHT	(_BV(PA1))


#endif // ifndef CONFIG_H
