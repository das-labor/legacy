#ifndef _CONFIG_H
#define _CONFIG_H


// SPI
#define MC_MOSI    PB3
#define MC_CLK     PB5

// Pin PB2 muss Ausgang sein wenn nicht als SS verwendet
#define MCP_CS        PB2
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT


#define F_MCP F_CPU

// switch off with hauptschalter
//#define LABOR_MOOD


#endif // ifndef CONFIG_H
