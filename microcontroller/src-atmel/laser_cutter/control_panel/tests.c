

void test_keys_and_display(){
	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			//_delay_ms(100);
			uint8_t value = (keys[(x/4)] & (1<<(x%4))) ? 0:1;
			
			setpixel(x, value);
			
			if(value){
				int8_t sbuf[10];
				sprintf(sbuf, "%04d",x);
				seg_print(0, sbuf);
			}
			
		}
	}
}

void test1(){
	while(1){
		uint8_t x;
		for(x=0;x<16;x++){
			_delay_ms(100);
			setpixel(x,1);
		}
		for(x=0;x<16;x++){
			_delay_ms(100);
			setpixel(x,0);
		}
	}

}

