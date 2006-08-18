#define TCNT0_INIT (0xFF-F_CPU/256/TICKRATE)

#define TMC8_CK256 (1<<CS02)

#define F_CPU 16000000L
#define F_MCP F_CPU

#define TICKRATE 2500

#define MEGA8

#ifdef MEGA8
#	define SPI_PORT PORTB
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

// Serial.[ch] defines
#define BAUDRATE 57600L     // Debug monitor baudrate
#define UBRR_INIT (F_CPU/(16*BAUDRATE)-1)
#define TX0_BUFSZ 48
#define RX0_BUFSZ 48
