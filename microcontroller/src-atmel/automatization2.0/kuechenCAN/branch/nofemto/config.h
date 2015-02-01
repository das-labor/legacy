#ifndef _CONFIG_H
#define _CONFIG_H

// SPI
#define MCP_CS_BIT    PB2
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

#define CAN_INTERRUPT
//#define CAN_INT_NOBLOCK

#define ENABLE_CAN_INT()    GIMSK |= _BV(INT0)
#define DISABLE_CAN_INT()   GIMSK &= ~_BV(INT0)
#define SETUP_CAN_INT()     MCUCR |= _BV(ISC01)
#define MCP_INT_VEC         INT0_vect

#define CAN_TX_BUFFER_SIZE 4
#define CAN_RX_BUFFER_SIZE 4

#define F_MCP F_CPU
#define MCP2510

#define OPTIMISED_LAP

#define NO_NETVAR
//#define NEW_PROTO

// RGB LED Taster Küchenlicht
#define RGBLED_R (_BV(PC1))
#define RGBLED_G (_BV(PC2))
#define RGBLED_B (_BV(PC3))
#define RGB_LED_PORT (&(PORTC))
#define RGB_LED_DDR DDRC

// Rote LEDs Taster
#define B_LED1 (_BV(PD5))
#define B_LED2 (_BV(PD6))
#define B_LED3 (_BV(PD7))
#define PORT_LED (&(PORTD))
#define DDR_LED DDRD

#define O_OUT00_PORT RGB_LED_PORT
#define O_OUT01_PORT RGB_LED_PORT
#define O_OUT02_PORT RGB_LED_PORT
#define O_OUT03_PORT RGB_LED_PORT
#define O_OUT04_PORT RGB_LED_PORT
#define O_OUT05_PORT RGB_LED_PORT

#define O_OUT00_BIT RGBLED_R
#define O_OUT01_BIT RGBLED_G
#define O_OUT02_BIT RGBLED_B
#define O_OUT03_BIT B_LED1
#define O_OUT04_BIT B_LED2
#define O_OUT05_BIT B_LED3
// Taster Eingänge
#define NUM_INPUTS 2
#define I_PIN_0 PINB
#define I_BV_0  _BV(PB1) //
#define I_PIN_1 PINC
#define I_BV_1  _BV(PC0) //


// TWI Adresse des Temperatursensors
#define I2C_ADRESSE_DS1631 0x92

#endif // _CONFIG_H

