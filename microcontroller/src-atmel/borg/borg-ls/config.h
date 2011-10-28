#define NUM_ROWS 8
#define NUM_COLS 96
#define NUM_PLANES 1
#define NUMPLANE 3

// ** programm.c  **
// for matrix
#define STREAMER_NUM 100

#define FEUER_S 30
#define FEUER_N 5
#define FEUER_DIV 47
#define FEUER_DELAY 30

#define SNAKE_DELAY 50

#define BORG_CAN

#define SCROLLTEXT_STRING_SIZE 128
//#define SCROLLTEXT_STRING_SIZE 34
//#define SCROLLTEXT_DEFAULT "</#Labor"

#define SCROLLTEXT_DEFAULT ">5+:<5|90>6:<6|78>:p10d50/#Labor#<5;>5|30<6;>6|40<6;p10+d50/# Borg#2d50-+/#Widerstand#ist#d50-b20p15#ZWECKLOS !"
#define SCROLL_X_SPEED 10
#define SCROLL_Y_SPEED 30
#define SCROLLTEXT_FONT font_uni53

// spi.[ch] defines
#define SPI_HARDWARE
#define SPI_PORT PORTB		//for slave select
#define SPI_PIN PINB		//for slave select
#define SPI_DDR DDRB

#define SPI_PIN_MOSI PB5
#define SPI_PIN_MISO PB6
#define SPI_PIN_SCK PB7
#define SPI_PIN_SS PB4		// for slave select

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_REG_PIN_MCP_INT PIND
#define SPI_PIN_MCP_INT PD2

// can.[ch] defines
#undef  CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt
#define F_MCP F_CPU

// PersistentCounter2 configuration
#define PERCNT_BPC 24

//enable time via can extensions
#define LAP_TIME_EXTENSION



