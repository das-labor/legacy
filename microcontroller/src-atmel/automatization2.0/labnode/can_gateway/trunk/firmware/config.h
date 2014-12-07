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


//#define BUSPOWER_SWITCH
//#define POWER_MEASUREMENT
//#define LED_SUPPORT
//#define LED_SUPPORT_MCP

// uart.[ch] defines
#define UART_INTERRUPT
#define UART_BAUD_RATE 57600
#define UART_RXBUFSIZE 50
#define UART_TXBUFSIZE 50
//#define UART_LEDS


#define LABNODE

#define DDR_LED DDRB
#define PORT_LED PORTB
#define BIT_LED PB0


// can.[ch] defines
#define CAN_RAW

#define F_MCP F_CPU

#define MCP_CS_BIT    PB4
#define MCP_CS_PORT   PORTB

#define SPI_REG_PIN_MCP_INT  PINB
#define SPI_PIN_MCP_INT      PB2


//#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt

#define   ENABLE_CAN_INT()    GICR |= _BV(INT2)
#define   DISABLE_CAN_INT()   GICR &= ~_BV(INT2)
#define   SETUP_CAN_INT()     MCUCSR &= ~_BV(ISC2)
#define   MCP_INT_VEC         INT2_vect


// lap.[ch] defines


#endif
