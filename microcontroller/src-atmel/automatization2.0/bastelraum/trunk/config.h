#ifndef _CONFIG_H
#define _CONFIG_H

// SPI
#define MCP_CS_BIT    PB4
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PINB
#define SPI_PIN_MCP_INT      PB2

//#define CAN_INTERRUPT


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


/* eeprom settings */
#define EEPROM_LAP_ADDR 0x00
#define EEPROM_MOTION_TRESH 0x01


#endif // ifndef CONFIG_H

