
#include "ks0108.h"
#include "ks0108_config.h"


// Commands
#define LCD_ON			0x3F
#define LCD_OFF			0x3E
#define LCD_SET_ADD		0x40
#define LCD_SET_PAGE		0xB8
#define LCD_DISP_START		0xC0

#define SELECT0()		PORT_CMD |= (1<<CSEL1); PORT_CMD &= ~(1<<CSEL0)
#define SELECT1()		PORT_CMD |= (1<<CSEL0); PORT_CMD &= ~(1<<CSEL1)
#define SELECT_ALL()		PORT_CMD &= ~(1<<CSEL0); PORT_CMD &= ~(1<<CSEL1)


static void dispEnable() {
	PORT_CMD |= (1 << EN);					// EN high level width: min. 450ns
	asm volatile("nop\n\t"
			"nop\n\t"
			"nop\n\t"
			::);

	PORT_CMD &= ~(1 << EN);
	volatile uint8_t i;
	for(i=0; i<8; i++);							// a little delay loop (faster than reading the busy flag)
		asm volatile ("nop");
}


static void dispCommand(uint8_t cmd) {
	PORT_CMD &= ~(1 << D_I);						// Set D_I to Instruction
	PORT_CMD &= ~(1 << R_W);						// Write
	DDR_DATA = 0xFF;								// data port is output
	PORT_DATA = cmd;								// write command
	dispEnable();
}

static void dispWriteData(uint8_t data) {
	PORT_CMD |= (1 << D_I);							// Set D_I to Data
	PORT_CMD &= ~(1 << R_W);						// Write
	DDR_DATA = 0xFF;								// data port is output
	PORT_DATA = data;								// write data
	dispEnable();
}

static uint8_t dispReadData() {
	uint8_t data;
	volatile uint8_t i;
	PORT_DATA = 0x00;
	DDR_DATA = 0x00;								// data port is input
	
	PORT_CMD |= 0x01 << D_I;						// Set D_I to Data
	PORT_CMD |= 0x01 << R_W;						// Read
	
	PORT_CMD |= 0x01 << EN;							// EN high level width: min. 450ns

	asm volatile("nop\n\t"
				 "nop\n\t"
				 "nop\n\t"
				 ::);
	data = PIN_DATA;								// read Data			 
	
	PORT_CMD &= ~(0x01 << EN);
	for(i=0; i<8; i++);								// a little delay loop (faster than reading the busy flag)
	return data;
}

static void dispSetAddress(uint8_t x, uint8_t page) {
	
	//validate coordinates
	x %= 128;
	page %= 8;
		
	if(x >= 64) {									// select the chip to use
		x -= 64;
		SELECT1();
	}else{
		SELECT0();	
	}
	
	dispCommand(LCD_SET_ADD | x);						// set x address on active chip
	
	dispCommand(LCD_SET_PAGE | page);					// set Page
}


void dispSetPix(uint8_t x, uint8_t y, uint8_t color) {
	uint8_t data, msk;
	
	msk = (1<<(y%8));
	
	dispSetAddress(x,y/8);
	dispReadData();
	
	data = dispReadData();
	if(color){
		data |= msk;
	}else{
		data &= ~msk;
	}
	
	dispSetAddress(x,y/8);
	dispWriteData(data);
}

void dispClear(){
	uint8_t x, page;
	for(page=0;page<8;page++){
		dispSetAddress(0, page);
		for(x=0;x<64;x++){
			dispWriteData(0);
		}
		dispSetAddress(64, page);
		for(x=0;x<64;x++){
			dispWriteData(0);
		}		
	}
}

void dispInit() {
	
	//set control pins to output
	DDR_CMD |= (1<<R_W) | (1<<EN) | (1<<CSEL0) | (1<<CSEL1) | (1<<D_I) | (1<<RESET) | (1 << PC7);
	
	//set enable to idle state
	PORT_CMD |= (1<<EN) | (1<<RESET);
	PORT_CMD &=  ~(1 << PC7);
	
	SELECT_ALL();
	dispCommand(LCD_ON);
	dispCommand(LCD_DISP_START);						// display start line = 0
	dispClear();
}


void dispHLine(uint8_t x, uint8_t y, uint8_t x2, uint8_t color) {
}
void dispVLine(uint8_t x, uint8_t y, uint8_t y2) {
} 
void dispFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
}
void dispDrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
}
