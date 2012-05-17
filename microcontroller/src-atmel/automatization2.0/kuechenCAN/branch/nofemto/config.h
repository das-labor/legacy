#ifndef CONFIG_H_
#define CONFIG_H_

#define TWI_BITRATE 100000
#define TWI_MTHREAD 
#define TWI_SLAVE (15)

#define MC_MOSI    PB3
#define MC_CLK     PB5

#define MCP_CS        PB2
#define MCP_CMD_PORT  PORTB

#define SPI_REG_PIN_MCP_INT  PIND
#define SPI_PIN_MCP_INT      PD2

#define F_MCP F_CPU

#define MEGA8

#ifdef MEGA8
#	define SPI_PORT PORTB
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB5
#	define SPI_PIN_MOSI PB3
#else
#	define SPI_PORT PORTB
#	define SPI_DDR DDRB
#	define SPI_PIN_SCK PB7
#	define SPI_PIN_MOSI PB5
#endif

#if CANADDR <=0x03
#error "you have to define a can-address which will not interfere with critical LABOR-Hardware"
#endif

#endif /*CONFIG_H_*/
