#ifndef _CONFIG_H
#define _CONFIG_H


// SPI
#define MC_MOSI    PB5
#define MC_CLK     PB7

// Pin PB2 muss Ausgang sein wenn nicht als SS verwendet
#define MCP_CS        PB4
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT


#define F_MCP F_CPU


//#define MEGA8

#ifdef MEGA8
#	define SPI_PORT PORTB
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB5
#	define SPI_PIN_MOSI PB3
#else
#	define SPI_PORT PORTB
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB7
#	define SPI_PIN_MOSI PB5
#endif



#define LED_GRUEN   _BV(PA2)
#define LED_ROT _BV(PA3)
#define LED_BLAU  _BV(PA4)



#endif // ifndef CONFIG_H
