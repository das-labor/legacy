#ifndef CAN_H
#define CAN_H

#include <avr/boot.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "mcp2515reg.h"

/*****************************************************************************
 * Simple CAN Library
 *
 * #define CAN_INTERRUPT 1	//set this to enable interrupt driven 
 *                                and buffering version
 * #define CAN_RX_BUFFER_SIZE 2	//only used for Interrupt
 * #define CAN_TX_BUFFER_SIZE 2	//only used for Interrupt
 */

/*****************************************************************************
 * Types
 */

typedef struct
{
	uint8_t      addr;
	uint8_t      port_src;
	uint8_t      port_dst;
	uint8_t dlc;
	uint8_t data[7];
} can_message;



/*****************************************************************************
 * MCP
 */

/*****************************************************************************
 * Management
 */


#define BFPCTRL 0x0C


/*****************************************************************************
 * Sending
 */


/*****************************************************************************
 * Receiving
 */



can_message Rx_msg, Tx_msg;

/* MCP */
//uint8_t mcp_read(uint8_t reg);


// Functions

#define spi_clear_ss() SPI_PORT |= _BV(SPI_PIN_SS)
#define spi_set_ss() SPI_PORT &= ~_BV(SPI_PIN_SS)


uint8_t spi_data(uint8_t c)
{
	SPDR = c;
	while (!(SPSR & _BV(SPIF)));
	return(SPDR);
}


void mcp_write_b(PGM_P stream)
{
	uint8_t len;
	
	while ((len = pgm_read_byte(stream++)))
	{
		spi_set_ss();
		while (len--)
		{
			SPDR = (pgm_read_byte(stream++));
			while (!(SPSR & _BV(SPIF)));
		}
		spi_clear_ss();
	}
}

uint8_t mcp_txreq_str[]  ={
	3, WRITE, TXB0CTRL, (1<<TXREQ), 0, 0
};

//load a message to mcp2515 and start transmission
void can_transmit()
{
	spi_set_ss();
	spi_data(WRITE);
	spi_data(TXB0SIDH);

	spi_data(((uint8_t)(Tx_msg.port_src << 2)) | (Tx_msg.port_dst >> 4));
	spi_data((uint8_t)((Tx_msg.port_dst & 0x0C) << 3) | (1<<EXIDE) | (Tx_msg.port_dst & 0x03));
	spi_data(0x02); // quelladresse
	spi_data(Tx_msg.addr);
	spi_data(Tx_msg.dlc);
	while (Tx_msg.dlc)
		spi_data(Tx_msg.data[Tx_msg.dlc--]);
	spi_clear_ss();

	mcp_write_b(mcp_txreq_str);
}

uint8_t mcp_canintf_str[]  ={
	3, BIT_MODIFY, CANINTF, (1<<RX0IF), 0, 0
};

//get a message from mcp2515 and disable RX interrupt Condition
static inline void message_fetch()
{
	uint8_t tmp1, tmp2, tmp3;
	uint8_t x;

	spi_set_ss();
	spi_data(READ);
	spi_data(RXB0SIDH);
	tmp1 = spi_data(0);
	Rx_msg.port_src = tmp1 >> 2;
	tmp2 = spi_data(0);
	tmp3 = (uint8_t)((uint8_t)(tmp2 >> 3) & 0x0C);
	Rx_msg.port_dst = ((uint8_t)(tmp1 << 4 ) & 0x30) | tmp3 | (uint8_t)(tmp2 & 0x03);

	Rx_msg.addr = spi_data(0);
	x = spi_data(0);   // nicht benötigte adresse
	Rx_msg.dlc = spi_data(0) & 0x0F;	
	for (x = 0; x < Rx_msg.dlc; x++)
		Rx_msg.data[x] = spi_data(0);
	spi_clear_ss();
	
	mcp_write_b(mcp_canintf_str);
}



#define FLT_PORT_SRC 0
#define FLT_PORT_DST1 PORT_SDO_CMD
#define FLT_PORT_DST2 PORT_SDO_DATA
#define FLT_ADDR_SRC 0	

#define MSK_PORT_SRC 0
#define MSK_PORT_DST 0x3F
#define MSK_ADDR_SRC 0
#define MSK_ADDR_DST 0xFF



/*******************************************************************/


// 0x01 : 125kbit/8MHz
// 0x03 : 125kbit/16MHz
// 0x04 : 125kbit/20MHz
#if F_MCP == 16000000
#define CNF1_T 0x03
#elif F_MCP == 8000000
#define CNF1_T 0x01
#elif F_MCP == 20000000
#define CNF1_T 0x04
#else
#error Can Baudrate is only defined for 8, 16 and 20 MHz
#endif 
uint8_t mcp_config_str1[] ={
	3, WRITE, BFPCTRL, 0x0C,		//RXBF Pins to Output
	6, WRITE, CNF3,
		0x05,			//CNF3
		0xf1,			//CNF2
		0x40 | CNF1_T,		//CNF1
		(1<<RX0IE),
	10, WRITE, RXF0SIDH,
		(FLT_PORT_SRC << 2) | (FLT_PORT_DST1 >> 4 ),
		((FLT_PORT_DST1 & 0x0C) << 3) | (1<<EXIDE) | (FLT_PORT_DST1 & 0x03),
		FLT_ADDR_SRC,
		0x35,
		(FLT_PORT_SRC << 2) | (FLT_PORT_DST2 >> 4 ),
		((FLT_PORT_DST2 & 0x0C) << 3) | (1<<EXIDE) | (FLT_PORT_DST2 & 0x03),
		FLT_ADDR_SRC,
		0x35,
	6, WRITE, RXM0SIDH,
		(MSK_PORT_SRC << 2) | (MSK_PORT_DST >> 4 ),
		((MSK_PORT_DST & 0x0C) << 3) | (MSK_PORT_DST & 0x03),
		MSK_ADDR_SRC,
		MSK_ADDR_DST,
	3, WRITE, RXB0CTRL, (0<<RXM1) | (0<<RXM0),
	3, WRITE, CANCTRL, 0,
	0
};

static inline void can_init()
{
	//set output SPI pins to output
	SPI_DDR = _BV(SPI_PIN_MOSI) | _BV(SPI_PIN_SCK) | _BV(SPI_PIN_SS);
	SPCR = _BV(SPE) | _BV(MSTR);
	//Double speed on
	SPSR = _BV(SPI2X);
	
	//set Slave select high
	spi_set_ss();
	spi_data(RESET);
	spi_clear_ss();
	
//	EEAR = 0;
//	EECR = _BV(EERE);

	_delay_ms(2);
//	mcp_write(RXF0EID0, EEDR);
//	mcp_write(RXF1EID0, EEDR);
	mcp_write_b(mcp_config_str1);
}


uint8_t can_get_nb()
{
	//check the pin, that the MCP's Interrup output connects to
	if (SPI_REG_PIN_MCP_INT & _BV(SPI_PIN_MCP_INT))
	{
		return 0; //no message
	}
	
	message_fetch();
	return 1;
}


#endif
