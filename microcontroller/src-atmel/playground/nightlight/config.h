#ifndef _CONFIG_H
#define _CONFIG_H


// SPI


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
