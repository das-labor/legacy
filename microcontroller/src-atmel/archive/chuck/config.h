#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

#define TMC8_CK256 (1<<CS02)

//#define F_CPU 16000000L
#define F_MCP F_CPU

#define TICKRATE 1000

#define MEGA8
#ifdef MEGA8
#	define SPI_PORT PORTB
//#	define SPI_PIN_SS PB2
#	define SPI_PIN_SS PB2
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB5
#	define SPI_PIN_MOSI PB3
#else
#	define SPI_PORT PORTB
#	define SPI_PIN_SS PB4
#	define SPI_HARDWARE
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB7
#	define SPI_PIN_MOSI PB5
#endif

//Number of Messages in Can TX Buffer
#define TX_SIZE 10
