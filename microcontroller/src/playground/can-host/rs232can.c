#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "rs232can.h"


rs232can_msg * rs232can_get()
{
	static enum {STATE_START, STATE_LEN, STATE_PAYLOAD, STATE_CRC} uartpkt_state;
	static unsigned char uartpkt_len;
	static rs232can_msg  uartpkt;
	static char *uartpkt_data;

	char c;
	
	while (uart_getc_nb(&c)) {
		switch (uartpkt_state) {
		case STATE_START:
			if (c) {
				uartpkt_state = STATE_LEN;
				uartpkt.cmd   = c;
			}
			break;
		case STATE_LEN:
			uartpkt_state = STATE_PAYLOAD;
			uartpkt_len   = (unsigned char)c;
			uartpkt.len   = c;
			uartpkt_data  = &uartpkt.data[0];

			break;
		case STATE_PAYLOAD:
			do {
				*(uartpkt_data++) = c;
			} while (uart_getc_nb(&c));
			break;
		case STATE_CRC:
			if (c != 0x23)		// XXX CRC
				uartpkt_state = STATE_START;
				return &uartpkt;
			break;
		}

	}
	return 0;
}

void rs232can_put(rs232can_msg *msg)
{
	char *ptr = (char *)msg;
	unsigned char i;

	for (i=0; i<msg->len; i++) {
		uart_putc( *ptr++);
	}

	uart_putc(0x23);		// XXX CRC
}


void rs232can_reset()
{  
	unsigned char i;
	for(i=RS232CAN_MAXLENGTH+3; i>0; i++)
		uart_putc( (char)0x00 );
}

can_message *  rs232can_pkt2can(rs232can_msg *msg)
{
	can_message *out_msg = can_buffer_get();
	can_message *in_msg  = (can_message *)&(msg->data[0]);

	memcpy(out_msg, in_msg, msg->len);

	return out_msg;
}


rs232can_msg * rs232can_can2pkt(can_message *msg)
{
	static rs232can_msg out_msg;
	
	msg->flags = 0x01;			// ready to send

	out_msg.cmd = RS232CAN_PKT;
	out_msg.len = sizeof(can_message);
	
	return &out_msg;
}
