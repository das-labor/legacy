
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



#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"

#include "borg_hw.h"
#include "pixel.h"
#include "7seg.h"

#include "channel.h"
#include "channels.h"

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
u16 simple_request(u08 type){
	u08 result;
	com_write(&type, 1);
	if( 1 == com_read(&result, 1) ){
		return result;
	}else{
		return 0xffff;
	}
}

static void send_reset(){
	u08 type = REQUEST_RESET;
	com_write(&type, 1);
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


//read the Laboranten-data from the card to the request object
u08 card_read_data(request_new_card_t * request){
	if(! i2cEeDetect())
		//no i2c card
		return 1;
	
	//the root object starts at address 4 and is 251 bytes large.
	asn1_obj_t root_obj = {4,251};
	asn1_obj_t obj;
	
	//get a reference to the Laboranten Data object in obj.
	if (asn1_get(&obj, &root_obj, 0x69))
		//no such object
		return 1;
	
	//read nickname
	if ( asn1_read(&obj, 0x82, (u08 *) &request->nickname, NICKNAME_MAX_LEN+1 ) != (NICKNAME_MAX_LEN+1) )
		//no such object
		return 1;

#ifdef USE_REALNAME
	//and realname
	if ( asn1_read(&obj, 0x83, (u08 *) &request->realname, 32 ) != 32)
		//no such object
		return 1;
#endif
	
	//succeeded
	return 0;		
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

u08 card_write_token_and_id(u08 * token, u16 id){
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

	if ( asn1_write(&obj, 0x80, (u08*)&id, 2) != 2 )
		//write failed
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


static reply_auth_t auth_reply;

//read credentials from card and check them with masterunit. reads reply to the global
//variable auth_reply. If the card checks out o.k., the new token from the master is written
//to the card.
//the mode selects wether some messages are surpressed. This is because we check a new
//card first with this function, so an old card isn't accicdently overwritten.
u08 check_card(u08 mode){
	char * errorstring;
	u08 errorcode;
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
		errorcode = 1;
		goto error;
	}
	
	seg_putstr_P(PSTR("\r" "keycheck"));
	AvrXDelay(&client_timer, 1000);
		
	com_write(&request, sizeof(request));
	if( sizeof(auth_reply) != com_read(&auth_reply, sizeof(auth_reply)) ){
		errorstring = PSTR("\r" "com error");
		errorcode = 2;
		goto error;		
	}
	
	if(	(auth_reply.result == RESULT_DEACTIVATED) || (auth_reply.result == RESULT_OK) ){
		if ( card_write_token(auth_reply.token) ){
			errorstring = PSTR("\r" "bad write");
			errorcode = 3;
			goto error;
		}
	}
	
	CARD_POWER_OFF();

	if(auth_reply.result == RESULT_DEACTIVATED){
		errorcode = 4;
		if(mode == 0){
			seg_putstr_P(PSTR("\r" "deactivated"));
			AvrXDelay(&client_timer, 2000);
			seg_putstr_P(PSTR("\r" "by  "));
			seg_putstr(auth_reply.nickname);
			AvrXDelay(&client_timer, 2000);
		}
		goto end;
	}
	
	if (auth_reply.result != RESULT_OK){
		errorcode = 5;
		if(mode == 0){
			errorstring = PSTR("\r" "bad key");
			goto error;
		}else{
			goto end;
		}
	}
	
	if(mode == 0){
		seg_putstr_P(PSTR("\r" "helo"));
		seg_putstr(auth_reply.nickname);
	
		AvrXDelay(&client_timer, 1800);
	}

	return 0;

error:
	seg_putstr_P(errorstring);
	AvrXDelay(&client_timer, 1800);
end:
	CARD_POWER_OFF();
	return errorcode;
}


u08 activate_card(){
	char * errorstring;
	u08 errorcode;
	request_new_card_t request;
	
	CARD_POWER_ON();

	seg_putstr_P(PSTR("\r" "activating"));
	
	AvrXDelay(&client_timer, 1000);
	
	//first we build a new_card request with the data on the card
	request.type = REQUEST_NEW_CARD;
	if( card_read_data(&request) ){
		//card doesn't contain right asn1 objects -> bad card
		errorstring = PSTR("\r" "bad card");
		errorcode = 1;
		goto error;
	}
		
	com_write(&request, sizeof(request));
	if( sizeof(auth_reply) != com_read(&auth_reply, sizeof(auth_reply)) ){
		errorstring = PSTR("\r" "com error");
		errorcode = 2;
		goto error;		
	}
		
	if (auth_reply.result != RESULT_OK){
		errorcode = 5;
		errorstring = PSTR("\r" "act error");
		goto error;
	}
	
	if ( card_write_token_and_id(auth_reply.token, auth_reply.id) ){
		errorstring = PSTR("\r" "bad write");
		errorcode = 3;
		goto error;
	}

	CARD_POWER_OFF();

	seg_putstr_P(PSTR("\r" "helo"));
	seg_putstr(auth_reply.nickname);
	
	AvrXDelay(&client_timer, 1800);

	return 0;

error:
	CARD_POWER_OFF();
	seg_putstr_P(errorstring);
	AvrXDelay(&client_timer, 1800);
	return errorcode;
}


//return codes for menu function handlers:
//0:	Go back to Admin menu
//1:	Exit completeley
//2:	Display "escaped" and exit completeley

//activate a new card
uint8_t handle_new_card(){
	u08 tmp;
	//ask for more credentials
	seg_putstr_P(PSTR("\r" "need2admin"));
	
	//eject the card
	eject_card();
	
	if( wait_for_card() ){
		//key was pressed instead of card inserted -> escape
		return 2;
	}
	
	if( check_card(0) ){
		//no valid card -> error
		return 1;
	}
	
	if( ! (auth_reply.permissions & PERM_ADMIN) ){
		seg_putstr_P(PSTR("\r" "no  admin"));
		AvrXDelay(&client_timer, 1800);
		return 1;
	}
	
	//o.k. - now we should have the authorisation to activate a new card
lb_activate_begin:	
	seg_putstr_P(PSTR("\r" "needn card"));
	
	//eject the card
	eject_card();
	
	if( wait_for_card() ){
		//key was pressed instead of card inserted -> escape
		return 2;
	}
	
	tmp = check_card(1);
	if( (tmp == 0) || (tmp == 4) ){
		//valid card was inserted by mistake
		seg_putstr_P(PSTR("\r" "not new"));
		AvrXDelay(&client_timer, 1800);
		goto lb_activate_begin;
	}
	
	//we want to get the result "bad key"
	if(tmp != 5){
		//error
		return 1;
	}
	
	activate_card();
	
	return 1;
}

void print_num(u16 num){
	char buf[5];
	u08 x, l;
	if(num>9999){
		seg_putstr_P(PSTR("\n" "E   "));
		return;
	}
	seg_putc('\n');
	itoa(num, buf, 10);
	l = strlen(buf);
	for(x=0; x<(4-l); x++){
		seg_putc(' ');
	}
	seg_putstr(buf);
}


u08 key_to_digit(u08 key){
	if(key == 0x50)
		return 0;
	if((key > 0x30) && (key < 0x35))
		return key - 0x30;
	if((key >= 0x40) && (key < 0x45))
		return key - 0x40 + 5;
	return 0xff;
}

//lets user enter a 4 digit decimal number on the keypad.
//the number is printed right aligned on the left display during entry.
//the D key can delete digits, and the R key escapes, returning 0xffff.
u16 numeric_input(u16 first_key){
	u16 key;
	u16 num = 0;
	u08 digit;
	while(1){
		if(first_key){
			key = first_key;
			first_key = 0;
		}else{
			key = get_key();
		}
		if(key == KEY_S){
			return num;
		}else if(key == KEY_D){
			num /= 10;
			print_num(num);
		}else if(key == KEY_R){
			return 0xffff;
		}else{
			digit = key_to_digit(key);
			if( (digit != 0xff) && (num < 1000) ){
				num *= 10;
				num += digit;
				print_num(num);
			}
		}
	}
}

uint8_t handle_deactivate_id(){
	return 0;
}

uint8_t handle_reactivate_id(){
	return 0;
}

u16 get_and_show_laboranten_data(u16 id){
	
}

uint8_t handle_dump_db(){
	u16 id = 1;
	while(1){
		u16 key;
		get_and_show_laboranten_data(id);
		key = get_key();
		
	
	
	
	}
	
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


u08 foobuf[40];

void handle_card_inserted(){
		
		
		
		seg_putstr_P(PSTR("\r" "thank you"));

		AvrXDelay(&client_timer, 2000);

		
		//read from card
		CARD_POWER_ON();
		i2cEeRead(foobuf,4,40);
		CARD_POWER_OFF();
		
		seg_putstr_P(PSTR("\r" "card ok"));

		AvrXDelay(&client_timer, 2000);
		
		seg_putstr_P(PSTR("\r" "pin "));
		
		numeric_input(0);
		
		eject_card();
		
		seg_putstr_P(PSTR("\r" "insert card"));

		return;		
		
		
		char * errorstring;
	
		u08 foobar;
	
		if( RESULT_OK != ( foobar = simple_request(REQUEST_RESET)) ){
			if(foobar == 1){
				//Hackhack: write to card
				
				com_read(foobuf, 40);
				CARD_POWER_ON();
				i2cEeWrite(4,foobuf,40);
				CARD_POWER_OFF();
			}else if(foobar == 2){
				//read from card
				CARD_POWER_ON();
				i2cEeRead(foobuf,4,40);
				CARD_POWER_OFF();
				com_write(foobuf, 40);
			}
			errorstring = PSTR("\r" "com error" );
			goto error;
		}
	
		//get_doorstate();
	
		if( check_card(0) ){
			goto end;
		}
		
		if(key_is_pressed(KEY_ADMIN)){
			if(auth_reply.permissions & PERM_ADMIN){
				if( 2 == admin_menu() ){
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

		send_reset();
		
		//get_doorstate();
	
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
