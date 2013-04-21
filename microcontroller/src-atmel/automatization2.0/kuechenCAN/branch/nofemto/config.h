#ifndef _CONFIG_H
#define _CONFIG_H

// SPI
#define MCP_CS_BIT    PB2
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

#define CAN_INTERRUPT
//#define CAN_INT_NOBLOCK

#define   ENABLE_CAN_INT()    GIMSK |= _BV(INT0)
#define   DISABLE_CAN_INT()   GIMSK &= ~_BV(INT0)
#define   SETUP_CAN_INT()     MCUCR |= _BV(ISC01)
#define   MCP_INT_VEC         INT0_vect

#define CAN_TX_BUFFER_SIZE 4
#define CAN_RX_BUFFER_SIZE 4

#define F_MCP F_CPU
#define MCP2510

#define NO_NETVAR

// RGB LED Taster Küchenlicht
#define R_LED _BV(PC1)
#define G_LED _BV(PC2)
#define B_LED _BV(PC3)
#define PORT_LED PORTD
#define DDR_LED DDRD

// Taster Eingänge
#define NUM_INPUTS 2
#define I_PIN_0 PINB
#define I_BV_0  _BV(PB1) //
#define I_PIN_1 PINC
#define I_BV_1  _BV(PC0) //


/* eeprom settings */
#define EEPROM_LAP_ADDR 0x00

// TWI Adresse des Temperatursensors
#define I2C_ADRESSE_DS1631 0x9e

#endif //_CONFIG_H

