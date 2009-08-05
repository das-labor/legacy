
void test_green(){
	for(y=0;y<8;y++){
		for(x=0;x<16;x++){
			setPixel((pixel){x,y}, green);
			swap();
			msleep(50);
		}
		
	}
}
