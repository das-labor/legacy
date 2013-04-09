#ifndef _CONFIG_H
#define _CONFIG_H

// SPI
#define MCP_CS_BIT    PB2
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

#define CAN_INTERRUPT

#define   ENABLE_CAN_INT()    GICR |= _BV(INT0)
#define   DISABLE_CAN_INT()   GICR &= ~_BV(INT0)
#define   SETUP_CAN_INT()     MCUCR |= _BV(ISC01)
#define   MCP_INT_VEC         INT0_vect

#define CAN_TX_BUFFER_SIZE 4
#define CAN_RX_BUFFER_SIZE 4

#define F_MCP F_CPU

// Ausgänge LED String
#define RGB_DATA _BV(PD6)
#define RGB_CLK  _BV(PD7)
#define MAXCOLORVALUE (0x0FFF)
#define RGB_PORT PORTD
#define RGB_DDR  DDRD
#define BITSPERLAMP 12
#define LAMPS 50

// Taster LED
#define R_LED _BV(PD1)
#define B_LED _BV(PD3)
#define G_LED _BV(PD4)
#define PORT_LED PORTD
#define DDR_LED DDRD

// Taster Eingänge
#define NUM_INPUTS 2
#define I_PIN_0 PINB
#define I_BV_0  _BV(PB0) // 
#define I_PIN_1 PINB
#define I_BV_1  _BV(PB1) // 

/* eeprom settings */
#define EEP_MY_ADDR 0x00
#define EEP_MOTION_TRESH 0x01

// TWI Adresse des Temperatursensors
#define I2C_ADRESSE_DS1631 0x9e

#endif // ifndef CONFIG_H

