#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "mcp2510regs.h"
#include "can.h"
#include "spi.h"
#include "../config.h"

typedef struct
{
	can_message msg;
	volatile unsigned char flags;
} can_message_x;

static uint8_t myaddr;

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
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(READ_STATUS);
	d = spi_send(0);
	MCP_CMD_PORT |= _BV(MCP_CS);
	return d;
}

static void mcp_bitmod(unsigned char reg, unsigned char mask, unsigned char val)
{
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(BIT_MODIFY);
	spi_send(reg);
	spi_send(mask);
	spi_send(val);
	MCP_CMD_PORT |= _BV(MCP_CS);
}

//load a message to mcp2515 and start transmission
void message_load(can_message_x * msg)
{
	unsigned char x;
	
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(WRITE);
	spi_send(TXB0SIDH);

	spi_send( ((unsigned char)(msg->msg.port_src << 2)) | (msg->msg.port_dst >> 4 ) );
	spi_send( (unsigned char)((msg->msg.port_dst & 0x0C) << 3) | (1<<EXIDE) | (msg->msg.port_dst & 0x03) );
	spi_send(msg->msg.addr_src);
	spi_send(msg->msg.addr_dst);
	spi_send(msg->msg.dlc);
	for(x=0;x<msg->msg.dlc;x++){
		spi_send(msg->msg.data[x]);
	}
	MCP_CMD_PORT |= _BV(MCP_CS);
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(WRITE);
	spi_send(TXB0CTRL);
	spi_send((1<<TXREQ));
	MCP_CMD_PORT |= _BV(MCP_CS);
}

//get a message from mcp2515 and disable RX interrupt Condition
void message_fetch(can_message_x * msg)
{
	unsigned char tmp1, tmp2, tmp3;
	unsigned char x;

	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(READ);
	spi_send(RXB0SIDH);
	tmp1 = spi_send(0);
	msg->msg.port_src = tmp1 >> 2;
	tmp2 = spi_send(0);
	tmp3 = (unsigned char)((unsigned char)(tmp2 >> 3) & 0x0C);
	msg->msg.port_dst = ((unsigned char)(tmp1 <<4 ) & 0x30) | tmp3 | (unsigned char)(tmp2 & 0x03);
	msg->msg.addr_src = spi_send(0);
	msg->msg.addr_dst = spi_send(0);
	msg->msg.dlc = spi_send(0) & 0x0F;	
	for (x = 0; x < msg->msg.dlc; x++)
	{
		msg->msg.data[x] = spi_send(0);
	}
	MCP_CMD_PORT |= _BV(MCP_CS);
	
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
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(RESET);
	MCP_CMD_PORT |= _BV(MCP_CS);
}

void mcp_write(unsigned char reg, unsigned char data)
{
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(WRITE);
	spi_send(reg);
	spi_send(data);
	MCP_CMD_PORT |= _BV(MCP_CS);
}

unsigned char mcp_read(unsigned char reg)
{
	unsigned char d;
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(READ);
	spi_send(reg);
	d = spi_send(0);
	MCP_CMD_PORT |= _BV(MCP_CS);
	return d;
}


/* Management */
void can_setmode( can_mode_t mode )
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

void can_init()
{
	DDRB |= _BV(MCP_CS);
	//set Slave select high
	MCP_CMD_PORT &= ~_BV(MCP_CS);

	
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
	myaddr = eeprom_read_byte(0x00);
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

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1<<RX0IE));
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
	can_setfilter();
	can_setmode(normal);

#ifdef CAN_INTERRUPT

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1<<RX0IE) | (1<<TX0IE));
	
#if defined (__AVR_ATmega8__)
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
	if (RX_HEAD == RX_TAIL
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
	if (msg)
	{
		msg->flags |= 0x01;
	}
	if (!TX_INT)
	{
		if (((can_message_x*)&TX_BUFFER[TX_TAIL])->flags & 0x01)
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
	if (SPI_REG_PIN_MCP_INT & _BV(SPI_PIN_MCP_INT))
	{
		return 0;
	} else
	{
		unsigned char status = mcp_status();
			
		if (status & 0x01)
		{
			PORTA |= _BV(PA4);
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
	while (SPI_REG_PIN_MCP_INT & (1<<SPI_PIN_MCP_INT)) { };
	
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
