
#include <avr/io.h>
#include "progmem.h"
#include <avrx.h>
#include "xcan.h"
#include "xlap.h"
#include "fifo.h"
#include "rf.h"

#define LAMP_AND_SWITCH_C
#include "lamp_and_switch.h"

AVRX_DECL_FIFO(lapd_fifo, 5);

static can_message_t msg_onoff_info = {0, 0x00, PORT_LAPD, PORT_LAPD, 3, {FKT_ONOFF_INFO, 0, 0}};

static int8_t lampbright[4];

void lampedim(uint16_t param){
	uint8_t lampe;
	int8_t d;
	int8_t tmp;
	static can_message_t msg = {0, 0x35, PORT_LAMPE, PORT_LAMPE, 3};

	lampe = param & 0xff;
	d = param>>8;	

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

void all_lampedim(uint16_t param){
	uint8_t x;
	for(x=0;x<4;x++){
		lampedim(param | x);
	}
}

uint8_t switch_states[32];


static void switch_send_info(uint8_t num){
	msg_onoff_info.addr_src = myaddr;
	msg_onoff_info.data[1] = num;
	msg_onoff_info.data[2] = switch_states[num];
	can_put(&msg_onoff_info);
}


uint8_t relais[2];

void set_relais(uint8_t num, uint8_t state){
	relais[num] = state;
}

void rc_switch_set(uint8_t num, uint8_t state){
	if(num >= sizeof(switch_states))
		return;
	
	if(switch_states[num] != state){
		switch_states[num] = state;
		if(num < 20){	//rc switch
			if(num < (sizeof(rc_switch_codes)/8)){
				AvrXPutFifo(rftxfifo, PD(rc_switch_codes[num][state]));
			}else{
				return;
			}
		}else{			//relais
			if(num<(NUM_RELAIS+20)){
				set_relais(num-20,state);
			}else{
				return;
			}
		}
		switch_send_info(num);
	}
}

static can_message_t msg_switch_set = {0, 0x00, PORT_LAPD, PORT_LAPD, 3, {FKT_ONOFF_SET, 0, 0}};

void can_switch_set(uint8_t device, uint8_t num, uint8_t state){
	msg_switch_set.addr_src = myaddr;
	msg_switch_set.addr_dst = device;
	msg_switch_set.data[1] = num;
	msg_switch_set.data[2] = state;
	can_put(&msg_switch_set);
}


void rc_switch_set_p(uint16_t param){
	uint8_t num = param & 0xff;
	uint8_t state = 0;
	
	if(param & SWITCH_ON)
		state = 1;
	
	rc_switch_set(num, state);
}


AVRX_GCC_TASKDEF(lapd_task, 70, 7){
	while(1){
		uint8_t d[4];
		*(uint32_t*) d = AvrXWaitPullFifo(lapd_fifo);
		switch(d[0]){	
			case FKT_ONOFF_SET:
				rc_switch_set(d[1], d[2]);
				break;
			case FKT_ONOFF_GET:
				switch_send_info(d[1]);
				break;
		}
		
	}
}

void lamp_and_switch_init(){
	AVRX_INIT_FIFO(lapd_fifo);
}
