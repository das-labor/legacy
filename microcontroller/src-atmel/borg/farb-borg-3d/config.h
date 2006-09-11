//#define UART_INTERRUPT 1
#define UART_BAUD_RATE 57600
#define UART_RXBUFSIZE 16
#define UART_TXBUFSIZE 16
#define UART_LINE_BUFFER_SIZE 40
#define UART_LEDS 0

#define UART_ESCAPE 0x42
#define UART_SOI 1
#define UART_SOF 2
#define UART_SS  3

#define BORG_CAN

#define SCROLLTEXT_STRING_SIZE 128
#define SCROLLTEXT_DEFAULT "<25/#Borg 3D"

#ifndef AVR
#	define AVR
#endif

// spi.[ch] defines
#define SPI_HARDWARE
#define SPI_PORT PORTB          //for slave select
#define SPI_PIN PINB            //for slave select
#define SPI_DDR DDRB

#define SPI_PIN_MOSI PB5
#define SPI_PIN_MISO PB6
#define SPI_PIN_SCK PB7
#define SPI_PIN_SS PB4          // for slave select

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_REG_PIN_MCP_INT PIND
#define SPI_PIN_MCP_INT PD2

// can.[ch] defines
#undef  CAN_INTERRUPT           //set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 2    //only used for Interrupt
#define CAN_TX_BUFFER_SIZE 2    //only used for Interrupt
#define F_MCP F_CPU
