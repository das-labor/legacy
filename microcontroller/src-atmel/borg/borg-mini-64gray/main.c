
#include <setjmp.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

#include <avr/pgmspace.h>

#include "config.h"
//#include "programm.h"
#include "borg_hw2.h"
#include "util.h"


#ifdef AVR
	#include <avr/io.h>
#endif

/*
  Fuse Low Byte:
  BODLEVEL		: 1 [ ]	; 1: Brown-Out ab 2.7V [2.5V - 3.2V], 0: Brown-Out ab 4.0V [3.7V - 4.2V]
  BODEN			: 0 [X] ; 0:BOD Spannungsreferenz aktiv (höhere Stromaufnahme)
  SUT1			: 1 [ ]	; 11: 16K CK + 65ms (slowly rising power)
  SUT0			: 0 [X] ; 01: BOD benutzen für Reset-Timing
  CKSEL3		: 0 [X]	; 1111: Crystal >3 Mhz
  CKSEL2		: 1 [ ] ; 1101: Crystal <3 Mhz
  CKSEL1		: 0 [X] ; 0100: Intern 8 MHz, CKOPT = 1 [ ]
  CKSEL0		: 0 [X] ; 0001: Intern 1 MHz, CKOPT = 1 [ ]
				= 0xa4

  Fuse High Byte:  
  RSTDISBL		: 1 [ ]	; 1:PC6 ist RESET-pin
  WDTON			: 1 [ ]	; 0:WDT always on (wollen wir nicht)
  SPIEN			: 0 [X]	; 0:SPI immer aktiviert lassen
  CKOPT			: 1 [ ]	; 1:Schwache Clock bis 8 Mhz, 0:Starke Clock ab 1 Mhz (höhere Stromaufnahme)
  EESAVE		: 0 [X] ; 1: EEPROM not preserved through Chip Erase
  BOOTSZ1		: 0 [X] ; 00: 1024 words, $1C00-$1FFF
  BOOTSZ0		: 1 [ ] ; 01:  512 words, $1E00-$1FFF
  BOOTRST		: 1 [ ] ; 0:Reset Vector zeigt auf Bootloaderbeginn
				= 0xd3
				
*/

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

typedef struct {
	unsigned char x;
	unsigned char y;
} pixel;

//uint8_t Level[] PROGMEM = "h3,5,4;";
void setpixel(pixel p, unsigned char value ){
	p.x %= NUM_COLS;
	p.y %= NUM_ROWS;
	pixmap[p.y][p.x]=value;
}

unsigned char get_pixel(pixel p){
	p.x %= NUM_COLS;
	p.y %= NUM_ROWS;
	return(pixmap[p.y][p.x]);
}
#define setpix(x,y,b) setpixel((pixel){x,y},b)

void draw_hline(uint8_t xstart, uint8_t xend, uint8_t y){
	uint8_t x;
	for(x = xstart;x<=xend;x++){
		setpix(x,y,1);
	}	
}
	

void draw_level(uint8_t num){
	draw_hline(3,8,2);
	
	draw_hline(3,8,5);
}

int main (void){
	unsigned char mode = 1;

	int8_t gd[2];
	int16_t speed0 = 0, speed1=0, gmedx = 0, gmedy = 0;
	int16_t pos0 = 0, pos1 = 0, posi0, posi1;
	
	uint16_t bobscreen[10][8];
	
	DDRB = 0x03;
	PORTB = 0x01;
	//while(1) PORTB ^= 0x03;
	
//	clear_screen(0);
	borg_hw_init();
	g_init();
	sei();	
	
	
	//setpix(0,0,3);
	
	
	//while(1);
	
	wait(100);
	
	gzero[0] = gval[0];
	gzero[1] = gval[1];
	
	
	int ix, iy;	
	unsigned int bb = 0;
	for(iy = 0;iy < NUM_ROWS; iy++) {
		for(ix = 0;ix < NUM_COLS; ix++) {
			pixmap[iy][ix] = 0; //bb++ % 64;
		}
	}
//	pixmap[0][0] = 1;
//	while(1);
	
	//draw_level(1);
	//while(1) wait(1);
	
#define BRAKE 1024
#define BOWL 2048
	uint8_t b = 0;


	while(1){
		pixel mypix, oldpix;
		
		// Neue X-Position berechnen
			gd[0] = gval[0] - gzero[0];
			
			gmedx = (gmedx * 7 + gd[0])/8;
			
				speed0 += gd[0];	// Über Integration aus Beschleunigung die Geschwindigkeit gewinnen
				
				speed0 -= (speed0/BRAKE);
				
				//speed0 -= pos0/BOWL;
				
				pos0 += (speed0/32);	// Position über Integration der Geschwindigkeit
				
				if((pos0 <(-6*1024)) && (speed0<0)){
					pos0 = -6*1024;
					speed0 = -(speed0-speed0/2)+1;	// Bouncen am Rand
				}
				
				if((pos0 >1024*5) && (speed0>0)){
					pos0 = 1024*5;
					speed0 = -(speed0-speed0/2)-1;	
				}
			
			pos0 = gmedx*128;
		
		// Neue Y-Position berechnen
			gd[1] = gval[1] - gzero[1];	
			
			gmedy = (gmedy * 7 + gd[1])/8;
			
				speed1 -= gd[1];
				
				speed1 -= (speed1/BRAKE);
				
				//	speed1 -= pos1/BOWL;
				
				pos1 += (speed1/32);
				
				if((pos1 < (-5*1024)) && (speed1<0)){
					pos1 = -5*1024;
					speed1 = -(speed1-speed1/2)+1;	
				}
				
				if((pos1 > (4*1024)) && (speed1>0)){
					pos1 = 4*1024;
					speed1 = -(speed1-speed1/2)-1;	
				}
		
			pos1 = gmedy*-128;
		
		posi0 = pos0/1024; posi1 = pos1/1024;
		posi0 += 5; posi1 += 4;
		
		if(posi0 < 0) posi0 = 0; if(posi0 > 9) posi0 = 9;
		if(posi1 < 0) posi1 = 0; if(posi1 > 7) posi1 = 7;	
	
		mypix = (pixel){posi0, posi1};

	/*	
		// Einzelpixelroutine
		if((mypix.x != oldpix.x) || (mypix.y != oldpix.y) ){
			setpixel(oldpix,b);
		    b = get_pixel(mypix);
			setpixel(mypix, 31);
			
		//	if(get_pixel(mypix)){
		//		setpixel(mypix,3);
		//		b = 2;
		//	}else{
		//		setpixel(mypix,3);
		//		b = 0;
		//	}
			
		}
				
		oldpix = mypix;
	*/
		
		// BOBSCREEN
		#define bobfact	16
		if(bobscreen[mypix.x][mypix.y] < (63*bobfact)) bobscreen[mypix.x][mypix.y] += bobfact * 8;
		for(ix = 0;ix < 10; ix++) {
			for(iy = 0;iy < 8; iy++) {
				setpix(ix,iy,bobscreen[ix][iy] / (bobfact));
				if(bobscreen[ix][iy]>=1) bobscreen[ix][iy]-=1;
			}
		}
		
		wait(1);
	}

	
	for(;;){}

}
