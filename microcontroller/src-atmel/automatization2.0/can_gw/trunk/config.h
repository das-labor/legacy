#ifndef CONFIG_H
#define CONFIG_H


//firmware version defines
#define FW_VERSION_MAJOR 1
#define FW_VERSION_MINOR 1

//firmware svn revision is set via -D compiler option
#ifndef FW_SVNREVISION
	#define FW_SVNREVISION 0
#endif

//firmware id string is set via -D compiler option
#ifndef FW_IDSTRING
	#define FW_IDSTRING "unknown"
#endif


#define BUSPOWER_SWITCH
#define POWER_MEASUREMENT
#define LED_SUPPORT
//#define LED_SUPPORT_MCP

// uart.[ch] defines
#define UART_INTERRUPT
#define UART_BAUD_RATE 230400UL
#define UART_RXBUFSIZE 50
#define UART_TXBUFSIZE 50
//#define UART_LEDS

// can.[ch] defines

// if mcp2510 is used
//#define MCP2510

#define CAN_RAW

#define F_MCP 16000000UL

#define MCP_CS_PORT  PORTB
#define MCP_CS_BIT   PB2


//#define SPI_REG_PIN_MCP_INT PIND
//#define SPI_PIN_MCP_INT     PD2


#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_INT_NOBLOCK
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt

#define ENABLE_CAN_INT()  GICR |= _BV(INT0)
#define DISABLE_CAN_INT() GICR &= ~(_BV(INT0))
#define SETUP_CAN_INT()   MCUCR |= _BV(ISC01)
#define MCP_INT_VEC       INT0_vect


#endif // CONFIG_H

