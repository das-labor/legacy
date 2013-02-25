#include <stdlib.h>
#include <string.h>
#include "can-encap.h"

void rs232can_rs2can(can_message_raw *cmsg, rs232can_msg *rmsg)
{
	memcpy(cmsg, rmsg->data, sizeof(can_message_raw) );
}


void rs232can_can2rs(rs232can_msg *rmsg, can_message_raw *cmsg)
{
	rmsg->cmd = RS232CAN_PKT;
	rmsg->len = sizeof(can_message_raw) + cmsg->dlc - 8;
	memcpy(rmsg->data, cmsg, rmsg->len);
}
