#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "mcp2510.h"
#include "mcp2510regs.h"
#include "../spi.h"
#include "../config.h"
#include "can.h"


#define cs_low()   PIN_SET(SPI_CS_MCP); 
#define cs_high() PIN_CLEAR(SPI_CS_MCP);


typedef struct {
	can_message_raw msg;
	volatile unsigned char flags;
} can_message_x;


/* MCP */
void mcp_reset();
void mcp_write(unsigned char reg, unsigned char data);
unsigned char mcp_read(unsigned char reg);
void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val);
unsigned char mcp_status();

// Functions

unsigned char mcp_status() {
	unsigned char d;
	cs_high();
	spi_send(READ_STATUS);
	d = spi_send(0);
	cs_low();
	return d;
}

void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val) {
	cs_high();
	spi_send(BIT_MODIFY);
	spi_send(reg);
	spi_send(mask);
	spi_send(val);
	cs_low();
}

//load a message to mcp2515 and start transmission
void message_load(can_message_x * msg) {
	unsigned char x;
	
	cs_high();
	spi_send(WRITE);
	spi_send(TXB0SIDH);

	spi_send((uint8_t)((uint32_t)msg->msg.id >> 21));
	spi_send(((uint8_t)((uint32_t)msg->msg.id >> 13) & 0xE0) | (1<<EXIDE) | ((uint8_t)((uint32_t)msg->msg.id >> 16)& 0x03));
	spi_send((uint8_t)(msg->msg.id >> 8));
	spi_send((uint8_t)(msg->msg.id));
	
	spi_send(msg->msg.dlc);
	for (x = 0; x < msg->msg.dlc; x++) {
		spi_send(msg->msg.data[x]);
	}
	cs_low();
	cs_high();
	spi_send(WRITE);
	spi_send(TXB0CTRL);
	spi_send( (1<<TXREQ) );
	cs_low();
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(can_message_x * msg) {
	unsigned char tmp1, tmp2, tmp3;
	unsigned char x;

	cs_high();
	spi_send(READ);
	spi_send(RXB0SIDH);
	tmp1 = spi_send(0);
	tmp2 = spi_send(0);
	tmp3 = spi_send(0);
	
	msg->msg.id = ((uint32_t)tmp1 << 21) | ((uint32_t)((uint8_t)tmp2 & 0xE0) << 13) 
			| ((uint32_t)((uint8_t)tmp2 & 0x03) << 16) | ((uint16_t)tmp3 << 8) | spi_send(0);
	
	msg->msg.dlc = spi_send(0) & 0x0F;	
	for (x = 0; x < msg->msg.dlc; x++) {
		msg->msg.data[x] = spi_send(0);
	}
	cs_low();
	
	mcp_bitmod(CANINTF, (1 << RX0IF), 0x00);
}

#ifdef CAN_INTERRUPT
static can_message_x RX_BUFFER[CAN_RX_BUFFER_SIZE], TX_BUFFER[CAN_TX_BUFFER_SIZE];
unsigned char RX_HEAD = 0;
volatile unsigned char RX_TAIL = 0;
unsigned char TX_HEAD = 0;
volatile unsigned char TX_TAIL = 0;
static volatile unsigned char TX_INT;

ISR (MCP_INT_VEC) {
	unsigned char status = mcp_status();
		
	if (status & 0x01) {	// Message in RX0
		if ( !(((can_message_x*)&RX_BUFFER[RX_HEAD])->flags & 0x01) ) {
			message_fetch(&RX_BUFFER[RX_HEAD]);
			RX_BUFFER[RX_HEAD].flags |= 0x01;//mark buffer as used
			if( ++RX_HEAD == CAN_RX_BUFFER_SIZE)
			    RX_HEAD = 0;
		} else {
			//buffer overflow
			//just clear the Interrupt condition, and lose the message
			mcp_bitmod(CANINTF, (1 << RX0IF), 0x00);
		}
	} else if (status & 0x08) {	// TX1 empty
		if (((can_message_x*)&TX_BUFFER[TX_TAIL])->flags & 0x01) {
			((can_message_x*)&TX_BUFFER[TX_TAIL])->flags &= ~0x01;
			TX_INT = 1;
			message_load(&TX_BUFFER[TX_TAIL]);
			if(++TX_TAIL == CAN_TX_BUFFER_SIZE)
			    TX_TAIL = 0;
		} else {
			TX_INT = 0;
		}
		mcp_bitmod(CANINTF, (1 << TX0IF), 0x00);
	} else {
#ifdef CAN_HANDLEERROR
		status = mcp_read(EFLG);

		if(status) { // we've got a error condition
			can_error = status;
			mcp_write(EFLG, 0x00);
		}
#endif // CAN_HANDLEERROR
	}
}

#endif

void mcp_reset() {
	cs_high();
	spi_send(RESET);
	cs_low();
}

void mcp_write(unsigned char reg, unsigned char data) {
	cs_high();
	spi_send(WRITE);
	spi_send(reg);
	spi_send(data);
	cs_low();
}

unsigned char mcp_read(unsigned char reg) {
	unsigned char d;
	cs_high();
	spi_send(READ);
	spi_send(reg);
	d = spi_send(0);
	cs_low();
	return d;
}

/* Management */
void can_setmode(can_mode_t mode) {
	unsigned char val = mode << 5;
	val |= 0x04;  // CLKEN

	mcp_write (CANCTRL, val);
}


void can_setfilter()
{
	//RXM1   RXM0
	//  0      0     receive matching filter
	//  0      1     " only 11bit Identifier
	//  1      0     " only 29bit Identifier
	//  1      1     any
	mcp_write(RXB0CTRL, (1 << RXM1) | (1 << RXM0));
}

void can_setled(unsigned char led, unsigned char state) {
	mcp_bitmod(BFPCTRL, 0x10 << led, state ? 0xff : 0x00);
}

/*******************************************************************/

void mcp_init()
{

#ifdef CAN_INTERRUPT
	unsigned char x;
	for (x = 0; x < CAN_RX_BUFFER_SIZE; x++) {
		RX_BUFFER[x].flags = 0;
	}
	for (x = 0; x < CAN_TX_BUFFER_SIZE; x++) {
		TX_BUFFER[x].flags = 0;
	}

#endif

#ifdef CAN_HANDLEERROR
	can_error = 0;
#endif
	
	mcp_reset();
	
	_delay_ms(250);
	
	mcp_write(BFPCTRL, 0x0C);   //RXBF Pins to Output
	
	// 0x01 : 125kbit/8MHz
	// 0x03 : 125kbit/16MHz
	// 0x04 : 125kbit/20MHz
#ifndef F_MCP
#error "please define F_MCP!"
#endif
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
	mcp_write(CANINTE, (1 << RX0IE) | (1 << TX0IE));

	can_setfilter();
	can_setmode(normal);

#ifdef CAN_INTERRUPT

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
//	mcp_write( CANINTE, (1<<RX0IE) | (1<<TX0IE) );


#if ATMEGA
	//this turns on INT0 on the Atmega
	GICR |= (1 << MCP_INT_MASK);
#else
	//this turns on INT0 on the Atmel
	MCUCR |= (1 << MCP_INT_FLAG);
	GIMSK |= (1 << MCP_INT_MASK);
#endif //ATMEGA

#else  //CAN_INTERRUPT
	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1 << RX0IE)); //only turn RX int on
#endif //CAN_INTERRUPT
}

//returns next can message in buffer, or 0 Pointer if buffer is empty
can_message_raw * can_get_nb_raw() {
	can_message_x *p;
	if (RX_HEAD == RX_TAIL) {
		return 0;
	} else {
		p = &RX_BUFFER[RX_TAIL];
		if (++RX_TAIL == CAN_RX_BUFFER_SIZE)
            RX_TAIL = 0;
		return &(p->msg);
	}
}

//marks a receive buffer as unused again so it can be overwritten in Interrupt
void can_free_raw(can_message_raw* msg) {
	can_message_x * msg_x = (can_message_x *) msg;
	msg_x->flags = 0;
}


//returns pointer to the next can TX buffer
can_message_raw * can_buffer_get_raw() {
	can_message_x *p;
	p = &TX_BUFFER[TX_HEAD];
	while (p->flags&0x01); //wait until buffer is free
	if (++TX_HEAD == CAN_TX_BUFFER_SIZE)
	    TX_HEAD = 0;
	return &(p->msg);
}


//start transmitting can messages, and mark message msg as transmittable
void can_transmit_raw(can_message_raw* msg2) {
	can_message_x* msg = (can_message_x*) msg2;
	if (msg) {
		msg->flags |= 0x01;
	}
	if (!TX_INT) {
		if (((can_message_x*)&TX_BUFFER[TX_TAIL])->flags & 0x01) {
			((can_message_x*)&TX_BUFFER[TX_TAIL])->flags &= ~0x01;
			TX_INT = 1;
			message_load(&TX_BUFFER[TX_TAIL]);
			if (++TX_TAIL == CAN_TX_BUFFER_SIZE)
                TX_TAIL = 0;
		}
	}
}

