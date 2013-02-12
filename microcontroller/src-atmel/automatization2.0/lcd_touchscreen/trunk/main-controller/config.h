#ifndef _H_CONFIG
#define _H_CONFIG


// SPI
#define MC_MOSI    PB2
#define MC_CLK     PB1

#define MCP_CS_BIT   PB4
#define MCP_CS_PORT  PORTB

#define SPI_REG_PIN_MCP_INT PINE
#define SPI_PIN_MCP_INT     PE7

#define CAN_INTERRUPT

#define   ENABLE_CAN_INT()    EIMSK |= _BV(INT7)
#define   DISABLE_CAN_INT()   EIMSK &= ~_BV(INT7)
#define   SETUP_CAN_INT()     EICRB |= _BV(ISC71)
#define   MCP_INT_VEC         INT7_vect


#define CAN_RX_BUFFER_SIZE 16
#define CAN_TX_BUFFER_SIZE 16

#define CAN_HANDLER_C

#define F_MCP F_CPU

// Display config

// Touchscreen

#define PORT_TOUCH PORTF
#define DDR_TOUCH DDRF

#include "../include/personal_config.h"

#if defined(HW_REV_1_1) || defined(HW_REV_2)
	#define MSK_YL  _BV(PF3)
	#define MSK_XH  _BV(PF2)
	#define MSK_YH  _BV(PF1)
	#define MSK_XL  _BV(PF0)
	
	#define TOUCH_Y_CHANNEL PF1 //Must be YH pin
	#define TOUCH_X_CHANNEL PF2 //Must be XH pin
#elif defined(HW_REV_1)
	#define MSK_YL  _BV(PF0)
	#define MSK_XH  _BV(PF1)
	#define MSK_YH  _BV(PF2)
	#define MSK_XL  _BV(PF3)
	
	#define TOUCH_Y_CHANNEL PF2
	#define TOUCH_X_CHANNEL PF1
#else
	#error please define your hardware revision!
	//HW_REV_1    Old board with 4 bit Bus LED-lit display
	//HW_REV_1_1  Old board with 8 bit Bus CCFL-lit display
	//HW_REV_2    New board with 8 bit Bus CCFL-lit display
#endif


// Display Controller Communication

#define PORT_HANDSHAKE  PORTC
#define DDR_HANDSHAKE   DDRC
#define PIN_HANDSHAKE   PINC

#define BIT_ATN PC7
#define BIT_ACK PC6



#define DDR_DC_DATA  DDRA
#define PIN_DC_DATA  PINA
#define PORT_DC_DATA PORTA


//Uart for communication with buttons
#define UART_BAUD_RATE 19200
#define USE_UART_1

#endif // ifndef _H_CONFIG

