//Display driver for Intelligent Displays connected to latches

#include <avr/io.h>

#define DISP_DDR DDRA
#define DISP_PORT PORTA


unsigned char ADDRESS=0; 


static void setletter(unsigned char disp, unsigned char addr, unsigned char data){
	DISP_PORT = data>>2;
	DISP_PORT |= 0x20;
	DISP_PORT &= ~0x20;
	
	DISP_PORT =  (data&0x03) | ( (0x03-addr) << 2);
	DISP_PORT |= 0x40;
	DISP_PORT &= ~0x40;
	
	DISP_PORT = 0x0F ^ (0x01<<disp);
	DISP_PORT |= 0x80;
	DISP_PORT &= ~0x80;
	
	DISP_PORT = 0x0F;
	DISP_PORT |= 0x80;
	DISP_PORT &= ~0x80;
}


void display_clear(){
	unsigned char x,y;
	for(x=0;x<4;x++){
		for(y=0;y<4;y++){
			setletter(x,y,' ');
		}
	}
}

void display_char(char c){
	ADDRESS &= 0x0F;
	setletter(ADDRESS>>2, ADDRESS & 0x03, c);
	if ( ++ADDRESS == 0x10) ADDRESS = 0;
}

void display_print(char * text){
	char c;
	for (;(c = *text++);display_char(c));
}

void display_set_cursor(unsigned char line, unsigned char pos){
	ADDRESS = 8*line + pos;
}

void display_clear_line(unsigned char line){
	ADDRESS = 8*line;
	unsigned char x;
	for(x=0;x<8;x++){
		display_char(' ');
	}
	ADDRESS = 8*line;
}

void display_init(){
	DISP_DDR = 0xFF;
	
	DISP_PORT =  0x0F;
	DISP_PORT |= 0x80;
	DISP_PORT &= ~0x80;

	display_clear();
}

