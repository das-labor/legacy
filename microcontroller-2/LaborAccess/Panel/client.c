
#include "myint.h"

#include "channel.h"

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
uint8 get_event(void ** event_data){
	MessageControlBlock *p;
	uint8 event_type;
	
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
		*event_data = &Channels[CLIENT_CHANNEL];
	}
		
}


//call this function with the pointer returned by the get_event function, if it is a COM event.
//this function checks the size of the message, and returns its ID.
uint8_t get_message_id(void * channel){
	channel_t * chan = (channel_t *) channel;
	
	//return 0 if there are no bytes in message
	if(chan->size == 0) return 0;
	
	//return the first byte in message; it is the messages id.
	return(chan->buffer[0]);
}

//wait for a com event ignoring other events, and copy its data to the given buffer.
//if the received packets data is larger than max_size, 1 is returned.
//returns 0 on success.
uint8 com_read(void * data, uint8 max_size){
	

}


//read the data from the card to the request object
uint8_t card_read_credentials(request_auth_t * request){
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

//send a request, that consists of only a type,
//and receive the result, that is also only one byte.
uint8_t simple_request(uint8_t type){
	uint8_t result;
	channel_write(0, (uint8_t *)&type, 1);
	com_read(0, (uint8_t *)&result, 1);
	return result;
}


void handle_card_inserted(){
		request_auth_t request;
		reply_auth_t reply;
		char * errorstring;

		send_reset();
	
		CARD_POWER_ON();

		//i2cEeWrite(0,sizeof(muh), muh);
		
		seg_putstr_P(PSTR("\r" "cardcheck"));
		
		AvrXDelay(&client_timer, 1000);
		
		
		//first we build an auth request with the data on the card
		request.type = REQUEST_AUTH;
		if( card_read_credentials(&request) ){
			errorstring = PSTR("\r" "bad card");
			goto error;
		}
		
		seg_putstr_P(PSTR("\r" "keycheck"));
		AvrXDelay(&client_timer, 1000);
			
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
			if(reply.permissions & PERM_ADMIN){
				admin_menu();
			}else{
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




AVRX_GCC_TASKDEF(client, 150, 4){
	while(1){
		uint8 event;
		void * data;
		
		event = get_event(&data);
		
		
		if(event == EVENT_CARD){
			handle_card_inserted();
		}else if(event == EVENT_KEY){
			//ignore keys
		}else if(event == EVENT_COM){
			//ignore com
		}
		
	}
			
}

void client_init(){
	Channels[CLIENT_CHANNEL].queue = &ClientQueue;
}
