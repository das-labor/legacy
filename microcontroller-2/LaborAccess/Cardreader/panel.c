
#include "panel.h"


TimerControlBlock   myTimer;

AVRX_GCC_TASKDEF(panel, 50, 4)
{
    while(1){
			AvrXDelay(&myTimer, 20);
	}
}
