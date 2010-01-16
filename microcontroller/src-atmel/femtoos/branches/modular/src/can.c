#include "femtoos_code.h"
#ifdef CAN_THREAD


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>


#include "mcp2510regs.h"
#include "can.h"
#include "spi.h"
//#include "lap.h"

typedef struct
{
	can_message msg;
	volatile Tuint08 flags;
} can_message_x;


can_message_x RX_MESSAGE, TX_MESSAGE;

/* MCP */
static Tuint08 mcp_status();
static void mcp_bitmod(Tuint08 reg, Tuint08 mask, Tuint08 val);
static void mcp_reset();

void mcp_write(Tuint08 reg, Tuint08 data);
Tuint08 mcp_read(Tuint08 reg);


/*
  imports for size - no double
*/
void
_delay_loop_1(uint8_t __count)
{
        __asm__ volatile (
                "1: dec %0" "\n\t"
                "brne 1b"
                : "=r" (__count)
                : "0" (__count)
        );
}



#if F_MCP ==  8000000
#define delayonems() _delay_loop_1(255); \
  _delay_loop_1(255)
#elif  F_MCP == 16000000 
#define delayonems() _delay_loop_1(255); \
  _delay_loop_1(255); \
  _delay_loop_1(255); \
  _delay_loop_1(255)
#elif F_MCP == 20000000
#define delayonems() _delay_loop_1(255); \
  _delay_loop_1(255); \
  _delay_loop_1(255); \
  _delay_loop_1(255); \
  _delay_loop_1(255)
#else
#error delayonems not defined for F_MCP
#endif






// Functions

static Tuint08 mcp_status()
{
	Tuint08 d;
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(READ_STATUS);
	d = spi_send(0);
	MCP_CMD_PORT |= _BV(MCP_CS);
	return d;
}

static void mcp_bitmod(Tuint08 reg, Tuint08 mask, Tuint08 val)
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
	Tuint08 x;
	
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(WRITE);
	spi_send(TXB0SIDH);

	spi_send( ((Tuint08)(msg->msg.port_src << 2)) | (msg->msg.port_dst >> 4 ) );
	spi_send( (Tuint08)((msg->msg.port_dst & 0x0C) << 3) | (1<<EXIDE) | (msg->msg.port_dst & 0x03) );
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
	Tuint08 tmp1, tmp2, tmp3;
	Tuint08 x;
	taskEnterGlobalCritical();
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(READ);
	spi_send(RXB0SIDH);
	tmp1 = spi_send(0);
	msg->msg.port_src = tmp1 >> 2;
	tmp2 = spi_send(0);
	tmp3 = (Tuint08)((Tuint08)(tmp2 >> 3) & 0x0C);
	msg->msg.port_dst = ((Tuint08)(tmp1 <<4 ) & 0x30) | tmp3 | (Tuint08)(tmp2 & 0x03);
	msg->msg.addr_src = spi_send(0);
	msg->msg.addr_dst = spi_send(0);
	msg->msg.dlc = spi_send(0) & 0x0F;	
	for (x = 0; x < msg->msg.dlc; x++)
	{
		msg->msg.data[x] = spi_send(0);
	}
	MCP_CMD_PORT |= _BV(MCP_CS);
	
	mcp_bitmod(CANINTF, (1<<RX0IF), 0x00);
	taskExitGlobalCritical();
}


static void mcp_reset()
{
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(RESET);
	MCP_CMD_PORT |= _BV(MCP_CS);
}

void mcp_write(Tuint08 reg, Tuint08 data)
{
	MCP_CMD_PORT &= ~_BV(MCP_CS);
	spi_send(WRITE);
	spi_send(reg);
	spi_send(data);
	MCP_CMD_PORT |= _BV(MCP_CS);
}

Tuint08 mcp_read(Tuint08 reg)
{
	Tuint08 d;
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
  Tuint08 val = (Tuint08)((Tuint08)(mode) << 5);
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

void can_setled(Tuint08 led, Tuint08 state)
{
	mcp_bitmod(BFPCTRL, 0x10<<led, state?0xff:0);
}

/*******************************************************************/

void can_init()
{
	DDRB |= _BV(MCP_CS);
	//set Slave select high
	MCP_CMD_PORT &= ~_BV(MCP_CS);

	mcp_reset();
	
	delayonems();
	
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

	can_setfilter();
	can_setmode(normal);

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1<<RX0IE) ); //only turn RX int on
}

can_message * can_get_nb()
{
	//check the pin, that the MCP's Interrup output connects to
	if (SPI_REG_PIN_MCP_INT & _BV(SPI_PIN_MCP_INT))
	{
		return 0;
	} else
	{
		Tuint08 status = mcp_status();

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

#endif // CAN_THREAD
