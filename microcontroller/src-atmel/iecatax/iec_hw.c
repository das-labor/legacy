
#include <avrx.h>


AVRX_GCC_TASKDEF(iectask, 200, 3){

}



mutex atnMutex;

AVRX_SIGINT(SIG_INTERRUPT0){
	
	AvrXSetSemaphore(&atnMutex);
}

AVRX_GCC_TASKDEF(atntask, 20, 3){
	AvrXWaitSemaphore(&atnMutex);
	
}
