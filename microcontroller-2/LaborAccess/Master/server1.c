
#include <string.h>
#include <avrx.h>


#include "uart_sw.h"

#include "protocol.h"
#include "channels.h"
#include "channel.h"
#include "random.h"
#include "database.h"

#include <stdio.h>

uint8_t doorstate;


void send_simple_reply(uint8_t result){
	channel_write(CHANNEL_SERVER1, (uint8_t *) &result, 1);
}


AVRX_GCC_TASKDEF(server1, 200, 4)
{
	uint8_t admin_num = 0;
	uint8_t session_perm = 0;
	printf("Debug\n");
	
	while(1){
		request_t req;
		uint8_t size;
		doorstate |= STATE_DOOR_UPSTAIRS;
		
		size = channel_read(CHANNEL_SERVER1, (uint8_t *) &req, sizeof(request_t));
		
		if(size==0) goto handle_error;
		
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
				request_auth_t * req_cred = (request_auth_t*) &req;
				uint8_t result;
				uint16_t id = req_cred->card_id;
				
				reply.permissions = 0;
				
				result = database_check(id, req_cred->token);
				reply.result = result;
				
				if(result == RESULT_OK){
					generate_token(reply.token);
					database_store_token(id, reply.token);
					
					session_perm = database_get_permissions(id);
					reply.permissions = session_perm;
					
					if(session_perm & PERM_ADMIN){
						admin_num++;	
					}
					
					database_get_nickname(id, reply.nickname);
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
				printf("reply\n");
				break;
			}	
			case REQUEST_RESET:
				admin_num = 0;
				session_perm = 0;
				send_simple_reply(RESULT_OK);
				break;
			case REQUEST_UNLOCK_UPSTAIRS:
				if(session_perm & PERM_DOOR_UPSTAIRS){
					doorstate |= STATE_DOOR_UPSTAIRS;
					send_simple_reply(RESULT_OK);
				}else{
					send_simple_reply(RESULT_DENIED);
				}
				break;
			case REQUEST_LOCK_UPSTAIRS:
				if(session_perm & PERM_DOOR_UPSTAIRS){
					doorstate &= ~STATE_DOOR_UPSTAIRS;
					send_simple_reply(RESULT_OK);
				}else{
					send_simple_reply(RESULT_DENIED);
				}	
				break;
			case REQUEST_NEW_CARD:
				if(session_perm & PERM_ADMIN){
					uint16_t id;
					reply_new_card_t reply;
					request_new_card_t * req_nc = (request_new_card_t*) &req;
					id = database_new_entry(req_nc->permissions, req_nc->nickname, req_nc->realname);
					if(id == 0xffff){
						send_simple_reply(RESULT_DENIED);
					}else{
						reply.result = RESULT_OK;
						reply.id = id;
						generate_token(reply.token);
						database_store_token(id, reply.token);
						channel_write(CHANNEL_SERVER1, (uint8_t *) &reply, sizeof(reply));
					}
				}else{
					send_simple_reply(RESULT_DENIED);
				}
				break;
			case REQUEST_MAKE_ADMIN:
				if(admin_num >= 2){
					reply_make_admin_t reply;
					request_make_admin_t * req_ma = (request_make_admin_t*) &req;
					uint8_t result;
					uint16_t id = req_ma->card_id;
					
					result = database_check(id, req_ma->token);
					reply.result = result;
					
					if(result == RESULT_OK){
						uint8_t permissions;
						generate_token(reply.token);
						database_store_token(id, reply.token);
						
						permissions = database_get_permissions(id);
						permissions |= PERM_ADMIN;
						database_set_permissions(id, permissions);
						
					}else if(result == RESULT_DEACTIVATED){
						generate_token(reply.token);
						database_store_token(id, reply.token);
						
					}else{
						//result == RESULT_DENIED
					}
					
					//return the reply to the client.
					channel_write(CHANNEL_SERVER1, (uint8_t *) &reply, sizeof(reply));
				}else{
					send_simple_reply(RESULT_DENIED);
				}
				break;
			case REQUEST_DELETE_ADMIN:
				if(session_perm & PERM_ADMIN){
					
					
				}else{
					send_simple_reply(RESULT_DENIED);
				}
				break;
			default:
				send_simple_reply(RESULT_DENIED);
				break;
					
		}
		continue;
		
		
		handle_error:
		while(0);
		
	}
}
