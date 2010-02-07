
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "touchscreen.h"
#include "dc_com.h"

uint8_t analog_reference = 1;

int analogRead(uint8_t pin)
{
        // set the analog reference (high two bits of ADMUX) and select the
        // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
        // to 0 (the default).
        ADMUX = (analog_reference << 6) | (pin & 0x07);
  
#if defined(__AVR_ATmega1280__)
        // the MUX5 bit of ADCSRB selects whether we're reading from channels
        // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
        ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif

		ADCSRA = (1<<ADEN)| 7;
        // start the conversion
        ADCSRA |= (1<<ADSC);

        // ADSC is cleared when the conversion finishes
        while (bit_is_set(ADCSRA, ADSC));

        return ADC;
}



#define BIG_DISPLAY

#define PORT_TOUCH PORTF
#define DDR_TOUCH DDRF

#ifdef BIG_DISPLAY
	#define MSK_YL  _BV(PF3)
	#define MSK_XH  _BV(PF2)
	#define MSK_YH  _BV(PF1)
	#define MSK_XL  _BV(PF0)
	
	#define TOUCH_Y_CHANNEL PF1
	#define TOUCH_X_CHANNEL PF2
#else
	#define MSK_YL  _BV(PF0)
	#define MSK_XH  _BV(PF1)
	#define MSK_YH  _BV(PF2)
	#define MSK_XL  _BV(PF3)
	
	#define TOUCH_Y_CHANNEL PF2
	#define TOUCH_X_CHANNEL PF1
#endif


#define TOUCH_OFF_MSK (~(MSK_YH|MSK_YL|MSK_XH|MSK_XL))

#define TOUCH_CLEAR_PORT() {DDR_TOUCH  &= TOUCH_OFF_MSK;}
#define TOUCH_CLEAR_DDR()  {PORT_TOUCH &= TOUCH_OFF_MSK;}

#define TOUCH_SET_DDR_TO_READ_Y()         {DDR_TOUCH  |= MSK_YH|MSK_YL;}
#define TOUCH_SET_PORT_TO_READ_Y()        {PORT_TOUCH |= MSK_YH;}

#define TOUCH_SET_DDR_TO_READ_X()         {DDR_TOUCH  |= MSK_XH|MSK_XL;}
#define TOUCH_SET_PORT_TO_READ_X()        {PORT_TOUCH |= MSK_XH;}


uint8_t sqrt_table[] = {0,10,14,17,20,22,24,26,28,30,32,33};

uint16_t my_sqrt(uint16_t d){
	if(d <= 11){
		return sqrt_table[d];
	}else{
		return d + 23;
	}

}

uint32_t variance(uint16_t val, uint16_t * data, uint8_t data_size){
	uint32_t var = 0;
	uint8_t i;
	for(i=0;i<data_size;i++){
		int16_t diff = data[i]-val;
		if(diff<0) diff = -diff;
		/*
		if(diff > 8){
			var += 8;
		}else{
			var += diff;
		}
		*/
		var += my_sqrt(diff);
		//var += (data[i]-val)*(data[i]-val);
	}

	return var;
}

#define FILTER 17
#define SCALE 1


uint16_t read_filtered(uint8_t channel){
	uint16_t readings[FILTER];
	
	uint8_t i;
	
	uint16_t min=0xffff; uint16_t max=0;
	
	for(i=0;i<FILTER;i++){
		uint16_t adc = analogRead(channel);
		readings[i] = adc;
		if(adc > max) max = adc;
		if(adc < min) min = adc;
	}

	uint32_t min_variance = 0xffffffff;
	
	uint8_t min_variance_i = 0;
	for(i=0;i<FILTER;i++){
		uint32_t var = variance(readings[i],readings,FILTER);
		if(var < min_variance){
			min_variance = var;
			min_variance_i = i;
		}
	}
	return readings[min_variance_i];

	/*
	uint16_t min_variance_guess;
	uint16_t guess;
	for(guess=min;guess<=max;guess++){
		uint32_t var = variance(guess,readings,FILTER);
		if(var < min_variance){
			min_variance = var;
			min_variance_guess = guess;
		}
	}
	return min_variance_guess;
	*/	
}


pixel read_touch_raw(){
	pixel p;
	
	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	
	TOUCH_SET_DDR_TO_READ_Y();
  	TOUCH_SET_PORT_TO_READ_X();
	
	__asm("nop;");__asm("nop;");__asm("nop;");
	_delay_us(100);

	
	if(analogRead(TOUCH_X_CHANNEL) > 200){
		p.x = -1;
		p.y = -1;
		return p;
	}
	
	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	TOUCH_SET_DDR_TO_READ_X();
	TOUCH_SET_PORT_TO_READ_X();
	
	_delay_us(100);
	
	//read y channel while X-Resistor is powered
		
	p.x = read_filtered(TOUCH_Y_CHANNEL);

	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	TOUCH_SET_DDR_TO_READ_Y();
	TOUCH_SET_PORT_TO_READ_Y();
		
	_delay_us(100);
/*
	akku = analogRead(TOUCH_X_CHANNEL)*SCALE*FILTER;
	
	for(i=0;i<FILTER;i++){
		akku+=analogRead(TOUCH_X_CHANNEL) * SCALE;
		akku -= akku/FILTER;
	}
*/
	p.y = read_filtered(TOUCH_X_CHANNEL);
	
	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	
	TOUCH_SET_DDR_TO_READ_Y();
  TOUCH_SET_PORT_TO_READ_X();

	__asm("nop;");__asm("nop;");__asm("nop;");

	_delay_us(100);

	
	if(analogRead(TOUCH_X_CHANNEL) > 400){
		p.x = -1;
		p.y = -1;
		return p;
	}
	
	
	return p;
}

uint16_t click_timer = 0;	
  	pixel p1 = {-1,-1}, p2;

void handle_touchscreen(){
	pixel p;
		p = read_touch_raw();

		if(p.x != -1){
//			p.x = ((p.x-200) * 16) /64;
//			p.y = ((p.y-260) * 20) /128;
			p.x = (p.x / 4)+ p.x/16;
			p.y = (p.y /4) ;
		}
		
#ifdef MOUSE_TEST

		//mouse cursor
		if( ((p1.x != p.x) || (p1.y != p.y ))  ){
			g_set_draw_color(0);
			//g_draw_cross(p1.x , p1.y);
		}
		
		if(p.x != -1){
			g_set_draw_color(1);
			g_draw_cross(p.x , p.y);
		}


		if((p1.x == -1 )&& (p.x != -1))			g_clear_screen();
#endif



#ifdef OSZI
	//Oszi
		static uint16_t xc=0;

		if(p.x != -1 ) g_draw_cross(xc , p.x);


		xc++;
		if(xc==320){
			xc=0;
		}
#endif
		

		uint8_t click = 0;
		
		if(p.x != -1 && p1.x == -1 && click_timer == 0){
			click = 1;
			click_timer = 1000;
		}


		if(p.x != -1){
			menu_handle_touch(p.x,p.y,click);
		}

	
		if(click_timer>0){
			click_timer--;
		}
		
		draw_menu(0);

  		p1 = p;

}