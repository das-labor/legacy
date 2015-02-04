#ifndef _CONFIG_H
#define _CONFIG_H


// SPI
#define MCP_CS_BIT   PB4
#define MCP_CS_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

#define CAN_INTERRUPT

#define MCP_INT_VEC INT0_vect
#define ENABLE_CAN_INT()    GIMSK |= _BV(INT0)
#define DISABLE_CAN_INT()   GIMSK &= ~_BV(INT0)
#define SETUP_CAN_INT()     MCUCR |= _BV(ISC01)

#define CAN_RX_BUFFER_SIZE 4
#define CAN_TX_BUFFER_SIZE 4

#define F_MCP F_CPU

#define OPTIMISED_LAP

//#define TESTBOARD

#ifdef TESTBOARD
#define LED_ROT   _BV(PA3)
#define LED_GRUEN _BV(PA2)
#define LED_BLAU  _BV(PA4)
#else
#define LED_ROT   _BV(PA2)
#define LED_GRUEN _BV(PA3)
#define LED_BLAU  _BV(PA4)
#endif

#define PORT_LED   PORTA

#define NUM_INPUTS 7

#define PIN_HAUPTSCHALTER  (_BV(PA0))
#define PIN_POWER_OK       (_BV(PD7))
#define PIN_TASTER_LOUNGE  (_BV(PB2))
#define PIN_TASTER_VORTRAG (_BV(PD3))
#define PIN_RCD_SERVER     (_BV(PA1))
#define PIN_RCD_POWER      (_BV(PC2))
#define PIN_RCD_LICHT      (_BV(PD6))


#endif // ifndef CONFIG_H

