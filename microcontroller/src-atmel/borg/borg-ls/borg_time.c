//by hansi

//to use the time functions, you need to define LAP_TIME_EXT in config.h

#include "config.h"
#include "can.h"
#include "lap.h"
#include "util.h"
#include "scrolltext.h"
#include <stdio.h>
#include <stdlib.h>

//address of the time master
#define TIME_MASTER_ADDR 0x00

//update timeout in ms
#define TIME_UPDATE_TIMEOUT 100

//hackhack
extern can_addr myaddr;

//send a time request packet via can
void time_request(void)
{
	pdo_message *msg;
	
	//source address
	msg->addr_src = myaddr;
	msg->port_src = PORT_MGT;
	
	//destination address
	msg->addr_dst = TIME_MASTER_ADDR;
	msg->port_dst = PORT_MGT;
	
	//time request command
	msg->cmd = FKT_MGT_TIMEREQUEST;
	
	//set length and transmit
	msg->dlc = 1;
	can_transmit((can_message *)msg);
}

//update time via can, possibly blocking
uint8_t time_update(void)
{
	uint8_t timeout = TIME_UPDATE_TIMEOUT;
	
	//set "time-has-been-updated" to false
	lap_time_update = 0;
	
	//send request
	time_request();
	
	//wait some time for a reply in 1ms steps
	while((lap_time_update == 0) && (timeout-- > 0))
	{
		wait(1);
	}
	
	return lap_time_update;
}

//display the time
void time_anim(void)
{
	char timestring[34];
	
	//update time and return if we had no success
	if(time_update() == 0)
	{	
		return;
	}
	
	//convert the time to a string	
	sprintf(timestring, ">+:p20d50/#%02hi:#<;p20d50/#%02hi", lap_time_h, lap_time_m);
	
	//show the time
	scrolltext(timestring);
}