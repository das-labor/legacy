#include "lpc2103.h"
#include "fastfunc.h"
#include "config.h"
#include "types.h"

extern volatile uint8_t mode;
FASTRUN void fastfunc_test(void)
{
	CLK_LOW;
	uint16_t testvar=1;
loop:
	switch(mode){
			case 1:
				FIO0SETL = 0x07E0;
				CLK_PULSE;
				FIO0CLRL = 0x07E0;
				CLK_PULSE;
				FIO0SETL = 0x07E0;
				CLK_PULSE;
				FIO0CLRL = 0x07E0;
				CLK_PULSE;
			break;
			case 2:	
				//calc next position in RAM
				FIO0SETL = (testvar<<OUTPORTCONST)&0x07E0;
				FIO0CLRL = (~(testvar<<OUTPORTCONST))&0x07E0;
				//same as OUTPORT = (testvar<<OUTPORTCONST)&0x07E0;	//move from RAM to PORT
				__asm("nop");
				CLK_PULSE;
		
				testvar++;
				if(testvar > 0x3f)
				testvar=0;
				break;
			case 3:	
				//calc next position in RAM
				FIO0SETL = ((testvar+2)<<OUTPORTCONST)&0x07E0;
				FIO0CLRL = (~((testvar+2)<<OUTPORTCONST))&0x07E0;
				//same as OUTPORT = (testvar<<OUTPORTCONST)&0x07E0;	//move from RAM to PORT
				__asm("nop");
				CLK_PULSE;
				FIO0SETL = ((testvar+1)<<OUTPORTCONST)&0x07E0;
				FIO0CLRL = (~((testvar+1)<<OUTPORTCONST))&0x07E0;
				//same as OUTPORT = (testvar<<OUTPORTCONST)&0x07E0;	//move from RAM to PORT
				__asm("nop");
				CLK_PULSE;
		
				testvar+=2;
				if(testvar > 0x3d)
				testvar=0;
			break;
	}
	goto loop;
}

