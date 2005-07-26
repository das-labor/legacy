#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "uart-host.h"
#include "rs232can.h"


//tries to assemble a rs232can_msg from the chars received on the Uart.
//Returns Message or 0 if there is no complete message.
rs232can_msg * rs232can_get_nb(){
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
			if(uartpkt_len--){
				*(uartpkt_data++) = c;
			}else{
				uartpkt_state = STATE_START;
				//check CRC
				if(c == 0x23){ // XXX CRC
					return &uartpkt;
				}
			}
			break;
		}
	}
	return 0;
}

void rs232can_put(rs232can_msg *msg)
{
	char *ptr = (char *)msg;
	unsigned char i;

	for (i=0; i<msg->len+2; i++) {
		uart_putc( *ptr++);
	}

	uart_putc(0x23);		// XXX CRC
}


void rs232can_reset()
{  
	unsigned char i;
	for(i=RS232CAN_MAXLENGTH+3; i>0; i--)
		uart_putc( (char)0x00 );
}

void rs232can_rs2can(can_message *cmsg, rs232can_msg *rmsg)
{
	memcpy(cmsg, rmsg->data, sizeof(can_message) );
}


void rs232can_can2rs(rs232can_msg *rmsg, can_message *cmsg)
{
	rmsg->cmd = RS232CAN_PKT;
	rmsg->len = (sizeof(can_message)-8) + cmsg->dlc;
}
