#ifndef _H_CONFIG
#define _H_CONFIG


// SPI
#define MC_MOSI    PB2
#define MC_CLK     PB1

#define MCP_CS        PB4
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PINE
#define SPI_PIN_MCP_INT      PE7
//#define CAN_INTERRUPT


#define F_MCP F_CPU




//#define BIG_DISPLAY

#define PORT_TOUCH PORTF
#define DDR_TOUCH DDRF


#ifdef BIG_DISPLAY
	#define MSK_YL  _BV(PF3)
	#define MSK_XH  _BV(PF2)
	#define MSK_YH  _BV(PF1)
	#define MSK_XL  _BV(PF0)
	
	#define TOUCH_Y_CHANNEL PF1 //Must be YH pin
	#define TOUCH_X_CHANNEL PF2 //Must be XH pin
#else
	#define MSK_YL  _BV(PF0)
	#define MSK_XH  _BV(PF1)
	#define MSK_YH  _BV(PF2)
	#define MSK_XL  _BV(PF3)
	
	#define TOUCH_Y_CHANNEL PF2
	#define TOUCH_X_CHANNEL PF1
#endif








#endif // ifndef _H_CONFIG

