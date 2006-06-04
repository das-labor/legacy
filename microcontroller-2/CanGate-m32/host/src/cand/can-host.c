
#include "config.h"
#include "can.h"
#include "rs232can.h"

typedef struct{
	unsigned char flags;
	can_message msg;
}can_message_x;


/* Management */
void can_setmode( can_mode_t mode ) {

}


void can_setfilter() {

}


/*******************************************************************/


void can_init(){
	rs232can_reset();
}

//returns next can message, or 0 if no Message was received.
can_message * can_get_nb(){
	rs232can_msg * rmsg;
	rmsg = rs232can_get_nb();
	if(rmsg && (rmsg->cmd == RS232CAN_PKT)){
		static can_message cmsg;
		rs232can_rs2can(&cmsg, rmsg);
		return &cmsg;
	}else{
		return 0;
	}
}

can_message * can_get(){
	can_message * msg;
	while(! ((msg = can_get_nb())) );
	return msg;
}

static can_message Cmsg;

//returns pointer to the next can TX buffer
can_message * can_buffer_get(){
	return &Cmsg;
}

//transmit a can message
void can_transmit(can_message* msg){
	rs232can_msg rmsg;
	rs232can_can2rs(&rmsg, msg);
	rs232can_transmit(&rmsg);
}
