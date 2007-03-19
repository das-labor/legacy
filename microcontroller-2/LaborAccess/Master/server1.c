
#include <string.h>
#include <avrx.h>


#include "uart_sw.h"

#include "protocol.h"
#include "channels.h"
#include "channel.h"

AVRX_GCC_TASKDEF(server1, 100, 4)
{
	while(1){
		request_t req;
		reply_t reply;
		uint8_t size;
		
		size = channel_read(CHANNEL_SERVER1, (uint8_t *) &req, sizeof(request_t));
		if(size==0) goto handle_error;
		
		if(req.type < 0x80){//requests below 0x80 don't need credentials
			switch(req.type){
				default:
					break;
			}
		}else{
		
			switch (req.type){
				case REQUEST_OPEN_CRED:{
					strcpy(reply.nickname, "tixiv" );
					channel_write(CHANNEL_SERVER1, (uint8_t *) &reply, sizeof(reply_t));
				}
				break;
			}
		}
		
		continue;
		
		
		handle_error:
		while(0);
		
	}
}

