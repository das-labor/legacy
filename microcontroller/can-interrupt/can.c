#include <avr/io.h>
#include <avr/signal.h>
#include "can.h"
#include "util.h"
#include "spi.h"


//for slave select
#define PORT_SPI PORTB
#define PIN_SS PB4


//Registers
#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EID8 0x02
#define RXF0EID0 0x03
#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EID8 0x06
#define RXF1EID0 0x07
#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B
#define BFPCTRL 0x0C
#define TXRTSCTRL 0x0D
#define CANSTAT 0x0E
#define CANCTRL 0x0F
#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EID8 0x12
#define RXF3EID0 0x13
#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EID8 0x16
#define RXF4EID0 0x17
#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B
#define TEC 0x1C
#define REC 0x1D
#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM0EID8 0x22
#define RXM0EID0 0x23
#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM1EID8 0x26
#define RXM1EID0 0x27
#define CNF3 0x28
#define CNF2 0x29
#define CNF1 0x2A
#define CANINTE 0x2B
	#define MERRE 7
	#define WAKIE 6
	#define ERRIE 5
	#define TX2IE 4
	#define TX1IE 3
	#define TX0IE 2
	#define RX1IE 1
	#define RX0IE 0
#define CANINTF 0x2C
	#define MERRF 7
	#define WAKIF 6
	#define ERRIF 5
	#define TX2IF 4
	#define TX1IF 3
	#define TX0IF 2
	#define RX1IF 1
	#define RX0IF 0
#define EFLG 0x2D
#define TXB0CTRL 0x30
	#define TXREQ 3
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
	#define EXIDE 3
#define TXB0EID8 0x33
#define TXB0EID0 0x34
#define TXB0DLC 0x35
#define TXB0D0 0x36 

#define RXB0CTRL 0x60
	#define RXM1 6
	#define RXM0 5
	#define RXRTR 3
	// Bits 2:0 FILHIT2:0
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB0DLC 0x65
#define RXB0D0 0x66 

//Command Bytes
#define RESET 0xC0
#define READ 0x03
#define READ_RX_BUFFER 0x90
#define WRITE 0x02
#define LOAD_TX_BUFFER 0x40
#define RTS 0x80
#define READ_STATUS 0xA0
#define RX_STATUS 0xB0
#define BIT_MODIFY 0x05


#define RX_BUFFER_SIZE 10
#define TX_BUFFER_SIZE 10
static can_message RX_BUFFER[RX_BUFFER_SIZE], *RX_HEAD=RX_BUFFER, *RX_TAIL=RX_BUFFER;
static can_message TX_BUFFER[TX_BUFFER_SIZE], *TX_HEAD=TX_BUFFER, *TX_TAIL=TX_BUFFER;
static volatile unsigned char TX_INT;


inline unsigned char mcp_rx_status(){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(RX_STATUS);
	unsigned char d = spi_data(0);
	PORT_SPI |= (1<<PIN_SS);
	return d;
}

inline unsigned char mcp_status(){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(READ_STATUS);
	unsigned char d = spi_data(0);
	PORT_SPI |= (1<<PIN_SS);
	return d;
}

inline void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(BIT_MODIFY);
	spi_data(reg);
	spi_data(mask);
	spi_data(val);
	PORT_SPI |= (1<<PIN_SS);
}

inline void message_load(){
	unsigned char x;
	if( (TX_HEAD != TX_TAIL) && (TX_TAIL->flags & 0x01) ){
		TX_INT = 1;
		PORT_SPI &= ~(1<<PIN_SS);
		spi_data(WRITE);
		spi_data(TXB0SIDH);
		spi_data(TX_TAIL->addr_src);
		spi_data(  ((TX_TAIL->port_src<<2)&0x1F) |  ((TX_TAIL->port_src>>1)&0x03) | (1<<EXIDE) );
		spi_data( (TX_TAIL->port_dest) | (TX_TAIL->port_src<<7) );
		spi_data(TX_TAIL->addr_dest);
		spi_data(TX_TAIL->dlc);
		for(x=0;x<TX_TAIL->dlc;x++){
			spi_data(TX_TAIL->data[x]);
		}
		PORT_SPI |= (1<<PIN_SS);
		PORT_SPI &= ~(1<<PIN_SS);
		spi_data(WRITE);
		spi_data(TXB0CTRL);
		spi_data( (1<<TXREQ) );
		PORT_SPI |= (1<<PIN_SS);
		if(++TX_TAIL == TX_BUFFER+TX_BUFFER_SIZE) TX_TAIL = TX_BUFFER;
	}else{
		TX_INT = 0;
	}
}

SIGNAL(SIG_INTERRUPT0) {
	unsigned char status = mcp_status();
	unsigned char tmp1, tmp2;
	unsigned char x;
	
	if ( status & 0x01 ) {	// Message in RX0
		PORTC |= 0x04;
		
		PORT_SPI &= ~(1<<PIN_SS);
		SPDR = READ;
		wait_spi();
		SPDR = RXB0SIDH;
		wait_spi();
		SPDR = 0;
		wait_spi();
		RX_HEAD->addr_src = SPDR;					//SID10:3
		SPDR = 0;
		wait_spi();
		tmp1 = SPDR;
		SPDR = 0;
		wait_spi();
		tmp2 = SPDR;
		SPDR = 0;
		RX_HEAD->port_src = ((tmp1&0x03)<<1)|((tmp1>>2)&0x38)|(tmp2>>7);//SID2:0 : EID17:15
		RX_HEAD->port_dest = tmp2 & 0x7F; 				//EID14:8
		wait_spi();
		RX_HEAD->addr_dest = SPDR;					//EID7:0
		SPDR = 0;
		wait_spi();
		RX_HEAD->dlc = SPDR & 0x0F;					//DLC
		for(x=0;x<RX_HEAD->dlc;x++){
			SPDR = 0;
			wait_spi();
			RX_HEAD->data[x] = SPDR;
		}
		PORT_SPI |= (1<<PIN_SS);
		
		if( ++RX_HEAD == RX_BUFFER+RX_BUFFER_SIZE) RX_HEAD = RX_BUFFER;
		
		mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
		PORTC &= ~0x04;
	}

	if ( status & 0x08 ) {	// TX1 empty
		PORTC ^= 0x08;
		message_load();
		mcp_bitmod(CANINTF, (1<<TX0IF), 0x00);
	}
}



void mcp_reset(){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(RESET);
	PORT_SPI |= (1<<PIN_SS);
}



void mcp_write(unsigned char reg, unsigned char data){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(WRITE);
	spi_data(reg);
	spi_data(data);
	PORT_SPI |= (1<<PIN_SS);
}

void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(WRITE);
	spi_data(reg);
	unsigned char x;
	for(x=0;x<len;x++){
		spi_data(buf[x]);
	}
	PORT_SPI |= (1<<PIN_SS);
}

unsigned char mcp_read(unsigned char reg){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(READ);
	spi_data(reg);
	unsigned char d = spi_data(0);
	PORT_SPI |= (1<<PIN_SS);
	return d;
}

void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(READ);
	spi_data(reg);
	unsigned char x;
	for(x=0;x<len;x++){
		buf[x] = spi_data(0);
	}
	PORT_SPI |= (1<<PIN_SS);
}






/* Management */
void mcp_setmode( mcp_mode_t mode ) {
	unsigned char val = mode << 5;  
	val |= 0x04;  // CLKEN

	mcp_write( CANCTRL, val );
}


void mcp_setfilter() {
	//RXM1   RXM0
	//  0      0     receive matching filter
	//  0      1     " only 11bit Identifier
	//  1      0     " only 29bit Identifier
	//  1      1     any
	mcp_write(RXB0CTRL, (1<<RXM1) | (1<<RXM0));
}


/*******************************************************************/
void can_init(){
	mcp_reset();
	
	wait( 1 );
	
	// 100kbps 
	mcp_write( CNF1, 0x44 );
	mcp_write( CNF2, 0xf1 );
	mcp_write( CNF3, 0x05 );

	// configure IRQ
	mcp_write( CANINTE, (1<<MERRE) | (1<<RX0IE) | (1<<TX0IE) );
	GICR |= 0x40;

	mcp_setfilter();
	mcp_setmode(loopback);
}


void can_send(char *buf, unsigned char len){
	mcp_write( TXB0SIDL, 0xF0 );
	mcp_write( TXB0SIDH, 0x88 );


	mcp_write( TXB0CTRL, 0x03 );
	mcp_write_b( TXB0D0, buf, len );
	mcp_write( TXB0DLC, len );
	mcp_write( TXB0CTRL, 0x0b );
}

//returns pointer to the next can TX buffer
can_message * can_buffer_get(){
	can_message *p;
	p = TX_HEAD;
	p->flags = 0;
	if(++TX_HEAD == TX_BUFFER+TX_BUFFER_SIZE) TX_HEAD = TX_BUFFER;
	return p;
}

//start transmitting can messages
void can_transmit(){
	if(!TX_INT){
		message_load();
	}
}



//returns next can message in buffer, or 0 Pointer if Buffer is empty
can_message * can_message_get_nb(){
	can_message *p;
	if(RX_HEAD == RX_TAIL){
		return 0;
	}else{
		p = RX_TAIL;
		if(++RX_TAIL == RX_BUFFER+RX_BUFFER_SIZE) RX_TAIL = RX_BUFFER;
		return p;
	}
}

char *can_rcvpacket()
{
	static char buf[8];

	while( !(mcp_read(CANINTF) & 0x01) ) ;
	mcp_read_b(RXB0D0, buf, 7);
	mcp_write(CANINTF, 0x00);
	return buf;
}


