#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "can.h"
#include "can-uart.h"
#include "can-encap.h"

#ifdef UART_HOST
#include "uart-host.h"
#else
#include "uart.h"
#endif

/*****************************************************************************
 * Connection management
 */

// Initialize CAN over UART on port serial
void canu_init(char *serial)
{
	uart_init(serial);
	canu_reset();
}

// syncronize line
void canu_reset()
{  
	unsigned char i;
	for(i=RS232CAN_MAXLENGTH+3; i>0; i--)
		uart_putc( (char)0x00 );
}


/*****************************************************************************
 * rcv
 */

//Returns Message or 0 if there is no complete message.
rs232can_msg * canu_get_nb(){
	static enum {STATE_START, STATE_LEN, STATE_PAYLOAD} uartpkt_state;
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

/*****************************************************************************
 * transmit
 */
void canu_transmit(rs232can_msg *msg)
{
	char *ptr = (char *)msg;
	unsigned char i;

	for (i=0; i<msg->len+2; i++) {
		uart_putc( *ptr++);
	}

	uart_putc(0x23);		// XXX CRC
}


