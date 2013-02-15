#ifndef _CONFIG_H
#define _CONFIG_H

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

#define R_LED _BV(PC1)
#define G_LED _BV(PC2)
#define B_LED _BV(PC3)

/*
 * can ports
 */
#define LIGHTCANPORT 0x04
#define ALARMCANPORT 0x00
#define I2CTEMPCANPORT (0x10)

/*
 * button thresholds
 */
#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 1


#if CANADDR <=0x03
#error "you have to define a can-address which will not interfere with critical LABOR-Hardware"
#endif

#endif //_CONFIG_H

