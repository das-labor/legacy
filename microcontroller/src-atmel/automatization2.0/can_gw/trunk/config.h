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


// uart.[ch] defines
#define UART_INTERRUPT
#define UART_BAUD_RATE 230400
#define UART_RXBUFSIZE 50
#define UART_TXBUFSIZE 50
//#define UART_LEDS



// can.[ch] defines
#define CAN_RAW

#define F_MCP 16000000

#define MCP_CS_PORT PORTB
#define MCP_CS_BIT    PB2


#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2


#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 16	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 16	//only used for Interrupt

#define ENABLE_CAN_INT()  GICR |= _BV(INT0)
#define DISABLE_CAN_INT() GICR &= ~(_BV(INT0))
#define SETUP_CAN_INT()   MCUCR |= _BV(ISC01)

#define MCP_INT_VEC        INT0_vect



// lap.[ch] defines


#endif
