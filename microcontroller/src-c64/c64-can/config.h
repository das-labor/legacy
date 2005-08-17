#ifndef CONFIG_H
#define CONFIG_H


// spi.[ch] defines
#define SPI_DDR CIA2+CIA_DDRA	//for slave select
#define SPI_PORT CIA2+CIA_PRA	//for slave select
#define SPI_PIN_SS 2		//for slave select


// can.[ch] defines
#undef CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
#define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt

#define F_MCP 20000000

//interrupt pin of MCP2515 for non interrupt driven can
#define SPI_REG_PIN_MCP_INT ~REG(CIA2+CIA_ICR)
#define SPI_PIN_MCP_INT 4


// lap.[ch] defines


#endif
