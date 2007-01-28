#include "blinkenlights.h"
#include "borg_hw.h"
#include "pixel.h"
#include "util.h"

void showframe_blm(unsigned char *blm){
	int x,y;
	for(y=0;y < NUM_ROWS; y++){
		for(x=0;x < NUM_COLS; x++, blm++){
			if(*blm=='1')
				setpixel((pixel){x,y},3);
			else
				setpixel((pixel){x,y},0);
		}
		blm++;
	}
}

void test_blinkenlights(){
	//@200
	inline unsigned char test_blm1[] = 
	"000000000000000000\
	000011100011100000\
	000111110111110000\
	000111111111110000\
	000011111111100000\
	000000111110000000\
	000000001000000000\
	000000000000000000";

	//@800
	unsigned char test_blm2[] = 
	"000011100011100000\
	000111110111110000\
	001111111111111000\
	001111111111111000\
	000111111111110000\
	000011111111100000\
	000000111110000000\
	000000001000000000";

	for(;;){
		showframe_blm(test_blm1);
		wait(200);
		showframe_blm(test_blm2);
		wait(800);
	}
}

void parse_blm(const char *file[]){
	enum state_t state = nowhere;
	
	// FILE *input = mmc_open(file, "r");
	char c;
	// char duration[10] = "";
	// char frame[144] = "";
	//
	// while(!mmc_eof(input)){
	//	c=mmc_getc(input);

	
	switch (state) {
		case nowhere:
			if (c == '#')
				state = comment;
			else if (c == '@') {
				state = duration;
			}
			else if (c == '\n') {
				state = nowhere;
			}
			else
				state = inframe;
				// add the char to the frame array
				// ***** TODO
		break;
			
		// we are in a comment line...
		case comment:
			if (c == '\n')
				state = nowhere;
			else
				// walk through the comment...
		break;
			
		case duration:
			if (c == '\n')
				state = nowhere;
			else
				// add the character to the duration number
				
				// ***** TODO
		break;
			
		case inframe:
			if (c == '\n')
				state = nowhere;
			else
				// add the char to the frame array
				// ***** TODO
		break;
	}	
}
