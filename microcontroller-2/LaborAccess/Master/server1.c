
#include "uart_sw.h"
#include "avrx.h"

#include "protocol.h"

AVRX_GCC_TASKDEF(server1, 50, 4)
{
	uint8_t error;

	while(1){
		request_t req;
		error = channel_read(CHANNEL_SERVER1, (uint8_t *) req, sizeof(request_t));
		if(error) goto handle_error;
		
		if(req.type < 0x80){//requests below 0x80 don't need credentials
			switch(req.type){
				case REQ_OPEN:
					break;
			}
		}else{
		
			switch (req.type){
				case REQ_OPEN_CRED:{
					if ()
					break;
				}
			}
		}
		
		
		}
		
		
		handle_error:
		
		
	}
}

