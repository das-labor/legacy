
#include <string.h>
#include <avrx.h>


#include "uart_sw.h"

#include "protocol.h"
#include "channels.h"
#include "channel.h"
#include "random.h"

#include <stdio.h>

uint8_t doorstate;


void send_simple_reply(uint8_t result){
	channel_write(CHANNEL_SERVER1, (uint8_t *) &result, 1);
}


AVRX_GCC_TASKDEF(server1, 200, 4)
{
	uint8_t authenticated_session = 0;
	printf("Debug\r");
	
	while(1){
		request_t req;
		uint8_t size;
		
		size = channel_read(CHANNEL_SERVER1, (uint8_t *) &req, sizeof(request_t));
		
		if(size==0) goto handle_error;
		
		if(req.type < 0x80){//requests below 0x80 don't need credentials
			switch(req.type){
				case REQUEST_GET_DOORSTATE:
					send_simple_reply(doorstate);
					break;
				case REQUEST_LOCK_DOWNSTAIRS:
				{
					doorstate &= ~(STATE_DOOR_DOWNSTAIRS);
					send_simple_reply(RESULT_OK);
					break;
				}
				case REQUEST_UNLOCK_DOWNSTAIRS:
				{
					doorstate |= (STATE_DOOR_DOWNSTAIRS);
					send_simple_reply(RESULT_OK);
					break;
				}
				case REQUEST_AUTH:{
					reply_auth_t reply;
					request_cred_t * req_cred = (request_cred_t*) &req;
					uint8_t result;
					uint16_t id = req_cred->card_id;
					
					authenticated_session = 0;
					reply.flags = 0;
					
					result = database_check(id, req_cred->token);
					reply.result = result;
					
					if(result == RESULT_OK){
						generate_token(reply.token);
						database_store_token(id, reply.token);
						
						reply.flags = database_get_flags(id);
						database_get_nickname(id, reply.nickname);
						
						authenticated_session = 1;
					}else if(result == RESULT_DEACTIVATED){
						uint16_t did;
						generate_token(reply.token);
						database_store_token(id, reply.token);
						
						//if the card was deactivated, return the nickname of the
						//one deactivating it.
						did = database_get_deactivation_id(id);
						database_get_nickname(did, reply.nickname);
					}else{
						//result == RESULT_DENIED
					}
					
					//return the reply to the client.
					channel_write(CHANNEL_SERVER1, (uint8_t *) &reply, sizeof(reply_auth_t));
					printf("reply\r");
					break;
				}	
				case REQUEST_RESET:
					authenticated_session = 0;
					send_simple_reply(RESULT_OK);
					break;
				default:
					send_simple_reply(RESULT_DENIED);
					break;
			}
			
		}else{
			if(authenticated_session != 1){
				send_simple_reply(RESULT_DENIED);
			}else{
				switch (req.type){
					case REQUEST_UNLOCK_UPSTAIRS:
						doorstate |= STATE_DOOR_UPSTAIRS;
						send_simple_reply(RESULT_OK);
						break;
					case REQUEST_LOCK_UPSTAIRS:
						doorstate &= ~STATE_DOOR_UPSTAIRS;
						send_simple_reply(RESULT_OK);
						break;
					
					default:
						send_simple_reply(RESULT_DENIED);
						break;
					
				}
			}
		}
		
		continue;
		
		
		handle_error:
		while(0);
		
	}
}
