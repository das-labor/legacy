#ifndef _CONFIG_H
#define _CONFIG_H


// SPI
#define MC_MOSI    PB3
#define MC_CLK     PB5

// LS20 config

#define CMD_DIR     DDRC
#define CMD_PORT    PORTC
#define LCD_CS      PC5
#define LCD_RESET   PC4
#define LCD_RS      PC3



#define MCP_CS        PB0
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2
//#define CAN_INTERRUPT
#define F_MCP F_CPU


#define MEGA8

#ifdef MEGA8
#	define SPI_PORT PORTB
#	define SPI_PIN_CS PB0
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB5
#	define SPI_PIN_MOSI PB3
#else
#	define SPI_PORT PORTB
#	define SPI_PIN_CS PB4
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB7
#	define SPI_PIN_MOSI PB5
#endif


/* ATMega32 / 8*/
/*
#define MCP_INT_VEC INT0_vect
#define MCP_INT_MASK INT0
#define MCP_INT_REG GICR
*/
/* ATMega 644 / modern atmega */

#define MCP_INT_VEC INT0_vect
#define MCP_INT_MASK INT0
#define MCP_INT_REG EIMSK




#endif // ifndef CONFIG_H
