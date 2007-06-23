#include <avr/io.h>
#include <string.h>

#include <avrx.h>
#include <avr/interrupt.h>
#include <AvrXFifo.h>

#include "xcan.h"
#include "../spi/spi.h"

//Registers
#include "mcp2515.h"

#define spi_clear_ss() SPI_PORT |= (1<<SPI_PIN_SS)
#define spi_set_ss() SPI_PORT &= ~(1<<SPI_PIN_SS)


// ******************** Global Variables ***************************************
static Mutex tx_mutex, rx_mutex;

//this is the buffer for received messages. 
//can_get() overwrites this buffer when called
can_message_t rx_msg;


typedef struct{
	uint8_t in;
	uint8_t out;
	uint8_t size;
	can_message_t buf[TX_SIZE];
}can_fifo_t;

//packets to be sent are queued here
static can_fifo_t tx_fifo;

// *************** commands for Microchip MCP2515 CAN Controller ***************
void mcp_reset();
void mcp_write(unsigned char reg, unsigned char data);

unsigned char mcp_read(unsigned char reg);

void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val);
unsigned char mcp_status();

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
void message_load(can_message_t * tx_msg){
	unsigned char x;
	
	spi_set_ss();
	spi_data(WRITE);
	spi_data(TXB0SIDH);

	spi_data( (uint8_t)(tx_msg->channel >>1) );
	spi_data( (uint8_t)((tx_msg->channel & 0x01) << 7) | ((uint8_t)tx_msg->subchannel << 5 ) | (1<<EXIDE) );
	spi_data(tx_msg->addr_src);
	spi_data(tx_msg->addr_dst);
	spi_data(tx_msg->dlc);
	for(x=0;x<tx_msg->dlc;x++){
		spi_data(tx_msg->data[x]);
	}
	spi_clear_ss();
	spi_set_ss();
	spi_data(WRITE);
	spi_data(TXB0CTRL);
	spi_data( (1<<TXREQ) );
	spi_clear_ss();
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(){
	unsigned char tmp1, tmp2;
	unsigned char x;

	spi_set_ss();
	spi_data(READ);
	spi_data(RXB0SIDH);
	tmp1 = spi_data(0);
	tmp2 = spi_data(0);
	rx_msg.channel = tmp1 << 1 | ((uint8_t)tmp2>>7);
	rx_msg.subchannel = (((uint8_t)tmp2>>5) & 0x03);
	rx_msg.addr_src = spi_data(0);
	rx_msg.addr_dst = spi_data(0);
	rx_msg.dlc = spi_data(0) & 0x0F;	
	for(x=0;x<rx_msg.dlc;x++){
		rx_msg.data[x] = spi_data(0);
	}
	spi_clear_ss();
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
}


// *********************** Interrupt handler  **********************************
AVRX_SIGINT(SIG_INTERRUPT0)
{
	IntProlog();             // Switch to kernel stack/context
	GICR &= ~(1<<INT0);
	EndCritical();
	unsigned char status = mcp_status();

	if ( status & 0x01 ) {	// Message in RX0
		mcp_bitmod(CANINTE, (1<<RX0IE), 0x00); //disable interrupt
		AvrXIntSetSemaphore(&rx_mutex);
	} else if ( status & 0x08 ) {	// TX0 empty
		if (tx_fifo.in == tx_fifo.out)	// tx_fifo is Empty
		{
			mcp_bitmod(CANINTE, (1<<TX0IE), 0x00); //disable interrupt
		}
		else
		{
			//Pull can message from fifo and transmit it
			message_load (&tx_fifo.buf[tx_fifo.out]);
			uint8_t t = tx_fifo.out+1;
			if (t >= tx_fifo.size)
				t = 0;
			tx_fifo.out = t;
			mcp_bitmod(CANINTF, (1<<TX0IF), 0x00); //clear interrupt condition
			AvrXIntSetSemaphore(&tx_mutex); //Signal were ready for new messages
		}
	}
	GICR |= (1<<INT0);
	Epilog();                // Return to tasks
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
	
	spi_init();
	
	mcp_reset();
	
	tx_fifo.size = TX_SIZE;
	tx_fifo.in = 0;
	tx_fifo.out = 0;
	
	AvrXResetSemaphore(&rx_mutex);
	
	delayloop();
	
	mcp_write(BFPCTRL,0x0C);//RXBF Pins to Output
	
	// 0x01 : 125kbit/8MHz
	// 0x03 : 125kbit/16MHz
	// 0x04 : 125kbit/20MHz
	
#	if F_MCP == 16000000L
#	define CNF1_T 0x03
#	elif F_MCP == 8000000L
#	define CNF1_T 0x01
#	elif F_MCP == 20000000L
#	define CNF1_T 0x04
#	else
#	error Can Baudrate is only defined for F_MCP 8, 16 and 20 MHz
#	endif

	//SJW=1, BLTMODE=1, SAM=1, PHSEG1=6, PHSEG2=5, PRSEG=1
	//CNF1 = SJW1-0:BRP5-0
	//CNF2 = BLTMODE:SAM:PHSEG12-PHSEG10:PRSEG2-PRSEG0
	//CNF3 = SOF:WAKFIL:X:X:X:PHSEG22-PHSEG20
	mcp_write( CNF1, 0x40 | CNF1_T );
	mcp_write( CNF2, 0xf1 );
	mcp_write( CNF3, 0x05 );

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write( CANINTE, (1<<RX0IE) );

	//the TX Interrupt is enabled when needed, so set flag that buffer is empty
	mcp_bitmod( CANINTF, (1<<TX0IF), 0xff );

	can_setfilter();
	can_setmode(normal);

#ifdef CLASSIC_ATMEL
	//this turns on INT0 on the Atmel
	MCUCR |=  (1<<ISC01);
	GIMSK |= (1<<INT0);
#else
	//this turns on INT0 on the Atmega
	//MCUCR |=  (1<<ISC01);
	GICR |= (1<<INT0);
#endif

}

void can_get(){
	AvrXWaitSemaphore(&rx_mutex);
	message_fetch();
	mcp_bitmod( CANINTE, (1<<RX0IE), 0xff); //interrupt back on
}

uint16_t can_put(can_message_t * msg){
	uint8_t t = tx_fifo.in+1;
	if (t >= tx_fifo.size)
		t = 0;
	if (t == tx_fifo.out)	// isFull()
		return FIFO_ERR;
	memcpy (&tx_fifo.buf[tx_fifo.in], msg, sizeof(can_message_t));
	tx_fifo.in = t;
	//AvrXSetSemaphore(&p->Producer);
	GICR &= ~(1<<INT0);
	mcp_bitmod(CANINTE, (1<<TX0IE), 0xff); //enable interrupt
	GICR |= (1<<INT0);
	return FIFO_OK;
}
