#ifndef _CONFIG_H
#define _CONFIG_H


// SS Pin muss Ausgang sein wenn nicht als SC verwendet

#define MC_MOSI    PB3
#define MC_SCK     PB5

#define MCP_CS_BIT    PB2
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

//#define CAN_INTERRUPT
//#define CAN_INT_NOBLOCK

#define   ENABLE_CAN_INT()    EIMSK |= _BV(INT2)
#define   DISABLE_CAN_INT()   EIMSK &= ~_BV(INT2)
#define   SETUP_CAN_INT()     EICRA &= ~_BV(ISC21)
#define   MCP_INT_VEC         INT2_vect

#define CAN_TX_BUFFER_SIZE 4
#define CAN_RX_BUFFER_SIZE 4

#define F_MCP F_CPU

// RGB LED Taster Küchenlicht
#define R_LED _BV(PC1)
#define G_LED _BV(PC2)
#define B_LED _BV(PC3)

/*
 * can ports
 */
#define LIGHTCANPORT 0x04
#define ALARMCANPORT 0x00
#define I2CTEMPCANPORT (0x10)

/*
 * button thresholds
 */
#define HOLD_THRESHOLD 24
#define CLICK_THRESHOLD 3

/* eeprom settings */
#define EEP_MY_ADDR 0x00

#endif //_CONFIG_H

