#ifndef _CONFIG_H
#define _CONFIG_H


// SPI

// SS Pin muss Ausgang sein wenn nicht als CS verwendet
#define MCP_CS        PB2
#define MCP_CS_BIT    2
#define MCP_CMD_PORT  PORTB
#define MCP_CS_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT


#define F_MCP F_CPU

/* eeprom settings */
#define EEP_MY_ADDR 0x00

#endif // ifndef CONFIG_H

