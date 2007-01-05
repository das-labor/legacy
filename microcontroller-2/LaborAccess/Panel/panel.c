
#include "panel.h"
#include "pixel.h"

#include "7seg.h"
#include "foo.h"
#include "borg_hw.h"
TimerControlBlock   myTimer;

AVRX_GCC_TASKDEF(panel, 50, 4)
{
    unsigned char x, y;
    while(1){
		for(x=0;x<16;x++){
			for(y=0;y<8;y++){
				setpixel((x^1)*16+8+y,3);
				AvrXDelay(&myTimer, 20);
				seg_putstr("\n");			
				hexdump(keys+5,5);
			
			}
		}
		
		for(x=0;x<16;x++){
			for(y=0;y<8;y++){
				setpixel((x^1)*16+8+y,1);
				AvrXDelay(&myTimer, 20);
				seg_putstr("\n");			
				hexdump(keys+5,5);
			
			}
		}
	}
}
