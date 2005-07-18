#ifndef RS232CAN_H
#define RS232CAN_H

#include "config.h"
#include "can.h"
#include "lap.h"

#define RS232CAN_MAXLENGTH 20

typedef enum { RS232CAN_RESET=0x00,
		RS232CAN_SETFILTER=0x10, RS232CAN_PKT=0x11, RS232CAN_SETMODE=0x12
	     } rs232can_cmd;

typedef struct {
	unsigned char cmd;
	unsigned char len;
	char data[RS232CAN_MAXLENGTH];
} rs232can_msg;

void rs232can_reset();

rs232can_msg * rs232can_get();
void rs232can_put(rs232can_msg *msg);

void rs232can_setmode(mcp_mode_t mode);
can_message *  rs232can_pkt2can(rs232can_msg *msg);
rs232can_msg * rs232can_can2pkt(can_message *msg);

#endif
