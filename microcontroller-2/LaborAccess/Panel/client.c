
/*
	This is the Client, that sends the requests to the Labor Access Master.
	Some of the requests need credentials in Form of card id and key, and some don't.
	
	Requests that don't need credentials:
		-Unlock downstairs door (when upstairs is unlocked)
		-Lock downstairs door (when upstairs is unlocked)
		
	Requests that need credentials:
		-Unlock upstairs door
		-Lock upstairs door
		
	Requests that need credentials from an admin:
		-Activate new card
		-Deactivate card by number
		-Reactivate card by number
		-Show data by number
		-Dump database to CAN
	
	Requests that need credentials from 2 admins:
		-Set admin flag for card
		-Delete database entry
		
	
	When a card is inserted, the Panel assumes a door open or lock request, depending on the
	state the door is in at that time.
	When one holds the tape-button while inserting a card, the admin mode is entered. One
	can now do the admin tasks.

*/


#include <stdlib.h>
#include <avr/pgmspace.h>
#include "avrx.h"

#include "enum.h"

#include "borg_hw.h"
#include "reader.h"
#include "i2csw.h"
#include "7seg.h"
#include "pixel.h"

#include "AvrXSerialIo.h"

#include "asn1.h"
#include "channel.h"

#include "protocol.h"

#define KEY_NEW_CARD   KEY_1
#define KEY_DEACTIVATE KEY_2
#define KEY_REACTIVATE KEY_3
#define KEY_DUMP_DB KEY_9




#define KEY_DOOR_DOWNSTAIRS 0x62
#define LED_FLAG 0xa8


ReaderMsg_t msg;

/*
uint8_t buf[256];

void putbuf(uint8_t * b, uint8_t s){
	while(s--)
		put_char0(*b++);

}
*/

//uint8_t muh[]={0xde,0xad,0xbe,0xef, 0x69, 0x0e ,0x80, 0x02, 0x01, 0x00, 0x81, 0x08, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};


MessageQueue ClientQueue;

uint8_t doorstate;

TimerControlBlock client_timer;

//get locked/unlocked state of the doors from the master.
void get_doorstate(){
	request_t req;
	req.type = REQUEST_GET_DOORSTATE;
	channel_write(0, (uint8_t *)&req, 1);
	channel_read(0, (uint8_t *)&doorstate, 1);
		
	if(doorstate & STATE_DOOR_DOWNSTAIRS){
		setpixel(LED_FLAG,1);
	}else{
		setpixel(LED_FLAG,0);
	}
}

//send a request, that consists of only a type,
//and receive the result, that is also only one byte.
uint8_t simple_request(uint8_t type){
	request_t req;
	uint8_t result;
	req.type = type;
	channel_write(0, (uint8_t *)&req, 1);
	channel_read(0, (uint8_t *)&result, 1);
	return result;
}

uint8_t card_read_credentials(request_cred_t * request){
	if(! i2cEeDetect())
		return 1;
	
	asn1_obj_t root_obj = {4,251};
	asn1_obj_t obj;
		
	if (asn1_get(&obj, &root_obj, 0x69))
		return 1;
			
	if ( asn1_read(&obj, 0x80, (uint8_t *) &request->card_id, 2) != 2)
		return 1;
			
	if ( asn1_read(&obj, 0x81, request->token, 8) != 8 )
		return 1;

	return 0;		
}

uint8_t card_write_token(uint8_t[8] token){
	if(! i2cEeDetect())
		return 1;
	
	asn1_obj_t root_obj = {4,251};
	asn1_obj_t obj;
		
	if (asn1_get(&obj, &root_obj, 0x69))
		return 1;
					
	if ( asn1_write(&obj, 0x81, token, 8) != 8 )
		return 1;

	return 0;		
}

uint8_t handle_new_card(){
	
}

uint8_t handle_deactivate_id(){
	
}

uint8_t handle_reactivate_id(){
	
}

uint8_t handle_dump_db(){
	
}

typedef struct{
	char name[12];
	uint8_t key;
	uint8_t (*handler)(void);
}menu_entry_t;

char newcard_str[] PROGMEM = "newcard";
char id_deact_str[] PROGMEM = "id  deact";
char id_react_str[] PROGMEM = "id  react";
char db_dump_str[] PROGMEM = "db  dump";

menu_entry_t menu[] PROGMEM = {
	{newcard_str, KEY_NEW_CARD, handle_new_card},
	{id_deact_str, KEY_DEACTIVATE, handle_deactivate_id},
	{id_react_str, KEY_REACTIVATE, handle_reactivate_id},
	{db_dump_str, KEY_DUMP_DB, handle_dump_db},
}

//blocks waiting for a key
uint8_t get_key(){
	uint8_t key;	
	MessageControlBlock *p;		
	while(1){
		p = AvrXWaitMessage(&ClientQueue);
		if(p == (MessageControlBlock*)&KeyboardMsg){
			key = KeyboardMsg.key;
			AvrXAckMessage(p);
			return key;
		}
	}				
}

uint8_t admin_menu(){
	uint8_t x;
	uint8_t key;
	uint8_t (*handler)(void);
	
	seg_putstr_P(PSTR("\r" "    admin"));
	while(1){
		key = get_key();
		for(x=0; x < (sizeof(menu)/sizeof(menu_entry_t)); x++ ){
			if(key == menu[x].key){
				seg_putstr_P (PW(menu[x].name);
				handler = PW(menu[x].handler);
				break;
			}
		}
		if(key == KEY_S){
			handler();	
		}
		if(key = KEY_R){
			break;	
		}
	}
}

void handle_card_inserted(){
		request_cred_t request;
		reply_auth_t reply;
		char * errorstring;

		CARD_POWER_ON();

		//i2cEeWrite(0,sizeof(muh), muh);
		
		seg_putstr_P(PSTR("\r" "cardcheck"));
		
		AvrXDelay(&client_timer, 400);
		
		if( card_read_credentials(&request) ){
			errorstring = PSTR("\r" "bad card");
			goto error;
		}
		
		seg_putstr_P(PSTR("\r" "keycheck"));
		AvrXDelay(&client_timer, 400);
		
		request.type = REQUEST_AUTH;
		
		channel_write(0, (uint8_t *)&request, sizeof(request));
		channel_read(0, (uint8_t *)&reply, sizeof(reply));
		
		if(reply.result == RESULT_DEACTIVATED){
			seg_putstr_P(PSTR("\r" "deactivated"));
			AvrXDelay(&client_timer, 2000);
			seg_putstr_P(PSTR("\r" "by  "));
			seg_putstr(reply.nickname);
			AvrXDelay(&client_timer, 2000);
			goto end;
		}
		
		if (reply.result != RESULT_OK){
			errorstring = PSTR("\r" "bad key");
			goto error;
		}
		
		seg_putstr_P(PSTR("\r" "helo"));
		seg_putstr(reply.nickname);
		
		AvrXDelay(&client_timer, 1000);
					
		if(key_is_pressed(KEY_ADMIN)){
			if(reply.flags & FLAG_ADMIN){
				admin_menu();
			else{
				errorstring = PSTR("\r" "no  admin");
				goto error;
			}
		}else{
			if(doorstate & STATE_DOOR_DOWNSTAIRS){
				//door is open, so we request locking it
				if(simple_request(REQUEST_LOCK_UPSTAIRS) == RESULT_OK){
					seg_putstr_P(PSTR("\r" "doorlocking"));
				}else{
					errorstring = PSTR("\r" "cantlock");
					goto error;
				}
			}else{
				//door is locked, so we request opening it
				if(simple_request(REQUEST_UNLOCK_UPSTAIRS) == RESULT_OK){
					seg_putstr_P(PSTR("\r" "dooropening"));
				}else{
					errorstring = PSTR("\r" "cantopen");
					goto error;
				}
			}
		}
		
		goto end;
		
		error:
		seg_putstr_P(errorstring);
		AvrXDelay(&client_timer, 400);
		
		
		end:
		CARD_POWER_OFF();

		msg.state = COMMAND_EJECT;
		
		AvrXSendMessage(&ReaderMsgInQueue, (MessageControlBlock*)&msg);
		AvrXWaitMessageAck((MessageControlBlock*)&msg);

}


void segputhex(uint8_t c){
	char buf[3];
	itoa(c, buf, 16);
	seg_putstr(buf);
}


AVRX_GCC_TASKDEF(client, 150, 4){
	while(1){
		MessageControlBlock *p;
		
		get_doorstate();
		
		p = AvrXWaitMessage(&ClientQueue);
		
		if(p == (MessageControlBlock*)&ReaderMsgOut){
			AvrXAckMessage(p);
			handle_card_inserted();
		}else if(p == (MessageControlBlock*)&KeyboardMsg){
			uint8_t key;
			key = KeyboardMsg.key;
			AvrXAckMessage(p);
			if(key == KEY_DOOR_DOWNSTAIRS){
				if(doorstate & STATE_DOOR_DOWNSTAIRS){
					simple_request(REQUEST_LOCK_DOWNSTAIRS);
				}else{
					simple_request(REQUEST_UNLOCK_DOWNSTAIRS);
				}
			}			
		}
		
		
	}
			
}
