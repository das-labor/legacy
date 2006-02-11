
#include <avr/io.h>
#include <string.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "fifo.h"
#include "xcan.h"
#include "xlap.h"

#define RF_C
#include "rf.h"

#define PORT_RF_TX PORTB
#define DDR_RF_TX DDRB
#define BIT_RF_TX PB0

#define TX_ON() PORT_RF_TX |= (1<<BIT_RF_TX)
#define TX_OFF() PORT_RF_TX &= ~(1<<BIT_RF_TX)


typedef struct{
	uint8_t p;
	uint8_t buf[100];
}rfrxbuf_t;

uint8_t receiving;

rfrxbuf_t rfrxbuf;

//This is set while we are receiving
static Mutex rfin_mutex, rxtx_mutex;


AVRX_DECL_FIFO(rftxfifo, 10);

AVRX_SIGINT(SIG_INTERRUPT1)
{
	IntProlog();             // Switch to kernel stack/context
	if(receiving){
		uint8_t t;
		rfrxbuf.buf[rfrxbuf.p] = TCNT1L;
		TCNT1 = 0;
		
		t = rfrxbuf.p +1;
		if(t >= 100)
			t = 0;
		rfrxbuf.p = t;
	}else{
		AvrXResetSemaphore(&rxtx_mutex);
		receiving = 1;
		rfrxbuf.p = 0;
		TCNT1 = 0;
		TIMSK |= (1<<OCIE1A); //start timeout interrupt
		TIMSK |= (1<<OCIE1B); //start timeout interrupt
	}
	Epilog();
}

//timeout for one code received
AVRX_SIGINT(SIG_OUTPUT_COMPARE1A)
{
	IntProlog();
	TIMSK &= ~(1<<OCIE1A);
	GICR &= ~(1<<INT1);					//turn off Interrupts
	AvrXIntSetSemaphore(&rfin_mutex); //Signal remote signal was received
	Epilog();
}

//timeout for reenabling transmit
AVRX_SIGINT(SIG_OUTPUT_COMPARE1B)
{
	IntProlog();
	TIMSK &= ~(1<<OCIE1B);				//turn off Interrupt
	AvrXIntSetSemaphore(&rxtx_mutex); //Signal we can now transmit again
	Epilog();
}

uint8_t rf_time2code(rf_code_t * code, uint8_t * time, uint8_t numbits){
	uint8_t byte, bit=0, t0, t1, tmp, msk;	
	while(bit < numbits){
		tmp = 0;
		msk = 0x80;
		
		while(msk && (bit < numbits)){
			t0 = *time, t1=*(time+1);
			if((t0>1) && (t0<0x0a) && (t1>0x09) && (t1<0x12)){
				//tmp|=msk;
			}else if((t1>2) && (t1<9) && (t0>0x09) && (t0<0x12)){
				tmp|=msk;
			}else{
				return -1;
			}
			time+=2;
			msk>>=1;
			bit++;
		}
		code->b[byte++] = tmp;
	}
	return 0;
}

int8_t lampbright[4];

void lampedim(uint8_t lampe, int8_t d){
	int8_t tmp;
	static can_message_t msg = {0, 0x35, PORT_LAMPE, PORT_LAMPE, 3};
	tmp = lampbright[lampe] -d;
	if(tmp>0x40) tmp = 0x40;
	if(tmp<0) tmp = 0;
	lampbright[lampe] = tmp;
	
	msg.addr_src = myaddr;
	msg.data[0] = 0;
	msg.data[1] = lampe;
	msg.data[2] = tmp;
	can_put(&msg);
}





AVRX_GCC_TASKDEF(rfrxtask, 50, 6)
{
	OCR1A = 120;
	OCR1B = 2000;
	TCCR1A = 0;
	TCCR1B = 5; // clk/1024
	MCUCR |= (1<<ISC10); //trigger on any edge
	
	static can_message_t msg={0,0xff,PORT_REMOTE,PORT_REMOTE};
	msg.addr_src = myaddr;
	
	uint32_t code=0, lastcode=0;
	uint8_t gotcode;
	
    while (1)
    {
		receiving = 0;
		uint8_t x, new;
		for(x=0;x<100;x++)
			rfrxbuf.buf[x]=0;
		GICR |= (1<<INT1);
		AvrXWaitSemaphore(&rfin_mutex);
		
		gotcode=0;
		if(rfrxbuf.p == 49){
			if(!rf_time2code((rf_code_t*)&code, rfrxbuf.buf, 24))
				gotcode = 1;
			((rf_code_t*)&code)->b[3] = 0;
		}else if(rfrxbuf.p == 25){
			if(!rf_time2code((rf_code_t*)&code, &rfrxbuf.buf[1], 12))
				gotcode = 1;
			((rf_code_t*)&code)->b[3] = 1;
		}
		
		if(gotcode){
			msg.dlc = 4;
			memcpy(msg.data, &code, 4);
			can_put(&msg);
			
			if(code==lastcode) new=0; else new=1;
			
			switch (code){
				case 0x0015050d: //A 1 on
				case 0x00510550: //switch a on
					if(new) AvrXPutFifo(rftxfifo, 0x010000C0); //Fluter an
					break;
				case 0x0014050d: //A 1 off
				case 0x00540550: //switch a off
					if(new) AvrXPutFifo(rftxfifo, 0x01000040); //Fluter aus
					break;
				case 0x0015450d: //A 2 on
				case 0x00511150: //switch b on
					if(new) AvrXPutFifo(rftxfifo, 0x00154515); //Türfluter an
					break;
				case 0x0014450d: //A 2 off
				case 0x00541150: //switch b off
					if(new) AvrXPutFifo(rftxfifo, 0x00144515); //Türfluter aus
					break;
				case 0x0015054d: //B 1 on
				case 0x00511450: //switch c on
					lampedim(0,2);
					lampedim(1,2);
					lampedim(2,2);
					lampedim(3,2);
					break;
				case 0x0014054d: //B 1 off
				case 0x00541450: //switch c off
					lampedim(0,-2);
					lampedim(1,-2);
					lampedim(2,-2);
					lampedim(3,-2);
					break;
				case 0x0015454d: //B 2 on
				case 0x00111550: //switch d on
					if(new) AvrXPutFifo(rftxfifo, 0x00150515); //Theke an
					break;
				case 0x0014454d: //B 2 off
				case 0x00141550: //switch d off
					if(new) AvrXPutFifo(rftxfifo, 0x00140515); //Theke aus
					break;
				case 0x00411550: //switch e on
				case 0x0015051d: //C 1 on
					if(new) AvrXPutFifo(rftxfifo, 0x00150555); //Bastelecken Licht an
					break;
				case 0x00441550: //switch e off
				case 0x0014051d: //C 1 off
					if(new) AvrXPutFifo(rftxfifo, 0x00140555); //Bastelecken Licht aus
					break;
			}
			lastcode = code;
		}
		/*
		if (rfrxbuf.p >24){
			msg.dlc = 8;
			memcpy(msg.data, rfrxbuf.buf, 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[8], 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[16], 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[24], 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[32], 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[40], 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[48], 8);
			can_put(&msg);
			memcpy(msg.data, &rfrxbuf.buf[56], 8);
			can_put(&msg);
			msg.dlc = 0;
			can_put(&msg);
		}
		*/
    }
}


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
		uint8_t x, tmp;
		code = AvrXWaitPullFifo(rftxfifo);
		
		AvrXWaitSemaphore(&rxtx_mutex);
		
		tmp = GICR & (1<<INT1);
		GICR &= ~(1<<INT1);					//turn off receive Interrupt
		
		for(x=0;x<14;x++){
			code_tx(*(rf_code_t*)&code);
		}
		
		AvrXSetSemaphore (&rxtx_mutex);
		GICR |= tmp;					//restore state
		
	}
}

void rf_init(){
	AVRX_INIT_FIFO(rftxfifo);
	AvrXSetSemaphore(&rxtx_mutex);
	DDR_RF_TX |= (1<<BIT_RF_TX);
}
