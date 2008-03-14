
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

#include "util.h"



#define KEY_DOOR_DOWNSTAIRS 0x62


ReaderMsg_t msg;




uint8_t doorstate;


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



//overwrite the tokenfield on the card with new data
uint8_t card_write_token(uint8_t * token){
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

menu_entry_t menu[] PROGMEM = {
	{"newcard", KEY_NEW_CARD, handle_new_card},
	{"id  deact", KEY_DEACTIVATE, handle_deactivate_id},
	{"id  react", KEY_REACTIVATE, handle_reactivate_id},
	{"db  dump", KEY_DUMP_DB, handle_dump_db},
};

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
	uint8_t (*handler)(void) = 0;
	
	seg_putstr_P(PSTR("\r" "    admin"));
	while(1){
		key = get_key();
		for(x=0; x < (sizeof(menu)/sizeof(menu_entry_t)); x++ ){
			if(key == menu[x].key){
				seg_putstr_P (menu[x].name);
				handler = (uint8_t(*)(void))PW(menu[x].handler);
				break;
			}
		}
		if(key == KEY_S){
			if(handler)
				handler();
		}
		if(key == KEY_R){
			break;
		}
	}
}



void segputhex(uint8_t c){
	char buf[3];
	itoa(c, buf, 16);
	seg_putstr(buf);
}


