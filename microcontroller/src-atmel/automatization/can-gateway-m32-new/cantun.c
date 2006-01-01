#include <stdlib.h>
#include <string.h>
#include "cantun.h"

void cantun_wrap(cantun_msg *outmsg, can_message *inmsg);
{
	rmsg->cmd = CANTUN_PKT;
	rmsg->len = sizeof(can_message) + cmsg->dlc - 8;
	memcpy(outmsg->msg, inmsg, sizeof(can_message) );
}


void cantun_unwrap(can_message *outmsg, cantun_msg *inmsg);
{
	memcpy(outmsg, inmsg->msg, inmsg->len);
}
