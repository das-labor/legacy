

#include <avr/io.h>

#include <avrx-io.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "fifo.h"
#include "xcan.h"
#include "rf.h"



#define PIN_SWITCH PIND
#define BIT_SWITCH PD0

TimerControlBlock switchtimer;

void labor_on(){
	AvrXPutFifo(rftxfifo, 0x00111500);//Steckdose D (Bar)
	AvrXPutFifo(rftxfifo, 0x00510500);//Steckdose A (Couch)
	AvrXPutFifo(rftxfifo, 0x00511400);//Steckdose C (Bastelecke)
	
	AvrXPutFifo(rftxfifo, 0x010000C0); //Fluter an
	
	
	AvrXPutFifo(rftxfifo, 0x00111500);//Steckdose D (Bar)
	AvrXPutFifo(rftxfifo, 0x00510500);//Steckdose A (Couch)
	AvrXPutFifo(rftxfifo, 0x00511400);//Steckdose C (Bastelecke)
	
	AvrXPutFifo(rftxfifo, 0x010000C0); //Fluter an
	
}

void labor_off(){
	AvrXPutFifo(rftxfifo, 0x01000040); //Fluter aus
	
	AvrXPutFifo(rftxfifo, 0x00541400);//Steckdose C (Bastelecke)
	AvrXPutFifo(rftxfifo, 0x00540500);//Steckdose A (Couch)
	AvrXPutFifo(rftxfifo, 0x00141500);//Steckdose D (Bar)
	
	AvrXPutFifo(rftxfifo, 0x00541400);//Steckdose C (Bastelecke)
	AvrXPutFifo(rftxfifo, 0x00540500);//Steckdose A (Couch)
	AvrXPutFifo(rftxfifo, 0x00141500);//Steckdose D (Bar)

	AvrXPutFifo(rftxfifo, 0x00541400);//Steckdose C (Bastelecke)
	AvrXPutFifo(rftxfifo, 0x00540500);//Steckdose A (Couch)
	AvrXPutFifo(rftxfifo, 0x00141500);//Steckdose D (Bar)
}

AVRX_GCC_TASKDEF(switchtask, 20, 8){
	uint8_t state, oldstate = 0;
	
	while(1){
		state = PIN_SWITCH & (1<<BIT_SWITCH);
		if (state != oldstate){
			oldstate = state;
			if(state){
				labor_off();	
			}else{
				labor_on();
			}
		}
		AvrXDelay(&switchtimer, 200);		
	}
}
