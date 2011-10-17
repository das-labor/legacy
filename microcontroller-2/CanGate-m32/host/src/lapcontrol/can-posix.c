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


static cann_conn_t *conn;

/************************************************************************+**
 * Management 
 */

void can_init(cann_conn_t *aconn){
	conn = aconn;
}

void can_setmode( can_mode_t mode )
{
	rs232can_msg rmsg;

	rmsg.len     = 1; 
	rmsg.cmd     = RS232CAN_SETMODE;
	rmsg.data[0] = (char)mode;

	if (conn)
		cann_transmit(conn, &rmsg);
	else
		canu_transmit(&rmsg);
}

void can_setfilter() {
	// XXX
}

/****************************************************************************
 * Sending
 */

//returns pointer to the next can TX buffer
can_message * can_buffer_get()
{
	return (can_message*)malloc(sizeof(can_message));
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

	free(cmsg);
}

/****************************************************************************
 * reciving
 */

// XXX must free messages
	
//returns next can message, or 0 if no Message was received.
can_message * can_get_nb(){
	rs232can_msg *rmsg;
	can_message  *cmsg;

	if (conn)
		rmsg = cann_get_nb(conn);
	else
		rmsg = canu_get_nb();

	if (!rmsg) return 0;

	cmsg = can_buffer_get();
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

	if (!rmsg) return 0;

	cmsg = can_buffer_get();
	rs232can_rs2can(cmsg, rmsg);

	return cmsg;
}

void can_free(can_message *msg){
	free(msg);
}


