
#ifndef __C64__
#include <avr/io.h>
#include <avr/interrupt.h>
#define asm asm volatile
#endif

#include "can.h"
#include "spi.h"

#include "config.h"
#include "borg_can.h"

#define spi_clear_ss() SPI_PORT |= (1<<SPI_PIN_SS)
#define spi_set_ss() SPI_PORT &= ~(1<<SPI_PIN_SS)

#define CAN_INT_ON()  GICR |= (1<<INT0)
#define CAN_INT_OFF() GICR &= ~(1<<INT0)

/********** Global vars *********************/

can_message Can_rx_msg;


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


/* MCP */
void mcp_reset();
void mcp_write(unsigned char reg, unsigned char data);
//void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len);
unsigned char mcp_read(unsigned char reg);
//void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len);
void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val);
unsigned char mcp_status();
//unsigned char mcp_rx_status();

// Functions
/*
unsigned char mcp_rx_status(){
	unsigned char d;
	spi_set_ss();
	spi_data(RX_STATUS);
	d = spi_data(0);
	spi_clear_ss();
	return d;
}
*/

unsigned char mcp_status(){
	unsigned char d;
	spi_set_ss();
	spi_data(READ_STATUS);
	d = spi_data(0);
	spi_clear_ss();
	return d;
}

void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val){
	spi_set_ss();
	spi_data(BIT_MODIFY);
	spi_data(reg);
	spi_data(mask);
	spi_data(val);
	spi_clear_ss();
}

//load a message to mcp2515 and start transmission
void message_load(can_message * msg){
	unsigned char x;
	
	spi_set_ss();
	spi_data(WRITE);
	spi_data(TXB0SIDH);

	spi_data( ((unsigned char)(msg->port_src << 2)) | (msg->port_dst >> 4 ) );
	spi_data( (unsigned char)((msg->port_dst & 0x0C) << 3) | (1<<EXIDE) | (msg->port_dst & 0x03) );
	spi_data(msg->addr_src);
	spi_data(msg->addr_dst);
	spi_data(msg->dlc);
	for(x=0;x<msg->dlc;x++){
		spi_data(msg->data[x]);
	}
	spi_clear_ss();
	spi_set_ss();
	spi_data(WRITE);
	spi_data(TXB0CTRL);
	spi_data( (1<<TXREQ) );
	spi_clear_ss();
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(can_message * msg){
	unsigned char tmp1, tmp2, tmp3;
	unsigned char x;

	spi_set_ss();
	spi_data(READ);
	spi_data(RXB0SIDH);
	tmp1 = spi_data(0);
	msg->port_src = tmp1 >> 2;
	tmp2 = spi_data(0);
	tmp3 = (unsigned char)((unsigned char)(tmp2 >> 3) & 0x0C);
	msg->port_dst = ((unsigned char)(tmp1 <<4 ) & 0x30) | tmp3 | (unsigned char)(tmp2 & 0x03);
	msg->addr_src = spi_data(0);
	msg->addr_dst = spi_data(0);
	msg->dlc = spi_data(0) & 0x0F;	
	for(x=0;x<msg->dlc;x++){
		msg->data[x] = spi_data(0);
	}
	spi_clear_ss();
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
}


static volatile unsigned char TX_INT;


SIGNAL(SIG_INTERRUPT0) {
	CAN_INT_OFF();
	sei();
	unsigned char status = mcp_status();

	if ( status & 0x01 ) {	// Message in RX0
		message_fetch(&Can_rx_msg);
		bcan_process_message();
	} else if ( status & 0x08 ) {	// TX1 empty
		TX_INT = 0;
		//reset the Interrupt flag
		mcp_bitmod(CANINTF, (1<<TX0IF), 0x00);
	}
	cli();
	CAN_INT_ON();
}

void mcp_reset(){
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

/*
void mcp_write_b(unsigned char reg, unsigned char *buf, unsigned char len){
	unsigned char x;
	spi_set_ss();
	spi_data(WRITE);
	spi_data(reg);
	for(x=0;x<len;x++){
		spi_data(buf[x]);
	}
	spi_clear_ss();
}
*/

unsigned char mcp_read(unsigned char reg){
	unsigned char d;
	spi_set_ss();
	spi_data(READ);
	spi_data(reg);
	d = spi_data(0);
	spi_clear_ss();
	return d;
}

/*
void mcp_read_b(unsigned char reg, unsigned char *buf, unsigned char len){
	unsigned char x;
	spi_set_ss();
	spi_data(READ);
	spi_data(reg);
	for(x=0;x<len;x++){
		buf[x] = spi_data(0);
	}
	spi_clear_ss();
}
*/


/* Management */
void can_setmode( can_mode_t mode ) {
	unsigned char val = mode << 5;  
	val |= 0x04;  // CLKEN

	mcp_write( CANCTRL, val );
}


void can_setfilter() {
	//RXM1   RXM0
	//  0      0     receive matching filter
	//  0      1     " only 11bit Identifier
	//  1      0     " only 29bit Identifier
	//  1      1     any
	mcp_write(RXB0CTRL, (1<<RXM1) | (1<<RXM0));
}

void can_setled(unsigned char led, unsigned char state){
	mcp_bitmod(BFPCTRL, 0x10<<led, state?0xff:0);
}

/*******************************************************************/
void delayloop(){
	unsigned char x;
	for(x=0;x<255;x++){		
		asm ("nop");
	}

}

void can_init(){
	//set Slave select high
	SPI_PORT |= (1<<SPI_PIN_SS);
	
	mcp_reset();
	
	delayloop();
	
	mcp_write(BFPCTRL,0x0C);//RXBF Pins to Output
	
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
	mcp_write( CNF1, 0x40 | CNF1_T );
	mcp_write( CNF2, 0xf1 );
	mcp_write( CNF3, 0x05 );

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write( CANINTE, (1<<RX0IE) | (1<<TX0IE) );

	can_setfilter();
	can_setmode(normal);

	//this turns on INT0 on the Atmega	
	//We'll leave the ISC bits at zero, so we get an interrupt on low level.
	CAN_INT_ON();
}


//start transmitting a can message
void can_transmit(can_message* msg){
	if(!TX_INT){
		TX_INT = 1;
		message_load(msg);
	}
}

