
#include <setjmp.h>
#include <avr/interrupt.h>
#include <avr/signal.h>

#include <avr/pgmspace.h>

#include "config.h"
#include "programm.h"
#include "borg_hw.h"
#include "pixel.h"

volatile uint16_t gval[2];
uint16_t gzero[2];


void g_init(){
	GICR |= (1<<INT0) | (1<<INT1);
	MCUCR |= (1<<ISC00) | (1<<ISC10); //Interrupt on any edge
	
	TCCR1B |= (1<<CS11);//Timer 1 to clk/8
	
}

SIGNAL(SIG_INTERRUPT0){
	static uint16_t start;
	if(PIND & (1<<PD2)){
		gval[1] = (TCNT1-start);
	}else{
		start = TCNT1;
	}
}

SIGNAL(SIG_INTERRUPT1){
	static uint16_t start;
	if(PIND & (1<<PD3)){
		gval[0] = (TCNT1-start);
	}else{
		start = TCNT1;
	}
}

//uint8_t Level[] PROGMEM = "h3,5,4;";


#define setpix(x,y,b) setpixel((pixel){x,y},b)

void draw_hline(uint8_t xstart, uint8_t xend, uint8_t y){
	uint8_t x;
	for(x = xstart;x<=xend;x++){
		setpix(x,y,1);
	}	
}
	

void draw_level(uint8_t num){
	//draw_hline(3,8,2);
	
	//draw_hline(3,8,5);
	
	
	
}

int main (void){
	unsigned char mode = 1;

	int8_t gd[2];
	int16_t speed0 = 0, speed1=0;
	int16_t pos0 = 0, pos1 = 0;
	
	clear_screen(0);
	borg_hw_init();
	g_init();
	sei();	
	
	wait(100);
	
	gzero[0] = gval[0];
	gzero[1] = gval[1];
	
	
	draw_level(1);
	
#define BRAKE 1024
#define BOWL 2048
	uint8_t b = 0;

	while(1){
		pixel mypix, oldpix;
		
		gd[0] = gval[0] - gzero[0];
		
		speed0 += gd[0];
		
		speed0 -= (speed0/BRAKE);
		
		//speed0 -= pos0/BOWL;
		
		pos0 += (speed0/64);
		
		if((pos0 <0) && (speed0<0)){
			pos0 = 0;
			speed0 = -(speed0-speed0/2)+1;	
		}
		
		if((pos0 >1024*10-1) && (speed0>0)){
			pos0 = 1024*10-1;
			speed0 = -(speed0-speed0/2)-1;	
		}
		
		
		gd[1] = gval[1] - gzero[1];		
		speed1 -= gd[1];
		
		speed1 -= (speed1/BRAKE);
		
	//	speed1 -= pos1/BOWL;
		
		pos1 += (speed1/64);
		
		if((pos1 <0) && (speed1<0)){
			pos1 = 0;
			speed1 = -(speed1-speed1/2)+1;	
		}
		
		if((pos1 >(8*1024-1)) && (speed1>0)){
			pos1 = 8*1024-1;
			speed1 = -(speed1-speed1/2)-1;	
		}
		
		
		
		
		mypix = (pixel){pos0/1024, pos1/1024};
		
		if((mypix.x != oldpix.x) || (mypix.y != oldpix.y) ){
			setpixel(oldpix,b);
		
			if(get_pixel(mypix)){
				setpixel(mypix,3);
				b = 2;
			}else{
				setpixel(mypix,3);
				b = 0;
			}
			
		}
				
		oldpix = mypix;
		
		wait(1);
	}

	
	for(;;){
		switch(mode++) {
		case 0:
			off();
			break;
		case 1:
			schachbrett(18);
			break;
		case 2:
			spirale(45);
			break;
		case 3:
			fadein();
			break;
		case 4:
			joern1();
			break;
		case 5:
			test1();
			break;
		case 6:
			snake();
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			matrix();
			break;
		default:
			mode = 1;
		}
	}
}
