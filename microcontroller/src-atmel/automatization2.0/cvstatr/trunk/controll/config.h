#ifndef _CONFIG_H
#define _CONFIG_H


// SPI

#define MCP_CS        PB2
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT


#define F_MCP F_CPU

#define I2C_ADRESSE_SENSOR 0x96
#define EEPROM_CAN_ADDR 0x00


#define KLINGEL	_BV(PD6)
#define STANDBY	_BV(PD5)
#define TUER	_BV(PD7)
#define SCHLOSS _BV(PD0) //XXX

#define SIGNAL_PIN	(&(PIND))

#endif // ifndef CONFIG_H

