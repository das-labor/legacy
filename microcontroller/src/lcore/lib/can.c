#include <avr/io.h>
#include <avr/signal.h>
#include "config.h"
#include "can.h"
#include "util.h"
#include "spi.h"

// #include "mcp2515.inc"

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

// Functions

inline unsigned char mcp_rx_status(){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(RX_STATUS);
	unsigned char d = spi_data(0);
	SPI_PORT |= (1<<SPI_PIN_SS);
	return d;
}

inline unsigned char mcp_status(){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(READ_STATUS);
	unsigned char d = spi_data(0);
	SPI_PORT |= (1<<SPI_PIN_SS);
	return d;
}

void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(BIT_MODIFY);
	spi_data(reg);
	spi_data(mask);
	spi_data(val);
	SPI_PORT |= (1<<SPI_PIN_SS);
}

//load a message to mcp2515 and start transmission
void message_load(can_message * msg){
	unsigned char x;
	
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(WRITE);
	spi_data(TXB0SIDH);

	spi_data( (msg->port_src << 2) | (msg->port_dest >> 4 ) );
	spi_data( ((msg->port_dest & 0x0C) << 3) | (1<<EXIDE) | (msg->port_dest & 0x03) );

	spi_data(msg->addr_src);
	spi_data(msg->addr_dest);
	spi_data(msg->dlc);
	for(x=0;x<msg->dlc;x++){
		spi_data(msg->data[x]);
	}
	SPI_PORT |= (1<<SPI_PIN_SS);
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(WRITE);
	spi_data(TXB0CTRL);
	spi_data( (1<<TXREQ) );
	SPI_PORT |= (1<<SPI_PIN_SS);
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(can_message * msg){
	unsigned char tmp1, tmp2;
	unsigned char x;

	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(READ);
	spi_data(RXB0SIDH);
	tmp1 = spi_data(0);
	msg->port_src = tmp1 >> 2;
	tmp2 = spi_data(0);
	msg->port_dest = ((tmp1 & 0x03) << 4) | ((tmp2 & 0x60) >> 3) | (tmp2 & 0x03);

	msg->addr_src = spi_data(0);					//SID10:3
	msg->addr_dest = spi_data(0);					//EID7:0
	msg->dlc = spi_data(0) & 0x0F;					//DLC
	for(x=0;x<msg->dlc;x++){
		msg->data[x] = spi_data(0);
	}
	SPI_PORT |= (1<<SPI_PIN_SS);
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
}
#ifdef CAN_INTERRUPT

static can_message RX_BUFFER[CAN_RX_BUFFER_SIZE], *volatile RX_HEAD=RX_BUFFER, *volatile RX_TAIL=RX_BUFFER;
static can_message TX_BUFFER[CAN_TX_BUFFER_SIZE], *volatile TX_HEAD=TX_BUFFER, *volatile TX_TAIL=TX_BUFFER;
static volatile unsigned char TX_INT;

SIGNAL(SIG_INTERRUPT0) {
	PORTD ^=0x10;
	unsigned char status = mcp_status();
		
	if ( status & 0x01 ) {	// Message in RX0
		message_fetch(RX_HEAD);
		if( ++RX_HEAD == RX_BUFFER+CAN_RX_BUFFER_SIZE) RX_HEAD = RX_BUFFER;
	}

	if ( status & 0x08 ) {	// TX1 empty
		if( (TX_HEAD != TX_TAIL) && (TX_TAIL->flags & 0x01) ){
			TX_INT = 1;
			message_load(TX_TAIL);
			if(++TX_TAIL == TX_BUFFER+CAN_TX_BUFFER_SIZE) TX_TAIL = TX_BUFFER;
		}else{
			TX_INT = 0;
		}
		mcp_bitmod(CANINTF, (1<<TX0IF), 0x00);
	}
}

#endif


void mcp_reset(){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(RESET);
	SPI_PORT |= (1<<SPI_PIN_SS);
}

void mcp_write(unsigned char reg, unsigned char data){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(WRITE);
	spi_data(reg);
	spi_data(data);
	SPI_PORT |= (1<<SPI_PIN_SS);
}

void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(WRITE);
	spi_data(reg);
	unsigned char x;
	for(x=0;x<len;x++){
		spi_data(buf[x]);
	}
	SPI_PORT |= (1<<SPI_PIN_SS);
}

unsigned char mcp_read(unsigned char reg){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(READ);
	spi_data(reg);
	unsigned char d = spi_data(0);
	SPI_PORT |= (1<<SPI_PIN_SS);
	return d;
}

void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len){
	SPI_PORT &= ~(1<<SPI_PIN_SS);
	spi_data(READ);
	spi_data(reg);
	unsigned char x;
	for(x=0;x<len;x++){
		buf[x] = spi_data(0);
	}
	SPI_PORT |= (1<<SPI_PIN_SS);
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
	
	// 0x01 : 125kbit/8MHz
	// 0x03 : 125kbit/16MHz
	
	// 100kbps 
	mcp_write( CNF1, 0x40 | 0x01 );
	mcp_write( CNF2, 0xf1 );
	mcp_write( CNF3, 0x05 );

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write( CANINTE, (1<<RX0IE) | (1<<TX0IE) );

	mcp_setfilter();
	mcp_setmode(normal);

#ifdef CAN_INTERRUPT
	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write( CANINTE, (1<<RX0IE) | (1<<TX0IE) );
	//this turns on INT0 on the Atmel
#ifdef ATMEGA	
	GICR |= (1<<INT0);
#else
	MCUCR |=  (1<<ISC01);
	GIMSK |= (1<<INT0);
#endif //ATMEGA
#else  //CAN_INTERRUPT
	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write( CANINTE, (1<<RX0IE) ); //only turn RX int on
#endif //CAN_INTERRUPT
}


#ifdef CAN_INTERRUPT
//returns next can message in buffer, or 0 Pointer if Buffer is empty
can_message * can_get_nb(){
	can_message *p;
	if(RX_HEAD == RX_TAIL){
		return 0;
	}else{
		p = RX_TAIL;
		if(++RX_TAIL == RX_BUFFER+CAN_RX_BUFFER_SIZE) RX_TAIL = RX_BUFFER;
		return p;
	}
}

can_message * can_get(){
	can_message *p;

	while(RX_HEAD == RX_TAIL) { };

	p = RX_TAIL;
	if(++RX_TAIL == RX_BUFFER+CAN_RX_BUFFER_SIZE) RX_TAIL = RX_BUFFER;

	return p;
}

//returns pointer to the next can TX buffer
can_message * can_buffer_get(){
	can_message *p;
	p = TX_HEAD;
	p->flags = 0;
	if(++TX_HEAD == TX_BUFFER+CAN_TX_BUFFER_SIZE) TX_HEAD = TX_BUFFER;
	return p;
}

//start transmitting can messages, and mark message msg as transmittable
void can_transmit(can_message * msg){
	if(msg){
		msg->flags |= 0x01;
	}
	if(!TX_INT){
		if( (TX_HEAD != TX_TAIL) && (TX_TAIL->flags & 0x01) ){
			TX_INT = 1;
			message_load(TX_TAIL);
			if(++TX_TAIL == TX_BUFFER+CAN_TX_BUFFER_SIZE) TX_TAIL = TX_BUFFER;
		}
	}
}
#else

can_message RX_MESSAGE, TX_MESSAGE;

can_message * can_get_nb(){
	if(SPI_PIN_MCP_INT & (1<<SPI_PIN_MCP_INT)){
		return 0;
	}else{
		//So the MCP Generates an RX Interrupt
		message_fetch(RX_MESSAGE);
		return RX_MESSAGE;
	}
}

can_message * can_get(){
	//wait while the MCP doesn't generate an RX Interrupt
	while(SPI_PIN_MCP_INT & (1<<SPI_PIN_MCP_INT)) { };
	
	message_fetch(RX_MESSAGE);
	return RX_MESSAGE;
}

	//only for compatibility with Interrupt driven Version
can_message * can_buffer_get(){
	return TX_MESSAGE;
}

void can_transmit(can_message * msg){
	
}

#endif

