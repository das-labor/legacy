
#include "ks0108.h"
#include "ks0108_config.h"


// Commands
#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_SET_ADD			0x40
#define LCD_SET_PAGE		0xB8
#define LCD_DISP_START		0xC0

#define SELECT0()		PORT_CMD |= (1<<CSEL1); PORT_CMD &= ~(1<<CSEL0)
#define SELECT1()		PORT_CMD |= (1<<CSEL0); PORT_CMD &= ~(1<<CSEL1)
#define SELECT_ALL()	PORT_CMD &= ~(1<<CSEL0); PORT_CMD &= ~(1<<CSEL1)


static void enable() {
	volatile 	uint8_t i;
	PORT_CMD |= (1 << EN);					// EN high level width: min. 450ns
/*	asm volatile("nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 ::);
*/
	PORT_CMD &= ~(1 << EN);
	nsleep(10);
	
	//for(i=0; i<8; i++);							// a little delay loop (faster than reading the busy flag)
}


static void command(uint8_t cmd) {
	PORT_CMD &= ~(1 << D_I);						// Set D_I to Instruction
	PORT_CMD &= ~(1 << R_W);						// Write
	DDR_DATA = 0xFF;								// data port is output
	PORT_DATA = cmd;								// write command
	enable();
}

static void writeData(uint8_t data) {
	PORT_CMD |= (1 << D_I);							// Set D_I to Data
	PORT_CMD &= ~(1 << R_W);						// Write
	DDR_DATA = 0xFF;								// data port is output
	PORT_DATA = data;								// write data
	enable();
}

static uint8_t doReadData(){
	uint8_t data;
	volatile uint8_t i;
	
	PORT_DATA = 0x00;
	DDR_DATA = 0x00;								// data port is input
	
	PORT_CMD |= 0x01 << D_I;						// Set D_I to Data
	PORT_CMD |= 0x01 << R_W;						// Read
	
	PORT_CMD |= 0x01 << EN;							// EN high level width: min. 450ns

	nsleep(100);
	data = PIN_DATA;								// read Data			 
	
	PORT_CMD &= ~(0x01 << EN);
	//for(i=0; i<8; i++);								// a little delay loop (faster than reading the busy flag)
	return data;
}

static uint8_t readData(){
	//dummy read
	doReadData();
	return doReadData();
}

//select column and controller accordingly
static void setColumn(uint8_t x) {
	
	//validate coordinates
	x %= 128;
		
	if(x >= 64) {									// select the chip to use
		x -= 64;
		SELECT1();
	}else{
		SELECT0();	
	}
	
	command(LCD_SET_ADD | x);						// set x address on active chip
}

static void setPage(uint8_t page) {
	page %= 8;
	command(LCD_SET_PAGE | page);					// set Page on chip selected at the moment
}


void dispClear(){
	uint8_t x, page;
	for(page=0;page<8;page++){
		setColumn(0);
		setPage(page);
		for(x=0;x<64;x++){
			writeData(0);
		}
		setColumn(64);
		setPage(page);
		for(x=0;x<64;x++){
			writeData(0);
		}		
	}
}

void dispInit() {
	
	//set control pins to output
	//DDR_CMD |= (1<<R_W) | (1<<EN) | (1<<CSEL0) | (1<<CSEL1) | (1<<D_I) | (1<<RESET);
	
	//set enable to idle state
	PORT_CMD |= (1<<EN) | (1<<RESET);
	
	SELECT_ALL();
	command(LCD_ON);
	command(LCD_DISP_START);						// display start line = 0
	dispClear();
}



void dispSetPix(uint8_t x, uint8_t y, uint8_t color) {
	uint8_t data, msk;
	
	msk = (1<<(y%8));
	
	setColumn(x);
	setPage(y/8);
	
	data = readData();
	if(color){
		data |= msk;
	}else{
		data &= ~msk;
	}
	
	setColumn(x);
	writeData(data);
}

//draw vertical line, x & x2 inclusive
void dispVLine(uint8_t x, uint8_t y, uint8_t y2) 
{
	uint8_t sp, ep, mask, data;
	sp=y/8;
	ep=y2/8;

	mask = 0xff << (y%8);
	
	while(sp <= ep){
		if(sp == ep){
			mask &= 0xff >> (7-(y2%8));		
		}
		
		setColumn(x);
		setPage(sp);
		data = readData();
		
		//write pix
		setColumn(x);
		writeData(mask | data);
		mask = 0xff;
		sp++;
	}
}

//draw horizontal line, x & x2 inclusive
void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color)
{
	uint8_t data;
	uint8_t mask = 1 << (y%8);
	
	SELECT_ALL();
	setPage(y/8);
	
	for(;x<=x2;x++)
	{
		setColumn(x);
		data = readData();
		
		if(color)
			data |= mask;	
		else
			data &= ~mask;
		
		//write pix
		setColumn(x);
		writeData( data );
	}
}

void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color){
	uint8_t sp, ep, mask, data, xi, y2;
	y2 = y + height -1;
	sp=y/8;
	ep=y2/8;

	mask = 0xff << (y%8);
	
	//for each page the rectangle is in
	while(sp <= ep){
		if(sp == ep){
			//calculate mask for last page
			mask &= 0xff >> (7-(y2%8));		
		}
		
		//set page on both controllers
		SELECT_ALL();
		setPage(sp);
		
		//for each column
		for(xi = x; xi < x+width; xi++){
			setColumn(xi);
			data = readData();
		
			if(color)
				data |= mask;	
			else
				data &= ~mask;
			
			//write pix
			setColumn(xi);
			writeData(data);
		}
		mask = 0xff;
		sp++;
	}
}

void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
	width--; height--;
	dispHLine(x, y, x + width, color);		// top
	dispHLine(x, y+height, x + width, color);	// bottom
	dispVLine(x, y , y + height);			// left
	dispVLine(x+width, y, y + height);		// right
}

uint8_t xpos, ypos;

extern uint8_t chartable[];

void dispSetPos(uint8_t x, uint8_t y){
	xpos = x;
	ypos = y;
}

void dispPrint(uint8_t * str){
	uint8_t c, x;
	uint8_t * chr;
	while(c = *str++){
		if(c == '\n'){
			ypos += 8;
			xpos = 0;
			continue;			
		}
		if((c >= 0x60) && (c < 0x80 )){
			c -= 0x60;
		}
		setColumn(xpos);
		setPage(ypos/8);
		chr = &chartable[((uint16_t)c << 3)];
		for(x=0;x<8;x++){
			writeData(*chr++);
		}
		xpos += 8;
	}
}
