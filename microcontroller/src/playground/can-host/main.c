#include <stdio.h>
#include <stdlib.h>

#include "config.h"

#include "uart-host.h"
#include "can.h"
#include "rs232can.h"

static can_mode_t can_mode = normal;

void process_uart_msg( rs232can_msg *msg )
{
	switch((rs232can_cmd)msg->cmd) {
	case RS232CAN_RESET:
		// SHOULD NOT HAPPEN
		 break;
	case RS232CAN_SETFILTER:
		break;
	case RS232CAN_SETMODE:
		can_mode = (can_mode_t)(msg->data[0]);
		can_setmode(can_mode);
		break;
	case RS232CAN_PKT:
//		can_message *can_msg = rs232can_pkt2can(msg);
//can_transmit(can_msg);
		break;
	}
}

int main(){
	uart_init();
	can_init();

	rs232can_reset();
//	can_setmode(mcp_mode);

	while(1) {
		rs232can_msg *uart_msg;
		can_message  *can_msg;


		uart_msg = rs232can_get_nb();
		if (uart_msg) {
			printf( "Got Packet!\n" );
		}
	}

	return 0;
}
