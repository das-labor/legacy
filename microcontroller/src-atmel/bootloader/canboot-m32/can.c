#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "can.h"
#include "mcp2515reg.h"

can_message Rx_msg, Tx_msg;

/* MCP */
void mcp_write(unsigned char reg, unsigned char data) BOOTLOADER_SECTION;
void mcp_write_b(PGM_P stream) BOOTLOADER_SECTION;
unsigned char mcp_read(unsigned char reg) BOOTLOADER_SECTION;
unsigned char mcp_status()  BOOTLOADER_SECTION;

void message_load() BOOTLOADER_SECTION;

// Functions

#define spi_clear_ss() SPI_PORT |= (1<<SPI_PIN_SS)
#define spi_set_ss() SPI_PORT &= ~(1<<SPI_PIN_SS)

unsigned char spi_data(unsigned char c) BOOTLOADER_SECTION;

unsigned char spi_data(unsigned char c){
	SPDR = c;
	while(!(SPSR & (1<<SPIF)));
	return(SPDR);
}

unsigned char mcp_status(){
	unsigned char d;
	spi_set_ss();
	spi_data(READ_STATUS);
	d = spi_data(0);
	spi_clear_ss();
	return d;
}


inline static void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val){
	spi_set_ss();
	spi_data(BIT_MODIFY);
	spi_data(reg);
	spi_data(mask);
	spi_data(val);
	spi_clear_ss();
}



unsigned char mcp_txreq_str[] ={
	2, TXB0CTRL, (1<<TXREQ), 0, 0
};

//load a message to mcp2515 and start transmission
void can_transmit(){
	unsigned char x;
	
	spi_set_ss();
	spi_data(WRITE);
	spi_data(TXB0SIDH);

	spi_data( ((unsigned char)(Tx_msg.port_src << 2)) | (Tx_msg.port_dst >> 4 ) );
	spi_data( (unsigned char)((Tx_msg.port_dst & 0x0C) << 3) | (1<<EXIDE) | (Tx_msg.port_dst & 0x03) );
	spi_data(Tx_msg.addr_src);
	spi_data(Tx_msg.addr_dst);
	spi_data(Tx_msg.dlc);
	for(x=0;x<Tx_msg.dlc;x++){
		spi_data(Tx_msg.data[x]);
	}
	spi_clear_ss();
	
	mcp_write_b(mcp_txreq_str);
}


//get a message from mcp2515 and disable RX interrupt Condition
static inline void message_fetch(){
	unsigned char tmp1, tmp2, tmp3;
	unsigned char x;

	spi_set_ss();
	spi_data(READ);
	spi_data(RXB0SIDH);
	tmp1 = spi_data(0);
	Rx_msg.port_src = tmp1 >> 2;
	tmp2 = spi_data(0);
	tmp3 = (unsigned char)((unsigned char)(tmp2 >> 3) & 0x0C);
	Rx_msg.port_dst = ((unsigned char)(tmp1 <<4 ) & 0x30) | tmp3 | (unsigned char)(tmp2 & 0x03);

	Rx_msg.addr_src = spi_data(0);
	Rx_msg.addr_dst = spi_data(0);
	Rx_msg.dlc = spi_data(0) & 0x0F;	
	for(x=0;x<Rx_msg.dlc;x++){
		Rx_msg.data[x] = spi_data(0);
	}
	spi_clear_ss();
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
}


static inline void mcp_reset(){
	spi_set_ss();
	spi_data(RESET);
	spi_clear_ss();
}

void mcp_write(unsigned char reg, unsigned char data){
	spi_set_ss();
	spi_data(WRITE);
	spi_data(reg);
	spi_data(data);
	spi_clear_ss();
}

void mcp_write_b(PGM_P stream){
	unsigned char len;
	
	while(( len = pgm_read_byte(stream++) )){
		spi_set_ss();
		spi_data(WRITE);
		while(len--){
			spi_data(pgm_read_byte(stream++));
		}
		spi_clear_ss();
	}
}



unsigned char mcp_read(unsigned char reg){
	unsigned char d;
	spi_set_ss();
	spi_data(READ);
	spi_data(reg);
	d = spi_data(0);
	spi_clear_ss();
	return d;
}

/* Management */
/*
void can_setmode( can_mode_t mode ) {
	unsigned char val = mode << 5;  
	val |= 0x04;  // CLKEN

	mcp_write( CANCTRL, val );
}
*/

extern unsigned char Station_id;


#define FLT_PORT_SRC 0
#define FLT_PORT_DST1 PORT_SDO_CMD
#define FLT_PORT_DST2 PORT_SDO_DATA
#define FLT_ADDR_SRC 0	

#define MSK_PORT_SRC 0
#define MSK_PORT_DST 0x3F
#define MSK_ADDR_SRC 0
#define MSK_ADDR_DST 0xFF



/*******************************************************************/
static inline void delayloop(){
	unsigned char x;
	for(x=0;x<255;x++){		
		asm ("nop");
	}

}

static inline void spi_init(){
	//set output SPI pins to output
	SPI_DDR = (1<<SPI_PIN_MOSI) | (1<<SPI_PIN_SCK) | (1<<SPI_PIN_SS);

	//set Slave select high
	SPI_PORT = (1<<SPI_PIN_SS);
	
	//          0             1
	//DORD   msb first     lsb first
	//CPOL   low idle      high idle
	//CPHA   rising edge  fallling edge
	
	//SPR0    SPR1
	// 0        0       ck/4
	// 0        1       ck/16
	// 1        0       ck/64
	// 1        1       ck/128
	
	//Interrupt disable, SPI enable, MSB first, master, clk low idle, data valid on rising edge, ck/4
	SPCR = (0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

	//Double speed on
	SPSR = (1<<SPI2X);
}


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


unsigned char mcp_config_str1[] ={
	2, BFPCTRL, 0x0C,		//RXBF Pins to Output
	4, CNF3,
		0x05,			//CNF3
		0xf1,			//CNF2
		0x40 | CNF1_T,		//CNF1
	9, RXF0SIDH,
		(FLT_PORT_SRC << 2) | (FLT_PORT_DST1 >> 4 ),
		((FLT_PORT_DST1 & 0x0C) << 3) | (1<<EXIDE) | (FLT_PORT_DST1 & 0x03),
		FLT_ADDR_SRC,
		0x35,
		(FLT_PORT_SRC << 2) | (FLT_PORT_DST2 >> 4 ),
		((FLT_PORT_DST2 & 0x0C) << 3) | (1<<EXIDE) | (FLT_PORT_DST2 & 0x03),
		FLT_ADDR_SRC,
		0x35,
	5, RXM0SIDH,
		(MSK_PORT_SRC << 2) | (MSK_PORT_DST >> 4 ),
		((MSK_PORT_DST & 0x0C) << 3) | (MSK_PORT_DST & 0x03),
		MSK_ADDR_SRC,
		MSK_ADDR_DST,
	2, RXB0CTRL,(0<<RXM1) | (0<<RXM0),
	0
};
	
	
unsigned char mcp_config_str2[] ={	
	2, CANCTRL, 0,
	2, CANINTE, (1<<RX0IE),
	0
};

void can_init(){
	spi_init();
	mcp_reset();
	
	delayloop();
	
	mcp_write_b(mcp_config_str1);

	unsigned char tmp = Station_id;
	mcp_write(RXF0EID0, tmp );
	mcp_write(RXF1EID0, tmp );

	mcp_write_b(mcp_config_str2);
}


unsigned char can_get_nb(){
	//check the pin, that the MCP's Interrup output connects to
	if((SPI_REG_PIN_MCP_INT & (1<<SPI_PIN_MCP_INT))){
		return 0; //no message
	}
	
	message_fetch();
	return 1;
}

