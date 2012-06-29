#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "mcp2510regs.h"
#include "can.h"
#include "spi.h"
#include "../config.h"


//for backwards compatibility. Please use MCP_CS_PORT from now on!
#ifndef MCP_CS_PORT
	#warning please use MCP_CS_PORT and MCP_CS_BIT instead of MCP_CMD_PORT and MCP_CS !
	#define MCP_CS_PORT MCP_CMD_PORT
	#define MCP_CS_BIT  MCP_CS
#endif

#ifdef XMEGA
	#define SET_CS()   MCP_CS_PORT.OUTSET = _BV(MCP_CS_BIT)
	#define CLEAR_CS() MCP_CS_PORT.OUTCLR = _BV(MCP_CS_BIT)	
#else
	#define SET_CS()   MCP_CS_PORT |= _BV(MCP_CS_BIT)
	#define CLEAR_CS() MCP_CS_PORT &= ~_BV(MCP_CS_BIT)
#endif

typedef struct
{
	can_message msg;
	volatile unsigned char flags;
} can_message_x;


/* MCP */
static unsigned char mcp_status();
static void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val);
static void mcp_reset();

void mcp_write(unsigned char reg, unsigned char data);
unsigned char mcp_read(unsigned char reg);


// Functions

static unsigned char mcp_status()
{
	unsigned char d;
	CLEAR_CS();
	spi_send(READ_STATUS);
	d = spi_send(0);
	SET_CS();
	return d;
}

static void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val)
{
	CLEAR_CS();
	spi_send(BIT_MODIFY);
	spi_send(reg);
	spi_send(mask);
	spi_send(val);
	SET_CS();
}

//load a message to mcp2515 and start transmission
void message_load(can_message_x * msg)
{
	unsigned char x;
	
	CLEAR_CS();
	spi_send(WRITE);
	spi_send(TXB0SIDH);

#ifdef CAN_RAW
	spi_send((uint8_t)((uint32_t)msg->msg.id>>21));
	spi_send( ((uint8_t)((uint32_t)msg->msg.id>>13) & 0xE0) | (1<<EXIDE) | ((uint8_t)((uint32_t)msg->msg.id>>16)&0x03) );
	spi_send((uint8_t)(msg->msg.id>>8));
	spi_send((uint8_t)(msg->msg.id));
#else
	spi_send( ((unsigned char)(msg->msg.port_src << 2)) | (msg->msg.port_dst >> 4 ) );
	spi_send( (unsigned char)((msg->msg.port_dst & 0x0C) << 3) | (1<<EXIDE) | (msg->msg.port_dst & 0x03) );
	spi_send(msg->msg.addr_src);
	spi_send(msg->msg.addr_dst);
#endif
	
	spi_send(msg->msg.dlc);
	for(x=0;x<msg->msg.dlc;x++){
		spi_send(msg->msg.data[x]);
	}
	SET_CS();
	CLEAR_CS();
	spi_send(WRITE);
	spi_send(TXB0CTRL);
	spi_send((1<<TXREQ));
	SET_CS();
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(can_message_x * msg)
{
	unsigned char tmp1, tmp2, tmp3;
	unsigned char x;

	CLEAR_CS();
	spi_send(READ);
	spi_send(RXB0SIDH);
	
#ifdef CAN_RAW	
	tmp1 = spi_send(0);
	tmp2 = spi_send(0);
	tmp3 = spi_send(0);
	
	msg->msg.id = ((uint32_t)tmp1<<21) | ((uint32_t)((uint8_t)tmp2&0xE0)<<13) 
			| ((uint32_t)((uint8_t)tmp2&0x03)<<16) | ((uint16_t)tmp3<<8) | spi_send(0);
#else
	tmp1 = spi_send(0);
	msg->msg.port_src = tmp1 >> 2;
	tmp2 = spi_send(0);
	tmp3 = (unsigned char)((unsigned char)(tmp2 >> 3) & 0x0C);
	msg->msg.port_dst = ((unsigned char)(tmp1 <<4 ) & 0x30) | tmp3 | (unsigned char)(tmp2 & 0x03);
	msg->msg.addr_src = spi_send(0);
	msg->msg.addr_dst = spi_send(0);
#endif
	
	msg->msg.dlc = spi_send(0) & 0x0F;
	for (x = 0; x < msg->msg.dlc; x++)
	{
		msg->msg.data[x] = spi_send(0);
	}
	SET_CS();
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
}
#ifdef CAN_INTERRUPT

static can_message_x RX_BUFFER[CAN_RX_BUFFER_SIZE], TX_BUFFER[CAN_TX_BUFFER_SIZE];
unsigned char RX_HEAD=0;volatile unsigned char RX_TAIL=0;
unsigned char TX_HEAD= 0;volatile unsigned char TX_TAIL=0;
static volatile unsigned char TX_INT;

ISR (MCP_INT_VEC)
{
	unsigned char status = mcp_status();

	if (status & 0x01)
	{	// Message in RX0
		if (!(((can_message_x*)&RX_BUFFER[RX_HEAD])->flags & 0x01))
		{
			message_fetch(&RX_BUFFER[RX_HEAD]);
			RX_BUFFER[RX_HEAD].flags |= 0x01;//mark buffer as used
			if ( ++RX_HEAD == CAN_RX_BUFFER_SIZE) RX_HEAD = 0;
		} else
		{
			//buffer overflow
			//just clear the Interrupt condition, and lose the message
			mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
		}
	} else if ( status & 0x08 ) {	// TX1 empty
		if (((can_message_x*)&TX_BUFFER[TX_TAIL])->flags & 0x01)
		{
			((can_message_x*)&TX_BUFFER[TX_TAIL])->flags &= ~0x01;
			TX_INT = 1;
			message_load(&TX_BUFFER[TX_TAIL]);
			if (++TX_TAIL == CAN_TX_BUFFER_SIZE) TX_TAIL = 0;
		} else
		{
			TX_INT = 0;
		}
		mcp_bitmod(CANINTF, (1<<TX0IF), 0x00);
	} else
	{
#ifdef CAN_HANDLEERROR
		status = mcp_read(EFLG);

		if (status)
		{ // we've got a error condition
			can_error = status;

			mcp_write(EFLG, 0);
		}
#endif // CAN_HANDLEERROR
	}
}

#endif


static void mcp_reset()
{
	CLEAR_CS();
	spi_send(RESET);
	SET_CS();
}

void mcp_write(unsigned char reg, unsigned char data)
{
	CLEAR_CS();
	spi_send(WRITE);
	spi_send(reg);
	spi_send(data);
	SET_CS();
}

unsigned char mcp_read(unsigned char reg)
{
	unsigned char d;
	CLEAR_CS();
	spi_send(READ);
	spi_send(reg);
	d = spi_send(0);
	SET_CS();
	return d;
}


/* Management */
void can_setmode(can_mode_t mode)
{
	unsigned char val = mode << 5;  
	val |= 0x04;  // CLKEN

	mcp_write( CANCTRL, val );
}


void can_setfilter()
{
	//RXM1   RXM0
	//  0      0     receive matching filter
	//  0      1     " only 11bit Identifier
	//  1      0     " only 29bit Identifier
	//  1      1     any
	mcp_write(RXB0CTRL, (1<<RXM1) | (1<<RXM0));
}

void can_setled(unsigned char led, unsigned char state)
{
	mcp_bitmod(BFPCTRL, 0x10<<led, state?0xff:0);
}

/*******************************************************************/

#define DDR(port) (*((&(port))-1))

void can_init()
{
#ifdef XMEGA
	//set Slave select DDR to output
	MCP_CS_PORT.DIRSET = _BV(MCP_CS_BIT);
	//set Slave select high
	MCP_CS_PORT.OUTSET = _BV(MCP_CS_BIT);	
#else
	//set Slave select DDR to output
	DDR(MCP_CS_PORT) |= _BV(MCP_CS_BIT);
	//set Slave select high
	MCP_CS_PORT      |= _BV(MCP_CS_BIT);
#endif
	
#ifdef CAN_INTERRUPT	
	unsigned char x;
	for (x = 0; x < CAN_RX_BUFFER_SIZE; x++)
	{
		RX_BUFFER[x].flags = 0;
	}
	for (x = 0; x < CAN_TX_BUFFER_SIZE; x++)
	{
		TX_BUFFER[x].flags = 0;
	}
#endif	

#ifdef CAN_HANDLEERROR
	can_error = 0;
#endif	
	
	mcp_reset();
	
	_delay_ms(1);
	
	mcp_write(BFPCTRL, 0x0C);//RXBF Pins to Output
	
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

	can_setfilter();
	can_setmode(normal);

#ifdef CAN_INTERRUPT

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1<<RX0IE) | (1<<TX0IE));
	
#if defined (ENABLE_CAN_INT)
	ENABLE_CAN_INT();
#elif defined (__AVR_ATmega8__)
	//this turns on INT0
	MCUCR |= (1 << MCP_INT_FLAG);
	GIMSK |= (1 << MCP_INT_MASK);
#elif defined (__AVR_ATmega168__)
    EICRA |= (1 << MCP_INT_FLAG);
    EIMSK |= (1 << MCP_INT_MASK);
#else
	#error Interrup REG for Part not defined
#endif

#else  //CAN_INTERRUPT
	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1<<RX0IE) ); //only turn RX int on
#endif //CAN_INTERRUPT
}

#ifdef CAN_INTERRUPT
//returns next can message in buffer, or 0 Pointer if buffer is empty
can_message * can_get_nb()
{
	can_message_x *p;
	if (RX_HEAD == RX_TAIL)
	{
		return 0;
	} else
	{
		p = &RX_BUFFER[RX_TAIL];
		if (++RX_TAIL == CAN_RX_BUFFER_SIZE) RX_TAIL = 0;
		return &(p->msg);
	}
}

can_message * can_get()
{
	can_message_x *p;

	while (RX_HEAD == RX_TAIL) { };

	p = &RX_BUFFER[RX_TAIL];
	if (++RX_TAIL == CAN_RX_BUFFER_SIZE) RX_TAIL = 0;

	return &(p->msg);
}


//marks a receive buffer as unused again so it can be overwritten in Interrupt
void can_free(can_message * msg)
{
	can_message_x * msg_x = (can_message_x *) msg;
	msg_x->flags = 0;
}


//returns pointer to the next can TX buffer
can_message * can_buffer_get()
{
	can_message_x *p;
	p = &TX_BUFFER[TX_HEAD];
	while (p->flags&0x01); //wait until buffer is free
	if (++TX_HEAD == CAN_TX_BUFFER_SIZE) TX_HEAD = 0;
	return &(p->msg);
}


//start transmitting can messages, and mark message msg as transmittable
void can_transmit(can_message* msg2)
{
	can_message_x* msg=(can_message_x*) msg2;
	if (msg2)
	{
		msg->flags |= 0x01;
	}
	if (!TX_INT)
	{
		if (TX_BUFFER[TX_TAIL].flags & 0x01)
		{
			((can_message_x*)&TX_BUFFER[TX_TAIL])->flags &= ~0x01;
			TX_INT = 1;
			message_load(&TX_BUFFER[TX_TAIL]);
			if(++TX_TAIL == CAN_TX_BUFFER_SIZE) TX_TAIL = 0;
		}
	}
}

#else  // NON INTERRUPT VERSION 

can_message_x RX_MESSAGE, TX_MESSAGE;

can_message * can_get_nb()
{
	//check the pin, that the MCP's Interrup output connects to
#ifdef XMEGA
	if (SPI_REG_PIN_MCP_INT.IN & _BV(SPI_PIN_MCP_INT))
#else
	if (SPI_REG_PIN_MCP_INT    & _BV(SPI_PIN_MCP_INT))
#endif
	{
		return 0;
	} else
	{
		unsigned char status = mcp_status();

		if (status & 0x01)
		{
			//So the MCP Generates an RX Interrupt
			message_fetch(&RX_MESSAGE);

			return &(RX_MESSAGE.msg);
		}
		else
			return 0;
	}
}

can_message * can_get()
{
	//wait while the MCP doesn't generate an RX Interrupt
#ifdef XMEGA
	while (SPI_REG_PIN_MCP_INT.IN & _BV(SPI_PIN_MCP_INT));
#else
	while (SPI_REG_PIN_MCP_INT    & _BV(SPI_PIN_MCP_INT));
#endif

	message_fetch(&RX_MESSAGE);
	return &(RX_MESSAGE.msg);
}

	//only for compatibility with Interrupt driven Version
can_message * can_buffer_get()
{
	return &(TX_MESSAGE.msg);
}

void can_transmit(can_message * msg)
{
	message_load((can_message_x*)msg);
}

void can_free(can_message * msg)
{
}

#endif
