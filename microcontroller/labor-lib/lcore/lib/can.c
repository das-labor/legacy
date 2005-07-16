#include <avr/io.h>
#include <avr/signal.h>
#include "can.h"
#include "util.h"
#include "spi.h"

#include "mcp2515.inc"

//set this to enable interrupt driven and buffering version
#define CAN_INTERRUPT

//only used for Interrupt
#define RX_BUFFER_SIZE 2
#define TX_BUFFER_SIZE 2

//for slave select
#define PORT_SPI PORTB
#define PIN_SS PB4

//interrupt pin of MCP2515 for non interrupt drive can
#define PINREG_MCP_INT PIND
#define PIN_MCP_INT PD2

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

void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val){
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(BIT_MODIFY);
	spi_data(reg);
	spi_data(mask);
	spi_data(val);
	PORT_SPI |= (1<<PIN_SS);
}

//load a message to mcp2515 and start transmission
void message_load(can_message * msg){
	unsigned char x;
	
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(WRITE);
	spi_data(TXB0SIDH);
	spi_data(msg->addr_src);
	spi_data(  ((msg->port_src<<2)&0x1F) |  ((msg->port_src>>1)&0x03) | (1<<EXIDE) );
	spi_data( (msg->port_dest) | (msg->port_src<<7) );
	spi_data(msg->addr_dest);
	spi_data(msg->dlc);
	for(x=0;x<msg->dlc;x++){
		spi_data(msg->data[x]);
	}
	PORT_SPI |= (1<<PIN_SS);
	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(WRITE);
	spi_data(TXB0CTRL);
	spi_data( (1<<TXREQ) );
	PORT_SPI |= (1<<PIN_SS);
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(can_message * msg){
	unsigned char tmp1, tmp2;
	unsigned char x;

	PORT_SPI &= ~(1<<PIN_SS);
	spi_data(READ);
	spi_data(RXB0SIDH);
	msg->addr_src = spi_data(0);					//SID10:3
	tmp1 = spi_data(0);
	tmp2 = spi_data(0);
	msg->port_src = ((tmp1&0x03)<<1)|((tmp1>>2)&0x38)|(tmp2>>7);//SID2:0 : EID17:15
	msg->port_dest = tmp2 & 0x7F; 				//EID14:8
	msg->addr_dest = spi_data(0);					//EID7:0
	msg->dlc = spi_data(0) & 0x0F;					//DLC
	for(x=0;x<msg->dlc;x++){
		msg->data[x] = spi_data(0);
	}
	PORT_SPI |= (1<<PIN_SS);
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
}


#ifdef CAN_INTERRUPT

static can_message RX_BUFFER[RX_BUFFER_SIZE], *volatile RX_HEAD=RX_BUFFER, *volatile RX_TAIL=RX_BUFFER;
static can_message TX_BUFFER[TX_BUFFER_SIZE], *volatile TX_HEAD=TX_BUFFER, *volatile TX_TAIL=TX_BUFFER;
static volatile unsigned char TX_INT;

SIGNAL(SIG_INTERRUPT0) {
	PORTD ^=0x10;
	unsigned char status = mcp_status();
		
	if ( status & 0x01 ) {	// Message in RX0
		message_fetch(RX_HEAD);
		if( ++RX_HEAD == RX_BUFFER+RX_BUFFER_SIZE) RX_HEAD = RX_BUFFER;
	}

	if ( status & 0x08 ) {	// TX1 empty
		if( (TX_HEAD != TX_TAIL) && (TX_TAIL->flags & 0x01) ){
			TX_INT = 1;
			message_load(TX_TAIL);
			if(++TX_TAIL == TX_BUFFER+TX_BUFFER_SIZE) TX_TAIL = TX_BUFFER;
		}else{
			TX_INT = 0;
		}
		mcp_bitmod(CANINTF, (1<<TX0IF), 0x00);
	}
}

#endif


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
		if(++RX_TAIL == RX_BUFFER+RX_BUFFER_SIZE) RX_TAIL = RX_BUFFER;
		return p;
	}
}

can_message * can_get(){
	can_message *p;

	while(RX_HEAD == RX_TAIL) { };

	p = RX_TAIL;
	if(++RX_TAIL == RX_BUFFER+RX_BUFFER_SIZE) RX_TAIL = RX_BUFFER;

	return p;
}

//returns pointer to the next can TX buffer
can_message * can_buffer_get(){
	can_message *p;
	p = TX_HEAD;
	p->flags = 0;
	if(++TX_HEAD == TX_BUFFER+TX_BUFFER_SIZE) TX_HEAD = TX_BUFFER;
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
			if(++TX_TAIL == TX_BUFFER+TX_BUFFER_SIZE) TX_TAIL = TX_BUFFER;
		}
	}
}
#else

can_message RX_MESSAGE, TX_MESSAGE;

can_message * can_get_nb(){
	if(PINPORT_MCP_INT & (1<<PIN_MCP_INT)){
		return 0;
	}else{
		//So the MCP Generates an RX Interrupt
		message_fetch(RX_MESSAGE);
		return RX_MESSAGE;
	}
}

can_message * can_get(){
	//wait while the MCP doesn't generate an RX Interrupt
	while(PINPORT_MCP_INT & (1<<PIN_MCP_INT)) { };
	
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

