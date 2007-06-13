
#include <string.h>
#include <avrx.h>


#include "uart_sw.h"

#include "protocol.h"
#include "channels.h"
#include "channel.h"
#include "random.h"

#include <stdio.h>

uint8_t doorstate;

AVRX_GCC_TASKDEF(server1, 200, 4)
{
	uint8_t auth_mode = 0;
	printf("Debug\r");
	
	while(1){
		request_t req;
		uint8_t size;
		
		size = channel_read(CHANNEL_SERVER1, (uint8_t *) &req, sizeof(request_t));
		
		if(size==0) goto handle_error;
		
		if(req.type < 0x80){//requests below 0x80 don't need credentials
			switch(req.type){
				case REQUEST_GET_DOORSTATE:
					channel_write(CHANNEL_SERVER1, (uint8_t *) &doorstate, 1);
					break;
				case REQUEST_LOCK_DOWNSTAIRS:
				{
					uint8_t result = RESULT_OK;
					doorstate &= ~(STATE_DOOR_DOWNSTAIRS);
					channel_write(CHANNEL_SERVER1, (uint8_t *) &result, 1);
					break;
				}
				case REQUEST_UNLOCK_DOWNSTAIRS:
				{
					uint8_t result = RESULT_OK;
					doorstate |= (STATE_DOOR_DOWNSTAIRS);
					channel_write(CHANNEL_SERVER1, (uint8_t *) &result, 1);
					break;
				}
				default:
					break;
			}
		}else{
		
			switch (req.type){
				case REQUEST_AUTH:{
					reply_auth_t reply;
					request_cred_t * req_cred = (request_cred_t*) &req;
					
					if(database_check(req_cred->card_id, req_cred->token) == 0){
						reply.result = RESULT_OK;
						database_get_nickname(req_cred->card_id, reply.nickname);
						generate_token(reply.token);
						database_store_token(req_cred->card_id, reply.token);
					}else{
						reply.result = RESULT_DENIED;	
					}
					
					reply.privileges = 0;
					
					channel_write(CHANNEL_SERVER1, (uint8_t *) &reply, sizeof(reply_auth_t));
					printf("reply\r");
		
				}
				break;
			}
		}
		
		continue;
		
		
		handle_error:
		while(0);
		
	}
}
