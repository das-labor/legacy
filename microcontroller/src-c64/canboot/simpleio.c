#include "simpleio.h"

//pointer to beginning of line in screen buffer
#define PNT (*(unsigned int *)0xd1)
//current column
#define PNTR (*(unsigned char *)0xd3)
//current line number
#define TBLX (*(unsigned char *)0xd6)


void print(char * txt){
	char c;
	while(c = *txt++){
		if(c == '\n'){
			PNTR = 0;
			TBLX++;
			PNT+=40;
		}else{
			c &= 0x3f;
			*((char *)PNT+PNTR) = c;
			PNTR++;
			if(PNTR > 39){
				PNTR = 0;
				TBLX++;
				PNT+=40;
			}
		}
	}
}
