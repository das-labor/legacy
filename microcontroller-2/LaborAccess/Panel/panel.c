
#include "panel.h"
#include "pixel.h"


TimerControlBlock   myTimer;

AVRX_GCC_TASKDEF(panel, 50, 4)
{
    unsigned char x;
    while(1){
		clear_screen(0);
		for(x=0;x!=255;x++){
			setpixel(x,3);
			AvrXDelay(&myTimer, 20);
		}
	}
}
