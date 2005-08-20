#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "config.h"

#include "uart-host.h"
#include "can.h"
#include "rs232can.h"


void print_packets(){
	while(1) {
		can_message *msg = can_get_nb();
		if(msg)printf("\nmsg from %X:%X\tto %X:%X\t dlc %d\tdata %X\n",msg->addr_src, msg->port_src, msg->addr_dest, msg->port_dest, msg->dlc, msg->data[0]);
		sleep(1);
	}
}


int main(int argc, char *argv[]){
	if(argc < 2){
		printf("usage: command [param1, param2...] ");
		exit(1);
	}
	
	uart_init();
	can_init();

	rs232can_reset();

	if(!strcmp(argv[1], "p")){
		print_packets();
	}else if(!strcmp(argv[1], "g")){
		int sa,da,sp,dp,d[8];
		int num = sscanf(argv[2], "%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x",&sa,&sp,&da,&dp,&d[0],&d[1],&d[2],&d[3],&d[4],&d[5],&d[6],&d[7]) - 4;
		if(num >= 0){
			can_message *msg = can_buffer_get();
			msg->dlc = num;
			msg->addr_src = sa;
			msg->addr_dest = da;
			msg->port_src = sp;
			msg->port_dest = dp;
			unsigned int x;
			for(x=0;x<8;x++){
				msg->data[x] = (unsigned char) d[x];
			}
			can_transmit(msg);
		}else{
			printf("error: not enough bytes provided");
		}
	
	}
	
	
	

	return 0;
}
