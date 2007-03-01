#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart.h"

#include "api.h"
#include "testAnim.h"

#define PIXMAP ((uint8_t *)0xE000)

#define PWMTABLE ((uint16_t *)0xE400)
#define MAGIC (*(uint8_t *)0xE755)

/*
void wait(){
	uint32_t w;
	w = 750000;
	while(w--){
		asm volatile("nop");	
	}
}

void test1(){
	uint16_t x, y;
	for(x=0;x<5;x++){
		for(y=0;y<75;y++){
			PIXMAP[128*x+y] = 255;
			wait();
		
			PIXMAP[128*x+y] = 0;
		}
	}

}

*/

int main (void)
{	


	
	asm volatile("ldi r28, 0xFF\n\tldi r29, 0xBF\n\tout 0x3e, r29\n\tout 0x3d, r28");
	
	uint16_t i;
	uint8_t tmp;
	
	uart_putstr_P(PSTR("Appliaction\n\r"));
	
	//oepn uint8_t muh[256];
	
	for(i=0;i<256;i++){
		tmp = i;
		if (tmp < 14) tmp = 14;		
		PWMTABLE[i] = tmp;
	}
	
	PWMTABLE[256] = 100;
	PWMTABLE[257] = 100;
	
	MAGIC = 0x23;
	
	
	for(i=0;i<1024;i++){
		PIXMAP[i] = 0;
	}

	display_loop((void *) 0);

	/*
	while(1) {
		test1();
	}
	uint8_t x;
	while(1){
		
		PIXMAP[0] = x;	
		PIXMAP[1] = x;	
		PIXMAP[2] = x;	
		
		PIXMAP[128+0] = 0;	
		PIXMAP[128+1] = -x;	
		PIXMAP[128+2] = x;	
		
		PIXMAP[256+0] = x;	
		PIXMAP[256+1] = 0;	
		PIXMAP[256+2] = -x;	
		
		PIXMAP[384+0] = -x;	
		PIXMAP[384+1] = x;	
		PIXMAP[384+2] = 0;	
		
		PIXMAP[512+0] = x;	
		PIXMAP[512+1] = x;	
		PIXMAP[512+2] = x;	
		
		
		
		//PIXMAP[16+6] = -x;
		muh[x] = x;
	
		x++;
		wait();
		
	}	

	
	DDRA = 0xff;
	PORTA = 0x55;
	
	while(1);
	
*/	
}
