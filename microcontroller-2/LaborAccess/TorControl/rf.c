
#include <avr/io.h>
#include <string.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "fifo.h"
#include "xcan.h"
#include "xlap.h"

#define RF_C
#include "rf.h"

#define PORT_RF_TX PORTD
#define DDR_RF_TX DDRD
#define BIT_RF_TX PD0

#define TX_ON() PORT_RF_TX |= (1<<BIT_RF_TX)
#define TX_OFF() PORT_RF_TX &= ~(1<<BIT_RF_TX)

AVRX_DECL_FIFO(rftxfifo, 10);

TimerControlBlock timer;

void code_tx_24(rf_code_t code){
	uint8_t byte, tmp, msk;
		for(byte=0;byte<3;byte++){
			tmp = code.b[byte];
			msk = 0x80;
			while(msk){
				if( tmp & msk){
					TX_ON();
					AvrXDelay(&timer, 2);
					TX_OFF();
					AvrXDelay(&timer, 1);
				}else{
					TX_ON();
					AvrXDelay(&timer, 1);
					TX_OFF();
					AvrXDelay(&timer, 2);
				}
				msk>>=1;
			}
		}
		TX_ON();
		AvrXDelay(&timer, 1);
		TX_OFF();
		AvrXDelay(&timer, 20);
}

void code_tx_12(rf_code_t code){
	uint8_t byte=0, tmp, msk, bitcnt = 0;
		
	TX_ON();
	AvrXDelay(&timer, 1);
	while (bitcnt<12){
			tmp = code.b[byte++];
			msk = 0x80;
			while(msk && (bitcnt<12)){
				if( tmp & msk){
					TX_OFF();
					AvrXDelay(&timer, 1);
					TX_ON();
					AvrXDelay(&timer, 2);
				}else{
					TX_OFF();
					AvrXDelay(&timer, 2);
					TX_ON();
					AvrXDelay(&timer, 1);
				}
				msk>>=1;
				bitcnt++;
			}
		}
		TX_OFF();
		AvrXDelay(&timer, 20);
}

void code_tx(rf_code_t code){
	if(code.b[3] == 1){
		code_tx_12(code);	
	}else{
		code_tx_24(code);
	}
}

AVRX_GCC_TASKDEF(rftxtask, 100, 2){
	while(1){
		uint32_t code;
		uint8_t x;
		code = AvrXWaitPullFifo(rftxfifo);
				
		for(x=0;x<50;x++){
			code_tx(*(rf_code_t*)&code);
		}
	}
}

void rf_init(){
	AVRX_INIT_FIFO(rftxfifo);
	DDR_RF_TX |= (1<<BIT_RF_TX);
}
