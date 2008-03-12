
//returns 1 if value is in zero terminated list, else 0
uint8 is_in_list(uint8 value, uint8 * list){
	uint8 c;
	while((c = *list++)){
		if(c==value)
			return 1;
	}
	return 0;
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
