
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "canlib/spi.h"
#include "canlib/can.h"

#include "uart/uart.h"


typedef enum { RS232CAN_RESET=0x00,
		RS232CAN_SETFILTER=0x10, RS232CAN_PKT=0x11, RS232CAN_SETMODE=0x12, RS232CAN_ERROR=0x13
} rs232can_cmd;

#define RS232CAN_MAXLENGTH 20

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;


/*****************************************************************************
 * CAN to UART
 */
static void write_buffer_to_uart(char* buf, uint8_t len)
{
	uint8_t i;

	for (i = 0; i < len; i++) {
		uart_putc( *buf++);
	}
}

static void write_can_message_to_uart(can_message * cmsg) {
	uint8_t len = sizeof(can_message) + cmsg->dlc - 8; //actual size of can message

	uart_putc(RS232CAN_PKT);  //command
	uart_putc(len);           //length

	write_buffer_to_uart((char*)cmsg, len); //data

	uart_putc(0x23);		// XXX CRC
}
/*****************************************************************************/



/*****************************************************************************
 * Receive a message from uart non blocking.
 * Returns Message or 0 if there is no complete message.
 */ 

typedef enum {STATE_START, STATE_LEN, STATE_PAYLOAD} canu_rcvstate_t;

static rs232can_msg	canu_rcvpkt;
static canu_rcvstate_t	canu_rcvstate = STATE_START;
static unsigned char	canu_rcvlen   = 0;


static rs232can_msg *canu_get_nb() {
	static char *uartpkt_data;
	char c;

	while (uart_getc_nb(&c)) {
		#ifdef DEBUG
		printf("canu_get_nb received: %02x\n", c);
		#endif
		switch (canu_rcvstate) {
		case STATE_START:
			if (c) {
				canu_rcvstate = STATE_LEN;
				canu_rcvpkt.cmd = c;
			}
			break;
		case STATE_LEN:
			canu_rcvstate     = STATE_PAYLOAD;
			canu_rcvlen       = (unsigned char)c;
			canu_rcvpkt.len   = c;
			uartpkt_data      = &canu_rcvpkt.data[0];
			break;
		case STATE_PAYLOAD:
			if (canu_rcvlen--) {
				*(uartpkt_data++) = c;
			} else {
				canu_rcvstate = STATE_START;
				//check CRC
				if(c == 0x23){ // XXX CRC
					return &canu_rcvpkt;
				}
			}
			break;
		}
	}

	return NULL;
}

/*****************************************************************************/


static void process_cantun_msg(rs232can_msg *msg)
{
	can_message *cmsg;

	switch (msg->cmd) {
		case RS232CAN_SETFILTER:
			break;
		case RS232CAN_SETMODE:
			mcp_setmode(msg->data[0]);
			break;
		case RS232CAN_PKT:
			cmsg = can_buffer_get();                      //alocate buffer
			memcpy(cmsg, msg->data, sizeof(can_message)); //copy can message
			can_transmit(cmsg);                           //transmit it
			break;
	}
}

int main(void) {
	DDRB |= (1<<PB0); //LED-Pin to output

	uart_init();
	spi_init();
	can_init();

	sei();

	while (1) {
		rs232can_msg  *rmsg;
		can_message *cmsg;


		rmsg = canu_get_nb();
		if (rmsg) {
			PORTB ^= 0x01;
			process_cantun_msg(rmsg);
		}

		cmsg = can_get_nb();
		if (cmsg) {
			PORTB ^= 0x01;
			write_can_message_to_uart(cmsg);
			can_free(cmsg);
		}
	}
}
