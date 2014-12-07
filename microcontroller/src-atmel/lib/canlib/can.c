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

#ifdef CAN_INTERRUPT
	#ifndef ENABLE_CAN_INT //makro in which user can implement enabling of AVR-interrupt - enable int and set control mask
		#if defined (__AVR_ATmega8__) || (__AVR_ATmega32__)
			#define	ENABLE_CAN_INT()   GIMSK |= _BV(MCP_INT_MASK)
		#elif defined (__AVR_ATmega168__) || (__AVR_ATmega644__)
			#define	ENABLE_CAN_INT()   EIMSK |= _BV(MCP_INT_MASK)
		#else
			#error Interrupt Enable for Part not defined
		#endif
	#endif

	#ifndef DISABLE_CAN_INT //makro in which user can implement disabling of AVR-interrupt
		#if defined (__AVR_ATmega8__) || (__AVR_ATmega32__)
			#define	DISABLE_CAN_INT()   GIMSK &= ~_BV(MCP_INT_MASK)
		#elif defined (__AVR_ATmega168__) || (__AVR_ATmega644__)
			#define	DISABLE_CAN_INT()   EIMSK &= ~_BV(MCP_INT_MASK)
		#else
			#error Interrupt Disable for Part not defined
		#endif
	#endif

	#ifndef SETUP_CAN_INT //makro in which user can implement setting up the interrupt to falling edge trigger
		#if defined (__AVR_ATmega8__) || (__AVR_ATmega32__)
			#define	SETUP_CAN_INT()   MCUCR |= _BV(MCP_INT_CTL)
		#elif defined (__AVR_ATmega168__) || (__AVR_ATmega644__)
			#define	SETUP_CAN_INT()   EICRA |= _BV(MCP_INT_CTL)
		#else
			#error Interrupt Enable for Part not defined
		#endif
	#endif

	#ifndef MCP_INT_VEC
		#error MCP_INT_VEC undefined. please define it in config.h like this: #define MCP_INT_VEC INT0_vect
	#endif
#endif

typedef struct
{
	can_message msg;
	volatile uint8_t flags;
} can_message_x;

#ifdef OPTIMISED_LAP
static void lap_message_from_can_message(can_message *cmsg);
static void can_message_from_lap_message(can_message *cmsg);
#endif // OPTIMISED_LAP

// MCP
static uint8_t mcp_status(void);
static void mcp_bitmod(uint8_t reg, uint8_t mask, uint8_t val);
static void mcp_reset(void);
static void mcp_setfilter(void);

void mcp_write(uint8_t reg, uint8_t data);
uint8_t mcp_read(uint8_t reg);


// Functions

static uint8_t mcp_status(void)
{
	uint8_t d;
	CLEAR_CS();
	spi_send(READ_STATUS);
	d = spi_send(0);
	SET_CS();
	return d;
}

static void mcp_bitmod(uint8_t reg, uint8_t mask, uint8_t val)
{
	CLEAR_CS();
	spi_send(BIT_MODIFY);
	spi_send(reg);
	spi_send(mask);
	spi_send(val);
	SET_CS();
}

// load a message to mcp2515 and start transmission
static void message_load(can_message_x *msg)
{
	uint8_t x;

	CLEAR_CS();
#ifdef MCP2510
	spi_send(WRITE);
	spi_send(TXB0SIDH);
#else
	spi_send(LOAD_TX_BUFFER);
#endif

#ifdef CAN_RAW
	spi_send((uint8_t) ((uint32_t) msg->msg.id >> 21));
	spi_send(((uint8_t) ((uint32_t) msg->msg.id >> 13) & 0xE0) | (1 << EXIDE) | ((uint8_t) ((uint32_t) msg->msg.id >> 16) & 0x03));
	spi_send((uint8_t)(msg->msg.id >> 8));
	spi_send((uint8_t)(msg->msg.id));
#elif defined(OPTIMISED_LAP)
	for (x = 0; x < 5 + msg->msg.dlc; x++)
		spi_send(msg->msg.can_msg_array[x]);
#else
	spi_send(((uint8_t) (msg->msg.port_src << 2)) | (msg->msg.port_dst >> 4));
	spi_send((uint8_t) ((msg->msg.port_dst & 0x0C) << 3) | (1 << EXIDE) | (msg->msg.port_dst & 0x03));
	spi_send(msg->msg.addr_src);
	spi_send(msg->msg.addr_dst);
#endif
#ifndef OPTIMISED_LAP
	spi_send(msg->msg.dlc);
	for (x = 0; x < msg->msg.dlc; x++)
		spi_send(msg->msg.data[x]);
#endif // OPTIMISED_LAP
	SET_CS();
	CLEAR_CS();
	spi_send(RTS | _BV(TXB0)); // base addr + TXB0
	SET_CS();
}

//get a message from mcp2515 and disable RX interrupt Condition
static void message_fetch(can_message_x *msg)
{
#ifndef OPTIMISED_LAP
	uint8_t tmp1, tmp2, tmp3;
#endif
	uint8_t x;

	CLEAR_CS();
#ifdef MCP2510
	spi_send(READ);
	spi_send(RXB0SIDH);
#else
	spi_send(READ_RX_BUFFER);
#endif

#ifdef CAN_RAW
	tmp1 = spi_send(0);
	tmp2 = spi_send(0);
	tmp3 = spi_send(0);

	msg->msg.id = ((uint32_t) tmp1 << 21) | ((uint32_t) ((uint8_t) tmp2 & 0xE0) << 13) 
			| ((uint32_t) ((uint8_t) tmp2 & 0x03) << 16) | ((uint16_t) tmp3 << 8) | spi_send(0);
#elif defined(OPTIMISED_LAP)
	for (x = 0; x < 5 + (msg->msg.dlc & 0x0f); x++)
		msg->msg.can_msg_array[x] = spi_send(0);
#else
	tmp1 = spi_send(0);
	msg->msg.port_src = tmp1 >> 2;
	tmp2 = spi_send(0);
	tmp3 = (uint8_t) ((uint8_t) (tmp2 >> 3) & 0x0C);
	msg->msg.port_dst = ((uint8_t) (tmp1 << 4 ) & 0x30) | tmp3 | (uint8_t) (tmp2 & 0x03);
	msg->msg.addr_src = spi_send(0);
	msg->msg.addr_dst = spi_send(0);
#endif
#ifndef OPTIMISED_LAP
	msg->msg.dlc = spi_send(0) & 0x0F;
	for (x = 0; x < msg->msg.dlc; x++)
		msg->msg.data[x] = spi_send(0);
#endif // OPTIMISED_LAP
	SET_CS();
#ifdef MCP2510 // CANINTF is cleard by Read RX Buffer instruction
	mcp_bitmod(CANINTF, (1 << RX0IF), 0x00);
#endif
}
#ifdef CAN_INTERRUPT

static can_message_x rx_buffer[CAN_RX_BUFFER_SIZE], tx_buffer[CAN_TX_BUFFER_SIZE];
volatile uint8_t rx_head = 0; volatile uint8_t rx_tail = 0;
uint8_t tx_head = 0; volatile uint8_t tx_tail = 0;
static volatile uint8_t tx_int;

#ifdef CAN_INT_NOBLOCK
ISR (MCP_INT_VEC, ISR_NOBLOCK)
#else
ISR (MCP_INT_VEC)
#endif
{
	#ifdef CAN_INT_NOBLOCK
		DISABLE_CAN_INT(); // if global interrupts are enabled during handler, disable our own interrupt so it doesn't nest
	#endif
	uint8_t status;
	do {
		status = mcp_status();

		if (status & _BV(CANINTF_RX0IF)) // Message in RX0
		{
			if (    (((rx_head + 1) % CAN_RX_BUFFER_SIZE) != rx_tail)         // avoid overflow
			    && !(((can_message_x *) &rx_buffer[rx_head])->flags & 0x01)  ) // don't overwrite message that is still in use
			{
				message_fetch(&rx_buffer[rx_head]);
				rx_buffer[rx_head].flags |= 0x01; // mark buffer as used
				if (++rx_head == CAN_RX_BUFFER_SIZE)
					rx_head = 0;
			} else
			{
				// buffer overflow
				// just clear the Interrupt condition, and lose the message
				mcp_bitmod(CANINTF, (1 << RX0IF), 0x00);
			}
		}
		if (status & _BV(CANINTF_TX0IF)) {	// TX0 empty
			if (((can_message_x *) &tx_buffer[tx_tail])->flags & 0x01)
			{
				((can_message_x *) &tx_buffer[tx_tail])->flags &= ~0x01;
				tx_int = 1;
				message_load(&tx_buffer[tx_tail]);
				if (++tx_tail == CAN_TX_BUFFER_SIZE)
					tx_tail = 0;
			} else
			{
				tx_int = 0;
			}
			mcp_bitmod(CANINTF, (1 << TX0IF), 0x00);
		}
#ifdef CAN_HANDLEERROR
		if (status & ~0x09) // there is no error information in read_status
		{
			status = mcp_read(EFLG);

			if (status)
			{ // we've got a error condition
				can_error = status;

				mcp_write(EFLG, 0);
			}
		}
#endif // CAN_HANDLEERROR

	} while (status);

	#ifdef CAN_INT_NOBLOCK
		cli();
		ENABLE_CAN_INT(); //if global interrupts are enabled during handler, enable our own interrupt here again, but with globals off
	#endif
}

#endif // CAN_INTERRUPT


static void mcp_reset(void)
{
	CLEAR_CS();
	spi_send(RESET);
	SET_CS();
}

void mcp_write(uint8_t reg, uint8_t data)
{
	CLEAR_CS();
	spi_send(WRITE);
	spi_send(reg);
	spi_send(data);
	SET_CS();
}

uint8_t mcp_read(uint8_t reg)
{
	uint8_t d;
	CLEAR_CS();
	spi_send(READ);
	spi_send(reg);
	d = spi_send(0);
	SET_CS();
	return d;
}


/* Management */
void mcp_setmode(uint8_t mode)
{
	uint8_t val = mode << 5;
	val |= _BV(CLKEN);  // CLKEN

	mcp_write(CANCTRL, val);
}


static void mcp_setfilter(void)
{
	//RXM1   RXM0
	//  0      0     receive matching filter
	//  0      1     " only 11bit Identifier
	//  1      0     " only 29bit Identifier
	//  1      1     any
	mcp_write(RXB0CTRL, (1 << RXM1) | (1 << RXM0)); //  | (1 << BUKT) XXX use both rx buffers
}

void mcp_setled(uint8_t led, uint8_t state)
{
	mcp_bitmod(BFPCTRL, 0x10 << led, state ? 0xff : 0);
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
	uint8_t x;
	for (x = 0; x < CAN_RX_BUFFER_SIZE; x++)
	{
		rx_buffer[x].flags = 0;
	}
	for (x = 0; x < CAN_TX_BUFFER_SIZE; x++)
	{
		tx_buffer[x].flags = 0;
	}
#endif //CAN_INTERRUPT

#ifdef CAN_HANDLEERROR
	can_error = 0;
#endif

	_delay_ms(10);

	mcp_reset();

	_delay_ms(10);

	mcp_write(BFPCTRL, _BV(B1BFE) | _BV(B0BFE)); // RXBF Pins to Output

	// 0x01 : 125kbit/8MHz
	// 0x03 : 125kbit/16MHz
	// 0x04 : 125kbit/20MHz

#if F_MCP == 8000000
#define CNF1_T 0x01
#elif F_MCP == 16000000
#define CNF1_T 0x03
#elif F_MCP == 20000000
#define CNF1_T 0x04
#else
#error Can Baudrate is only defined for 8, 16 and 20 MHz
#endif
	mcp_write(CNF1, _BV(SJW0) | CNF1_T); // SJW: Synchronization Jump Width Length bits - 1 ms / TQ = 0,5 ms
	mcp_write(CNF2, (_BV(BTLMODE) | _BV(SAM) | _BV(PHSEG12) | _BV(PHSEG11) | _BV(PRSEG0))); // Length of PS2 determined by PHSEG, Bus line is sampled three times at the sample point, PS1 Length - 3,5 ms, Propagation Segment Length - 1 ms
	mcp_write(CNF3, (_BV(PHSEG22) | _BV(PHSEG20))); // (PHSEG2 + 1) x TQ  PS2 Lenght - 3 ms
	// bittime = t1 + propseg + ps1 + ps2 = 8 -  1 / 8 = 0,125

	mcp_setfilter();
	mcp_setmode(NORMAL);

#ifdef CAN_INTERRUPT

	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the AVR
	mcp_write(CANINTE, (1 << RX0IE) | (1 << TX0IE));

	SETUP_CAN_INT();//makro in which user can implement setting up the AVR-interrupt to trigger on falling edge
	ENABLE_CAN_INT();//makro in which user can implement enabling of AVR-interrupt

#else  //!CAN_INTERRUPT
	// configure IRQ
	// this only configures the INT Output of the mcp2515, not the int on the Atmel
	mcp_write(CANINTE, (1 << RX0IE)); //only turn RX int on
#endif //CAN_INTERRUPT
}

#ifdef CAN_INTERRUPT
//returns next can message in buffer, or 0 Pointer if buffer is empty
can_message *can_get_nb()
{
	can_message_x *p;
	if (rx_head == rx_tail)
	{
		return 0;
	} else
	{
		p = &rx_buffer[rx_tail];
		if (++rx_tail == CAN_RX_BUFFER_SIZE)
			rx_tail = 0;
#ifdef OPTIMISED_LAP
		lap_message_from_can_message(&(p->msg));
#endif // OPTIMISED_LAP
		return &(p->msg);
	}
}

can_message *can_get()
{
	can_message_x *p;

	while (rx_head == rx_tail);

	p = &rx_buffer[rx_tail];
	if (++rx_tail == CAN_RX_BUFFER_SIZE)
		rx_tail = 0;
#ifdef OPTIMISED_LAP
	lap_message_from_can_message(&(p->msg));
#endif // OPTIMISED_LAP
	return &(p->msg);
}


//marks a receive buffer as unused again so it can be overwritten in Interrupt
void can_free(can_message *msg)
{
	can_message_x *msg_x = (can_message_x *) msg;
	msg_x->flags = 0;
}


//returns pointer to the next can TX buffer
can_message *can_buffer_get()
{
	can_message_x *p;
	p = &tx_buffer[tx_head];
	while (p->flags & 0x01); //wait until buffer is free
	if (++tx_head == CAN_TX_BUFFER_SIZE)
		tx_head = 0;
	return &(p->msg);
}


//start transmitting can messages, and mark message msg as transmittable
void can_transmit(can_message *msg2)
{
#ifdef OPTIMISED_LAP
	can_message_from_lap_message(msg2);
#endif // OPTIMISED_LAP
	can_message_x *msg = (can_message_x *) msg2;
	if (msg2)
	{
		msg->flags |= 0x01;
	}
	if (!tx_int)
	{
		if (tx_buffer[tx_tail].flags & 0x01)
		{
			((can_message_x *) &tx_buffer[tx_tail])->flags &= ~0x01;
			tx_int = 1;
			DISABLE_CAN_INT(); // no INT with SPI transfer in msg_load
			message_load(&tx_buffer[tx_tail]);
			ENABLE_CAN_INT();
			if (++tx_tail == CAN_TX_BUFFER_SIZE)
				tx_tail = 0;
		}
	}
}

#else  // NON INTERRUPT VERSION

can_message_x rx_message, tx_message;

can_message *can_get_nb()
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
		uint8_t status = mcp_status();

		if (status & 0x01) // switch XXX dual rx buffers
		{
			//So the MCP Generates an RX Interrupt
			message_fetch(&rx_message);
#ifdef OPTIMISED_LAP
			lap_message_from_can_message((can_message *) &rx_message);
#endif // OPTIMISED_LAP
			return &(rx_message.msg);
		}
		else
			return 0;
	}
}

can_message *can_get()
{
	//wait while the MCP doesn't generate an RX Interrupt
#ifdef XMEGA
	while (SPI_REG_PIN_MCP_INT.IN & _BV(SPI_PIN_MCP_INT));
#else
	while (SPI_REG_PIN_MCP_INT    & _BV(SPI_PIN_MCP_INT));
#endif
	message_fetch(&rx_message);
#ifdef OPTIMISED_LAP
	lap_message_from_can_message((can_message *) &rx_message);
#endif // OPTIMISED_LAP
	return &(rx_message.msg);
}

// only for compatibility with Interrupt driven Version
can_message *can_buffer_get()
{
	return &(tx_message.msg);
}

void can_transmit(can_message *msg)
{
	static uint8_t not_first;
	if (!not_first) {
		not_first = 1; // first call: no message to wait for
	} else {
		while ((mcp_status() & 0x08) == 0); // wait until last packet transmitted
		mcp_bitmod(CANINTF, (1 << TX0IF), 0x00); // clear interrupt
	}
#ifdef OPTIMISED_LAP
	can_message_from_lap_message(msg);
#endif // OPTIMISED_LAP
	message_load((can_message_x *) msg);
}

void can_free(can_message *msg)
{
	(void) msg; // silence warning
}

#endif //CAN_INTERRUPT

#ifdef OPTIMISED_LAP
static void lap_message_from_can_message(can_message *cmsg)
{
	uint8_t temp1 = cmsg->port_src, temp2 = cmsg->port_dst;
	cmsg->port_src = temp1 >> 2;
	cmsg->port_dst = ((uint8_t) (temp1 << 4 ) & 0x30) | ((uint8_t) (temp2 >> 3) & 0x0C) | ((uint8_t) (temp2 & 0x03));
}

static void can_message_from_lap_message(can_message *cmsg)
{
	uint8_t temp1 = cmsg->port_src, temp2 = cmsg->port_dst;
	cmsg->port_src = ((uint8_t) (temp1 << 2)) | (temp2 >> 4);
	cmsg->port_dst = (uint8_t) ((temp2 & 0x0C) << 3) | (1 << EXIDE) | (temp2 & 0x03);
}
#endif // OPTIMISED_LAP
