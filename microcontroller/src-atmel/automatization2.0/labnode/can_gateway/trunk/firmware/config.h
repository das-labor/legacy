#ifndef CONFIG_H
#define CONFIG_H

// util.[ch] defines
// #define F_CPU 16000000         // Oszillator-Frequenz in Hz

// uart.[ch] defines
#define UART_INTERRUPT
#define UART_BAUD_RATE 57600
#define UART_RXBUFSIZE 50
#define UART_TXBUFSIZE 50
#define UART_LINE_BUFFER_SIZE 40
//#define UART_LEDS




//interrupt pin of MCP2515 for non interrupt drive can
#define SPI_PIN_MCP_INT PD2
	

// can.[ch] defines
#define F_MCP F_CPU

#define MCP_CMD_PORT PORTB
#define MCP_CS       PB4

#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt

#define ENABLE_CAN_INT()   GICR |= (1<<INT2)
#define MCP_INT_VEC        INT2_vect



// lap.[ch] defines


#endif
