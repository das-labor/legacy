
#include <avr/io.h>
#include<stdlib.h>
#include "../include/dc_commands.h"
#include <util/delay.h>

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


#define PORT_HANDSHAKE  PORTC
#define DDR_HANDSHAKE   DDRC
#define PIN_HANDSHAKE   PINC


#define BIT_ATN PC7
#define BIT_ACK PC6

#define ATN_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ATN);DDR_HANDSHAKE |= _BV(BIT_ATN);}
#define ATN_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ATN);PORT_HANDSHAKE |= _BV(BIT_ATN);}

#define ACK_PULL()    {PORT_HANDSHAKE &= ~_BV(BIT_ACK);DDR_HANDSHAKE |= _BV(BIT_ACK);}
#define ACK_RELEASE() {DDR_HANDSHAKE &= ~_BV(BIT_ACK);PORT_HANDSHAKE |= _BV(BIT_ACK);}

#define WAIT_ACK_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ACK));}
#define WAIT_ACK_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ACK))==0);}

#define WAIT_ATN_LOW()     {while(PIN_HANDSHAKE & _BV(BIT_ATN));}
#define WAIT_ATN_HIGH()    {while((PIN_HANDSHAKE & _BV(BIT_ATN))==0);}

void init(){
	DDR_HANDSHAKE  &= ~(_BV(BIT_ATN)|_BV(BIT_ACK));
	PORT_HANDSHAKE |=  (_BV(BIT_ATN)|_BV(BIT_ACK));
	
	//Backlight on
	DDRF |= _BV(PF4);
	PORTF |= _BV(PF4);
	
			
	
}



void dc_byte_put(uint8_t b){
	DDRA = 0xff;
	PORTA = b;

	WAIT_ATN_HIGH();
	ATN_PULL();
	WAIT_ACK_LOW();
	ATN_RELEASE();
	WAIT_ACK_HIGH();

}

void transmit_to_dc(uint8_t command, uint16_t size, void * data){
	uint16_t i;

	dc_byte_put(command);

	if(command >= 0x80){
		dc_byte_put(size);
		dc_byte_put(size>>8);
	}

	for(i=0;i<size;i++){
		dc_byte_put(((uint8_t*)data)[i]);
	}
}


void g_draw_cross(uint16_t x, uint16_t y){
	uint16_t data[2];
	data[0] = x;
	data[1] = y;
	
	transmit_to_dc(DC_DRAW_CROSS, 4, data); 
}

void set_draw_color(uint8_t color){
	uint8_t data[1];
	data[0] = color;

	transmit_to_dc(DC_SET_COLOR, 1, data); 

}

void g_clear_screen(){

	transmit_to_dc(DC_CLEAR_SCREEN, 0, 0); 
}

typedef struct{
	int16_t x;
	int16_t y;
} pixel;

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




pixel read_touch_raw(){
	pixel p;
	
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
	
	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	TOUCH_SET_DDR_TO_READ_X();
	TOUCH_SET_PORT_TO_READ_X();
	
	_delay_us(100);
	
	//read y channel while X-Resistor is powered
	
	uint16_t akku = 0, i;
	
	#define FILTER 16
	

	akku = analogRead(TOUCH_Y_CHANNEL);
	
	for(i=0;i<FILTER;i++){
		akku+=analogRead(TOUCH_Y_CHANNEL);
	}


	uint16_t v1,v2=0;
	uint8_t cnt = 0;
	/*
	while(1){
		v1 = analogRead(TOUCH_Y_CHANNEL);
		if((v1 < (v2+2))  && (v1 >(v2-2)) ) {
			cnt++;
			if(cnt > 4) break;
		}else{
			cnt = 0;
		}
		v2=v1;
	}
	
	*/
	
	p.x = akku/FILTER;

	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	TOUCH_SET_DDR_TO_READ_Y();
	TOUCH_SET_PORT_TO_READ_Y();
		
	_delay_us(100);
	
	akku = 0;
	
	for(i=0;i<FILTER;i++)
		akku += analogRead(TOUCH_X_CHANNEL);

	p.y = akku/FILTER;
	
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



int main (void)
{	

	init();

//	uart_init();
// 	fdevopen (put, 0);

//	draw_color = PIXEL_ON;
//  printf("hello %d\r\n\f",1234);
//  lcd_graphics_init();
//	lcd_graphics_clear();
	//g_draw_string(20, 20, "Graphics Demo!  Hello World!"); 
	//rectangle_t r ={ 40,40,23,19};
	//g_draw_string_in_rect_vert(&r, "Graphics Demo!  Hello World!");
	//g_draw_string_in_rect(&r, "plplplplplplplplplplplplplplplp");
	//g_draw_rectangle(&r);
	//draw_font = &font_f04b24;
	//g_draw_string(20, 40, "Graphics Demo!  Hello World!"); 
	//draw_font = &font_small6;
	//g_draw_string(20, 60, "Graphics Demo!  Hello World!"); 
//	lcd_graphics_copy_P(0, 0, 64, 64, laborlogo);
//	g_draw_line(0,0,70,80, PIXEL_ON);

  	pixel p, p1 = {-1,-1}, p2;
	
//	menu_test();
	
	uint16_t click_timer = 0;	
		
	set_draw_color(1);

	char linebuff[20];
		
	uint16_t xc=0;
	while(1){
		p = read_touch_raw();

		if(p.x != -1){
//			p.x = ((p.x-200) * 16) /64;
//			p.y = ((p.y-260) * 20) /128;
			p.x = (p.x / 4)+ p.x/16;
			p.y = (p.y /4) ;
		}
		

		itoa(p2.x, linebuff, 10);
//		g_draw_string(68, 0, linebuff);
//		g_draw_rectangle(68, 0, 100 ,10);

		itoa(p2.y, linebuff, 10);
//		g_draw_string(68, 10, linebuff);
//		g_draw_rectangle(68, 10, 100 ,10);



		//mouse cursor
		if( ((p1.x != p.x) || (p1.y != p.y ))  ){
			set_draw_color(0);
			//g_draw_cross(p1.x , p1.y);
		}
		
		if(p.x != -1){
			set_draw_color(1);
			g_draw_cross(p.x , p.y);
		}


		if((p1.x == -1 )&& (p.x != -1))			g_clear_screen();


/*
	//Oszi

		if(p.x != -1 ) g_draw_cross(xc , p.y);


		xc++;
		if(xc==320){
			xc=0;
		}
*/
		

		uint8_t click = 0;
		
		if(p.x != -1 && p1.x == -1 && click_timer == 0){
			click = 1;
			click_timer = 1000;
		}


		if(p.x != -1){
			//menu_handle_touch(p.x,p.y,click);
		}

	
		if(click_timer>0){
			click_timer--;
		}
		

		//draw_menu(0);

  		p1 = p;

		//_delay_ms(100);

	}

	

  //delay_ms_long(50);
  	
	return 0;
}

