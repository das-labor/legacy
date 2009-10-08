#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

#define BAUDRATE 19200L
#define UBRR_INIT (F_CPU/(16*BAUDRATE)-1)

#define TMC8_CK256 (1<<CS02)

#define F_CPU 16000000L
#define F_MCP F_CPU

#define TICKRATE 2500

#define MEGA8

#ifdef MEGA8
#	define SPI_PORT PORTB
#	define SPI_PIN_CS PB2
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB5
#	define SPI_PIN_MOSI PB3
#else
#	define SPI_PORT PORTB
#	define SPI_PIN_CS PB4
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB7
#	define SPI_PIN_MOSI PB5
#endif


/* ATMega32 / 8*/

#define MCP_INT_VEC INT0_vect
#define MCP_INT_MASK INT0
#define MCP_INT_REG GICR

/* ATMega 644 / modern atmega */
/*
#define MCP_INT_VEC INT0_vect
#define MCP_INT_MASK INT0
#define MCP_INT_REG EIMSK
*/

//Number of Messages in Can TX Buffer
#define TX_SIZE 10
