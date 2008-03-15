
#include <avr/pgmspace.h>
#include "client.h"

#include "borg_hw.h"
#include "pixel.h"
#include "7seg.h"

#include "channel.h"

#include "reader.h"
#include "i2csw.h"
#include "asn1.h"

#include "protocol.h"

#include "macro.h"

//******* User Interface Button and LED mappings *************/
#define LED_FLAG 0xa8

#define KEY_ADMIN KEY_PLAY

#define KEY_NEW_CARD   KEY_1
#define KEY_DEACTIVATE KEY_2
#define KEY_REACTIVATE KEY_3
#define KEY_DUMP_DB KEY_9


//*******            Global Variables            *************/

//Queue in which we receive incoming messages from Cardreader, Keyboard and COM.
MessageQueue ClientQueue;

//Timer for various stuff
TimerControlBlock client_timer;




#define EVENT_CARD 1
#define EVENT_KEY  2
#define EVENT_COM  3

//wait for an event. returns the kind of event encountered, and a pointer to the events data.
//Reader and Keyboard Messages are automatically acked, but Communication Messages must
//be acked by the caller when the messages data isn't needed anymore, as acking the
//message is the signal, that the buffer may be overwritten.
u08 get_event(uint16_t * event_data){
	MessageControlBlock *p;
	u08 event_type = 0;
	
	p = AvrXWaitMessage(&ClientQueue);
	
	*event_data = 0;
	
	if(p == (MessageControlBlock*)&ReaderMsgOut){
		//AvrXAckMessage(p);
		event_type = EVENT_CARD;	
	}else if(p == (MessageControlBlock*)&KeyboardMsg){
		event_type = EVENT_KEY;
		*event_data = KeyboardMsg.key;
		AvrXAckMessage(p);
	}else if(p == &Channels[CLIENT_CHANNEL].mcb ){
		event_type = EVENT_COM;
	}
	
	return event_type;
}


//blocks waiting for a key
//returns 0xffff if other event encountered
uint16_t get_key(){
	uint16_t key;
	u08 event;
	while(1){
		event = get_event(&key);
		if(event == EVENT_KEY){
			return key;
		}else if(event == EVENT_COM){
			AvrXAckMessage (&Channels[CLIENT_CHANNEL].mcb);
		}
		return 0xffff;
	}
}

u08 card_inside;

//wait for events. returns 0 on card inserted or 1 on other events.
u08 wait_for_card(){
	uint16_t data;
	u08 event = get_event(&data);

	if(event == EVENT_CARD){
		card_inside = 1;
		return 0;
	}else if(event == EVENT_KEY){
		return 1;
	}else if(event == EVENT_COM){
		AvrXAckMessage (&Channels[CLIENT_CHANNEL].mcb);			
		return 1;
	}
	return 1;
}

//eject card if we know one is inside
void eject_card(){
	if (card_inside){
		//eject the card
		ReaderMsg_t msg = {{0,0}, COMMAND_EJECT};
		CARD_POWER_OFF();
		AvrXSendMessage(&ReaderMsgInQueue, (MessageControlBlock*)&msg);
		//and remember it is out
		card_inside = 0;
	}
}


//wait for a com event ignoring other events, and copy its data to the given buffer.
//if the received packets data is larger than max_size, 0 is returned.
//returns actual size on success.
u08 com_read(void * data, u08 max_size){
	u08 event;
	uint16_t event_data;
	//wait for a com event
	do{
		event = get_event(&event_data);
	}while(event != EVENT_COM);
	
	return channel_read(CLIENT_CHANNEL, data, max_size);
}

void com_write(void * data, u08 size){
	channel_write(CLIENT_CHANNEL, data, size);
}

//send a request, that consists of only a type,
//and receive the result, that is also only one byte.
u08 simple_request(u08 type){
	u08 result;
	com_write(&type, 1);
	if( 1 == com_read(&result, 1) ){
		return result;
	}else{
		return 0;
	}
}


u08 doorstate;
//get locked/unlocked state of the doors from the master.
//this is kinda ugly. should better use another channel for status updates.
void get_doorstate(){
	doorstate = simple_request(REQUEST_GET_DOORSTATE);
	if(doorstate & STATE_DOOR_UPSTAIRS){
		setpixel(LED_FLAG,1);
	}else{
		setpixel(LED_FLAG,0);
	}
}


//read the data from the card to the request object
u08 card_read_credentials(request_auth_t * request){
	if(! i2cEeDetect())
		//no i2c card
		return 1;
	
	//the root object starts at address 4 and is 251 bytes large.
	asn1_obj_t root_obj = {4,251};
	asn1_obj_t obj;
	
	//get a reference to the Lab Acces Data object in obj.
	if (asn1_get(&obj, &root_obj, 0x69))
		//no such object
		return 1;
	
	//read card id
	if ( asn1_read(&obj, 0x80, (u08 *) &request->card_id, 2) != 2)
		//no such object
		return 1;
			
	//and token
	if ( asn1_read(&obj, 0x81, request->token, 8) != 8 )
		//no such object
		return 1;

	//succeeded
	return 0;		
}

u08 card_write_token(u08 * token){
	//the root object starts at address 4 and is 251 bytes large.
	asn1_obj_t root_obj = {4,251};
	asn1_obj_t obj;
	
	//get a reference to the Lab Acces Data object in obj.
	if (asn1_get(&obj, &root_obj, 0x69))
		//no such object
		return 1;
	
	if ( asn1_write(&obj, 0x81, token, 8) != 8 )
		//write failed
		return 1;

	//succeeded
	return 0;		
}


reply_auth_t auth_reply;

u08 check_card(){
	char * errorstring;
	request_auth_t request;
	
	CARD_POWER_ON();

	//i2cEeWrite(0,sizeof(muh), muh);
	
	seg_putstr_P(PSTR("\r" "cardcheck"));
	
	AvrXDelay(&client_timer, 1000);
	
	//first we build an auth request with the data on the card
	request.type = REQUEST_AUTH;
	if( card_read_credentials(&request) ){
		//card doesn't contain right asn1 objects -> bad card
		errorstring = PSTR("\r" "bad card");
		goto error;
	}
	
	seg_putstr_P(PSTR("\r" "keycheck"));
	AvrXDelay(&client_timer, 1000);
		
	com_write(&request, sizeof(request));
	if( sizeof(auth_reply) != com_read(&auth_reply, sizeof(auth_reply)) ){
		errorstring = PSTR("\r" "com error");
		goto error;		
	}
	
	if(	(auth_reply.result == RESULT_DEACTIVATED) || (auth_reply.result == RESULT_OK) ){
		if ( card_write_token(auth_reply.token) ){
			errorstring = PSTR("\r" "bad write");
			goto error;
		}
	}

	if(auth_reply.result == RESULT_DEACTIVATED){
		seg_putstr_P(PSTR("\r" "deactivated"));
		AvrXDelay(&client_timer, 2000);
		seg_putstr_P(PSTR("\r" "by  "));
		seg_putstr(auth_reply.nickname);
		AvrXDelay(&client_timer, 2000);
		goto end;
	}
	
	if (auth_reply.result != RESULT_OK){
		errorstring = PSTR("\r" "bad key");
		goto error;
	}
	
	seg_putstr_P(PSTR("\r" "helo"));
	seg_putstr(auth_reply.nickname);
	
	AvrXDelay(&client_timer, 1800);

	return 0;

error:
	seg_putstr_P(errorstring);
	AvrXDelay(&client_timer, 1000);
end:	
	return 1;
}


uint8_t handle_new_card(){
	seg_putstr_P(PSTR("\r" "need2admin"));
	
	//eject the card
	eject_card();
	
	if( wait_for_card() ){
		return 1;	
	}

	check_card();
	
	return 0;
}

uint8_t handle_deactivate_id(){
	return 0;
}

uint8_t handle_reactivate_id(){
	return 0;
}

uint8_t handle_dump_db(){
	return 0;
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

#define NUM_MENU_ITEMS (sizeof(menu)/sizeof(menu_entry_t))

u08 admin_menu(){
	uint8_t x;
	uint16_t key;
	uint8_t (*handler)(void) = 0;
	u08 return_code;
	
	seg_putstr_P(PSTR("\r" "    admin"));
	while(1){
		key = get_key();
		if(key == 0xffff){
			return 1;
		}
		for(x=0; x < NUM_MENU_ITEMS; x++ ){
			if(key == PB(menu[x].key) ){
				seg_putc('\r');
				seg_putstr_P (menu[x].name);
				handler = (uint8_t(*)(void))PW(menu[x].handler);
				break;
			}
		}
		if(key == KEY_S){
			if(handler){
				return_code = handler();
				if ( return_code ){
					return return_code;
				}
			}
		}
		if(key == KEY_R){
			return 0;
		}
	}
}


//uint8_t muh[]={0xde,0xad,0xbe,0xef, 0x69, 0x0e ,0x80, 0x02, 0x01, 0x00, 0x81, 0x08, 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};


void handle_card_inserted(){
		
		char * errorstring;
	
		if( RESULT_OK != simple_request(REQUEST_RESET)){
			errorstring = PSTR("\r" "com error" );
			goto error;
		}
	
		if( check_card() ){
			goto end;
		}
		
		if(key_is_pressed(KEY_ADMIN)){
			if(auth_reply.permissions & PERM_ADMIN){
				if( 1 == admin_menu() ){
					errorstring = PSTR("\r" "    escaped");
					goto error;				
				}
			}else{
				errorstring = PSTR("\r" "no  admin");
				goto error;
			}
		}else{
			if(doorstate & STATE_DOOR_UPSTAIRS){
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
		AvrXDelay(&client_timer, 1000);
		
		
		end:

		eject_card();
		
		AvrXDelay(&client_timer, 3000);
		
		seg_putstr_P(PSTR("\r"));
		
}


AVRX_GCC_TASKDEF(client, 150, 4){
	while(1){
		
		if( 0 == wait_for_card() ){
			handle_card_inserted();
		}
	
	}
}

void client_init(){
	Channels[CLIENT_CHANNEL].queue = &ClientQueue;
}
