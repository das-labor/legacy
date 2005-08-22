
#include "can.h"
#include "cann.h"
#include "rs232can.h"

static can_message Cmsg;
static cann_conn_t *conn;

/************************************************************************+**
 * Management 
 */

void can_setmode( can_mode_t mode ) {
}

void can_setfilter() {
}

// open connection to cand
void can_init( char *server, int port ){
	conn = cann_connect(server, port);
}

/****************************************************************************
 * Sending
 */

//returns pointer to the next can TX buffer
can_message * can_buffer_get(){
	return &Cmsg;
}


//transmit a can message
void can_transmit(can_message *cmsg)
{
	rs232can_msg rmsg;

	rs232_can2rs(&rmsg, cmsg);
	cann_transmit(&rmsg);
}

/****************************************************************************
 * reciving
 */

//returns next can message, or 0 if no Message was received.
can_message * can_get_nb(){
	rs232can_msg *rmsg;
	can_message  *cmsg;

	rmsg = cann_get_nb(conn);

	cmsg = (can_message *)malloc(sizeof(can_message));
	rs232_rs2can(cmsg, rmsg);

	return cmsg;
}

//returns next can message
can_message * can_get(){
	rs232can_msg *rmsg;
	can_message  *cmsg;

	rmsg = cann_get(conn);

	cmsg = (can_message *)malloc(sizeof(can_message));
	rs232_rs2can(cmsg, rmsg);

	return cmsg;
}

