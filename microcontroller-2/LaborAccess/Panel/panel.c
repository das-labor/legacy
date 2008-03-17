
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
				AvrXDelay(&myTimer, 40);
				//seg_putstr("\r    tixiv");			
				seg_putc('\r');
				seg_puthex(keys[0]);
				seg_puthex(keys[1]);
				seg_puthex(keys[2]);
				seg_puthex(keys[3]);
				seg_puthex(keys[4]);
				
			
			}
		}
		
		for(x=0;x<16;x++){
			for(y=0;y<8;y++){
				setpixel((x^1)*16+8+y,0);
				AvrXDelay(&myTimer, 40);
				//seg_putstr("\rlmnopqr");			
				seg_putc('\r');
				seg_puthex(keys[0]);
				seg_puthex(keys[1]);
				seg_puthex(keys[2]);
				seg_puthex(keys[3]);
				seg_puthex(keys[4]);
			
			}
		}
	}
}
