
#include <setjmp.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

#include <avr/pgmspace.h>
#include <stdlib.h> 

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

ISR(SIG_INTERRUPT0){
	static uint16_t start;
	if(PIND & (1<<PD2)){
		gval[1] = (TCNT1-start);
	}else{
		start = TCNT1;
	}
}

ISR(SIG_INTERRUPT1){
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
    int8_t pathleft[8];
    int8_t pathright[8];
    int8_t crash;
    
void do_path(int16_t pos0, int16_t pos1) {
    static int16_t scount=0, toncount=0;
    static int8_t posl=3, posr=5;
    int i,j;
    
    i = ((pos1+4*1024)/256);
    if(i<0) i=0;
    scount += i;
    toncount += i;
    //scount += ((pos1/1024));
    //scount += 12;
    if(toncount > 128) {
        toncount = 0;
        PORTB ^= (1<<1) | (1<<2);
    }    
    if(scount > 1024) {
        scount = 0;
            
            
            
        // clear field
        for(i=0;i<8;i++) {
            setpix(pathleft[i],i,0);
            setpix(pathright[i],i,0);
        }
        
        // scroll one line
        for(i=1;i<8;i++) {
            pathleft[i-1] = pathleft[i];
            pathright[i-1] = pathright[i];
        }
        
        // draw next line
        if(rand()>0x6000) {
            if(rand()>0x3fff) {
                posl++;
                if(posl>9) posl=9;
            } else {
                posl--;
                if(posl<0) posl=0;
            }
        }
        
        if(rand()>0x6000) {
            if(rand()>0x3fff) {
                posr++;
                if(posr>9) posr=9;
            } else {
                posr--;
                if(posr<0) posr=0;
            }
        }
        if((posr-posl) < 4) {
            if(posr<9) posr++;
            else posl--;
            
        }
        if((posr-posl) < 4) {
            if(posr<9) posr++;
            else posl--;
            
        }
        if((posr-posl) < 4) {
            if(posr<9) posr++;
            else posl--;
            
        }
        pathleft[7] = posl;
        pathright[7] = posr;
        if(pathleft[7]<0) pathleft[7] = 0;
        if(pathleft[7]>9) pathleft[7] = 9;
        if(pathright[7]<0) pathright[7] = 0;
        if(pathright[7]>9) pathright[7] = 9;        
        
        // draw field
        for(i=0;i<8;i++) {
            for(j=0;j<=9;j++) setpix(j,i,0); 
            if(crash) {
                for(j=0;j<=pathleft[i];j++) {
                    setpix(j,i,2);
                }    
                for(j=pathright[i];j<=9;j++) {
                    setpix(j,i,2);
                }    
            } else {
                for(j=0;j<=pathleft[i];j++) {
                    setpix(j,i,1);
                }    
                for(j=pathright[i];j<=9;j++) {
                    setpix(j,i,1);
                }            
            }
            
            
        }
        
        
    }
}

int main (void){
	unsigned char mode = 1;

	int8_t gd[2];
	int16_t speed0 = 0, speed1=0;
	int16_t pos0 = 0, pos1 = 0, posi0, posi1, posrl;
    int8_t i;
    
    DDRB |= (1<<1) | (2<<1);
    PORTB |= (1<<1);
    PORTB &= ~(2<<1);
	
	clear_screen(0);
	borg_hw_init();
    wait(100);
	g_init();
	sei();	
	
	wait(100);
	
	gzero[0] = gval[0];
	gzero[1] = gval[1];
	
	
	draw_level(1);
	
#define BRAKE 128
//#define BRAKE 4096
#define BOWL 512
#define UNSENSITIVITY 4
	uint8_t b = 0;

    srand(1);
    
	while(1){
		pixel mypix, oldpix;
		
		gd[0] = gval[0] - gzero[0];
		
		speed0 += (gd[0]/UNSENSITIVITY);
		
		speed0 -= (speed0/BRAKE);
		
		speed0 -= pos0/BOWL;
		
		pos0 += (speed0/64);
		
				if((pos0 <(-6*1024)) && (speed0<0)){
					pos0 = -6*1024;
					speed0 = -(speed0-speed0/2)+1;	// Bouncen am Rand
				}
				
				if((pos0 >1024*5) && (speed0>0)){
					pos0 = 1024*5;
					speed0 = -(speed0-speed0/2)-1;	
				}
		
		
		gd[1] = gval[1] - gzero[1];		
		speed1 -= (gd[1]/UNSENSITIVITY);
		
		speed1 -= (speed1/BRAKE);
		
		speed1 -= pos1/BOWL;
		
		pos1 += (speed1/64);
		
				if((pos1 < (-5*1024)) && (speed1<0)){
					pos1 = -5*1024;
					speed1 = -(speed1-speed1/2)+1;	
				}
				
				if((pos1 > (4*1024)) && (speed1>0)){
					pos1 = 4*1024;
					speed1 = -(speed1-speed1/2)-1;	
				}
		
		
	//	pos0 = 0; pos1 = 0;
        
		posi0 = pos0/1024; 
        posi1 = pos1/1024;
        
        if(pos0/1024 >=5 ) {
        //    if((pos0/1024) > 6) posi0++;
        } else {
        //    if((pos0/1024) < 3) posi0--;
        }
        
		posi0 += 5; 
        posi1 += 4;
		
		if(posi0 < 0) posi0 = 0; if(posi0 > 9) posi0 = 9;
		if(posi1 < 0) posi1 = 0; if(posi1 > 7) posi1 = 7;	
        
        // crash?
        if(((posi0)==(int16_t)pathleft[1]) || ((posi0)==(int16_t)pathright[1])) {
            // beep
        //    for(i=0;i<10;i++) {
        //        PORTB ^= (1<<1) | (1<<2);
        //        wait(1);
        //    }
            if((posi0)==(int16_t)pathleft[1]) posi0++;
            if((posi0)==(int16_t)pathright[1]) posi0--;
            crash = 1;
        } else {
            crash = 0;
        }
        
        posi1=1;
		mypix = (pixel){posi0, posi1};
        
		
	//	if((mypix.x != oldpix.x) || (mypix.y != oldpix.y) ){
			setpixel(oldpix,0);
		
			if(get_pixel(mypix)){
				setpixel(mypix,3);
				b = 2;
			}else{
				setpixel(mypix,3);
				b = 0;
			}
			
	//	}
				
		oldpix = mypix;
		
        do_path(pos0, pos1);
        
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
