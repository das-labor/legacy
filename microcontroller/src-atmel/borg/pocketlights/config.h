#define NUM_ROWS 8
#define NUM_COLS 18
#define NUM_PLANES 1
#define NUMPLANE 3

#define SCROLLTXT_DELAY 60
#define STREAMER_NUM 30

#define FEUER_N 5
#define FEUER_S 30
#define FEUER_DIV 44
#define FEUER_DELAY 50

#define SNAKE_DELAY 100

#define SCROLLTEXT_STRING_SIZE 128
#define SCROLLTEXT_FONT font_arial8
#define SCROLLTEXT_DEFAULT "</#LABOR Borg - Widerstand ist zwecklos."
#define SCROLL_X_SPEED 30
#define SCROLL_Y_SPEED 20

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




