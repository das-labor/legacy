#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "can.h"
#include "can-uart.h"
// #include "can-encap.h"

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
	for(i=16; i>0; i--)
		uart_putc( (char)0 );
}

/*****************************************************************************
 * rcv
 */

// Returns 0 on succsess or -1 if there is no complete message.
int canu_get_nb(can_message_t *msg){
	static char buf[16];
	static uint8_t len;
	char c, *p;
	
	while (uart_getc_nb(&c)) {
		if (len == 0) {
		 	len = (uint8_t)c;
			printf( "newpkt, len: %d\n", len );
			p = buf;
			if (len > 16) len = 0;
			continue;
		}

		*p++ = c; len--; 
	
		if (len == 0) {
			switch(buf[0]) {
			case 0: // NOP
				break;
			case 1: // rvc packet
				memcpy( msg, buf+1, sizeof(can_message_t) );
				return 0;
			default:
				canu_reset();
				break;
			}
		}
	}

	return -1;
}

void canu_get(can_message_t *msg){
	int ret;
	fd_set rset;

	for(;;) {
		FD_ZERO(&rset);
		FD_SET(uart_fd, &rset);

		ret = select(uart_fd + 1, &rset, (fd_set*)NULL, (fd_set*)NULL, NULL);
		debug_assert( ret >= 0, "canu_get: select failed" );

		if (!canu_get_nb(msg))
			return;
	}
}

/*****************************************************************************
 * Transmit
 */
void canu_send(can_message_t *msg)
{
	char *p = (char *)msg;
	uint8_t len, i;

	len = 1 + 4 + 1 + msg->dlc;   // len = cmd + id[4] + dlc + data 

	uart_putc(len);   // len
	uart_putc(1);     // cmd
	
	for (i=len-1; i>0; i--)
		uart_putc( *p++);
}


