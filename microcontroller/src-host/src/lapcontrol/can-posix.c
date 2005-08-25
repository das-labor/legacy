#include <stdlib.h>
#include <string.h>

#include "can.h"
#include "can-encap.h"
#include "cann.h"
#include "can-uart.h"

/***************************************************************************
 * Implement can.h for posix systems 
 *
 * Uses cann.[hc] for cand or can-uart.[hc] according to first call to 
 * can_init( ... );
 */


static can_message Cmsg;
static cann_conn_t *conn;

/************************************************************************+**
 * Management 
 */

void can_init(cann_conn_t *aconn){
	conn = aconn;
}

void can_setmode( can_mode_t mode ) {
	// XXX
}

void can_setfilter() {
	// XXX
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

	rs232can_can2rs(&rmsg, cmsg);

	if (conn)
		cann_transmit(conn, &rmsg);
	else
		canu_transmit(&rmsg);
}

/****************************************************************************
 * reciving
 */

//returns next can message, or 0 if no Message was received.
can_message * can_get_nb(){
	rs232can_msg *rmsg;
	can_message  *cmsg;

	if (conn)
		rmsg = cann_get_nb(conn);
	else
		rmsg = canu_get_nb();

	cmsg = (can_message *)malloc(sizeof(can_message));
	rs232can_rs2can(cmsg, rmsg);

	return cmsg;
}

//returns next can message
can_message * can_get(){
	rs232can_msg *rmsg;
	can_message  *cmsg;

	if (conn)
		rmsg = cann_get(conn);
	else
		rmsg = canu_get();

	cmsg = (can_message *)malloc(sizeof(can_message));
	rs232can_rs2can(cmsg, rmsg);

	return cmsg;
}

