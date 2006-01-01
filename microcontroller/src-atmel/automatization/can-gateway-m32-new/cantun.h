#ifndef CANTUN_H
#define CANTUN_H

/*****************************************************************************
 * Encapsulate CAN packets for serial transport
 *
 * Use with UART, TCP/IP or alike.
 */

#include "can.h"

typedef enum { CANTUN_RESET=0x00,
		CANTUN_SETFILTER=0x10, CANTUN_PKT=0x11, CANTUN_SETMODE=0x12, CANTUN_ERROR=0x13
} cantun_cmd;


typedef struct {
	unsigned char len;
	unsigned char type;
	union {
		can_message_t msg;
	}
} cantun_msg;



//void rs232can_setmode(can_mode_t mode);
void cantun_wrap(cantun_msg *outmsg, can_message *inmsg);
void cantun_unwrap(can_message *outmsg, cantun_msg *inmsg);

#endif
