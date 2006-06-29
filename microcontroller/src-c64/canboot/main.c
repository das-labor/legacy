#include "can.h"
#include "spi.h"

#include <stdio.h>
#include <stdlib.h>


void hexdump(unsigned char * addr, unsigned char size){
	unsigned char x=0, sbuf[3];
	
	while(size--){
		itoa(*addr++, sbuf, 16);
		if (sbuf[1] == 0) printf(" ");
		printf("%s ",sbuf);
		if(++x == 16){
			printf("\n");
			x = 0;
		}
	}
}


int main(){
	can_message * cm;
	
	asm ("sei");
	
	
	printf("SPI Init...\n");
	spi_init();
	
	printf("CAN Init...\n");
	can_init();

	printf("Main Loop");
	while(1){
		cm = can_get_nb();
		if(cm){
			printf("Received Message:\n");
			hexdump((void*)cm, (cm->dlc)+5);
			printf("\n");
		}
	
	}

}
