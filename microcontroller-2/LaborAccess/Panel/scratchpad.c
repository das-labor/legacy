
//returns 1 if value is in zero terminated list, else 0
uint8 is_in_list(uint8 value, uint8 * list){
	uint8 c;
	while((c = *list++)){
		if(c==value)
			return 1;
	}
	return 0;
}
