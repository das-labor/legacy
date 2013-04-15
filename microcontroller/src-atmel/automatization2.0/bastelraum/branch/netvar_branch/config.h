#ifndef _CONFIG_H
#define _CONFIG_H

// SPI
#define MCP_CS_BIT    PB4
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PINB
#define SPI_PIN_MCP_INT      PB2

#define CAN_INTERRUPT
#define CAN_INT_NOBLOCK

#define   ENABLE_CAN_INT()    EIMSK |= _BV(INT2)
#define   DISABLE_CAN_INT()   EIMSK &= ~_BV(INT2)
#define   SETUP_CAN_INT()     EICRA &= ~_BV(ISC21)
#define   MCP_INT_VEC         INT2_vect

#define CAN_TX_BUFFER_SIZE 4
#define CAN_RX_BUFFER_SIZE 4

#define F_MCP F_CPU

// Ausgänge Schieberegister
#define SREG_PORT	PORTC
#define SREG_CK		_BV(PC4)
#define SREG_DATA	_BV(PC5)
#define SREG_STROBE	_BV(PA5)

// Hardware PWM Ausgänge
#define O_PWM00_REG (&(OCR2A))
#define O_PWM01_REG (&(OCR2B))
#define O_PWM02_REG (&(OCR1AL))
#define O_PWM03_REG (&(OCR1BL))

#define NUM_INPUTS 2
#define I_PIN_0 PINA
#define I_BV_0  _BV(PA7)
#define I_PIN_1 PINA
#define I_BV_1  _BV(PA4)

/* eeprom settings */
#define EEPROM_LAP_ADDR 0x00
#define EEP_MOTION_TRESH 0x01


#endif // ifndef CONFIG_H

