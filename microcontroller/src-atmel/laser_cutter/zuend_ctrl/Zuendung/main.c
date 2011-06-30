/*
 * Zuendung.c
 *
 * Created: 23.05.2011 17:58:54
 *  Author: jkerrinnes
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdlib.h>
#include <util/delay.h>
#include "config.h"
#include "lcd-routines.h"


volatile uint16_t x_pos, y_pos;
volatile uint16_t radius, radius_q;
volatile uint16_t ticks, ticks_fire;
volatile uint16_t delay_count, delay;
volatile int8_t enc_delta;				// -128 ... 127
volatile uint8_t start, menu, activate, fire,over;

//EEPROM******************************
uint16_t EEMEM eeprom_delay;
uint16_t EEMEM eeprom_radius;

uint16_t EEMEM eeprom_delay_1;
uint16_t EEMEM eeprom_delay_2;
uint16_t EEMEM eeprom_delay_3;

uint16_t EEMEM eeprom_radius_1;
uint16_t EEMEM eeprom_radius_2;
uint16_t EEMEM eeprom_radius_3;
//************************************

void encode_init( void )
{

	PORTC |= (_BV(PC6) | _BV(PC7));
	PORTA |= (_BV(PA0));
  	TCCR0 = 1<<WGM01;
	TCCR0 = (1 <<CS01) | (1<<CS00);				// CTC, XTAL / 64
	OCR0 = (uint8_t)(XTAL / 64.0 * 1e-3 - 0.5);	// 1ms
	TIMSK |= 1<<OCIE0;
}

int8_t encode_read( void )			// read single step encoders
{
  int8_t val;

  cli();
  val = enc_delta / 2;
  enc_delta -= val*2;
  sei();
  return val;					// counts since last call
}

void splash_screen(){
	lcd_clear(); //LCD löschen
	lcd_setcursor(0,1);
  	lcd_string("LASER Pulse V0.4");
	lcd_setcursor(0,2);
	lcd_string("--Das Labor--");
	_delay_ms(500);
}

void handle_display(){
	static int16_t val = 1;
	static int16_t val_old = 1;
	static temp;
	char s[7];
	val += encode_read()/2;

	if(!ENABLE){
	lcd_clear();
	lcd_setcursor(0,1);
	}	
		
	//Hauptmenu********************************
	if (menu == MENU_MAIN){
		if (val > MAINMENU_MAX) val = MAINMENU_MAX;
		if (val <  1) val =  1;
		switch (val){
			case MENU_DURCHMESSER:
				lcd_string("-->Radius");
			break;
			
			case MENU_FREQUENZ:
				lcd_string("-->Delay");
			break;
			
			case MENU_START:			
				lcd_string("-->Start");
			break;
			
			case MENU_SAVE:
				lcd_string("-->Speichern");
			break;
			
			case MENU_LOAD:
				lcd_string("-->Laden");
			break;
		}
	}
	//*****************************************
		
	//Durchmesser-Menü*************************
	if (menu == MENU_DURCHMESSER){
		if (val > 200) val = 200;
		if (val <  1) val =  1;
		temp = val;
		lcd_string("Radius eingeben:");
		lcd_setcursor(0,2);
		lcd_string(itoa(temp,s,10));
		lcd_string(" Schritte");
	}	
	//*****************************************		
		
	//Frequenz-Menü*************************
	if (menu == MENU_FREQUENZ){
		if (val > 100) val = 100;
		if (val <  5) val =  5;
		temp = val * 10;
		lcd_string("Delay eingeben:");
		lcd_setcursor(0,2);
		lcd_string(itoa(temp,s,10));
		lcd_string(" ms");
	}	
	//*****************************************		
	
	//Enabled-Menü*************************
	if (menu == MENU_START && !ENABLE){
		start = 1;
		
		lcd_string("!READY!");
		
		if (!delay_count){
			lcd_setcursor(0,2);
			lcd_string("X:");
			lcd_string(itoa(x_pos,s,10));
			lcd_string("  Y:");
			lcd_string(itoa(y_pos,s,10));
		}			
	}	
	//*****************************************	
	
	//Speichern-Laden-Menü*******************
	if (menu == MENU_SAVE | menu == MENU_LOAD){
		if (val > PARAM_MAX) val = PARAM_MAX;
		if (val <  1) val =  1;
		lcd_string("Parametersatz:");
		temp = val;
		lcd_setcursor(0,2);
		switch (val){
			case PARAM_DEFAULT:
				lcd_string("Default");
			break;
			case PARAM_1:
				lcd_string("Parametersatz 1");
			break;
			case PARAM_2:
				lcd_string("Parametersatz 2");
			break;
			case PARAM_3:
				lcd_string("Parametersatz 3");
			break;
		}				
	}	
	//******************************************
	
	
	//Bei betätigung des Tasters**********	
	if (activate==1){
		activate = 0;
		switch (menu){
			case MENU_MAIN:
				if (val==MENU_START){
					menu = MENU_START;
					x_pos = radius;
					y_pos = radius;
				}
				else if (val==MENU_DURCHMESSER){
					menu = MENU_DURCHMESSER;
					val = radius;
				}
				else if (val==MENU_FREQUENZ){
					menu = MENU_FREQUENZ;
					val = delay/10;
				}
				else if (val==MENU_SAVE){
					menu = MENU_SAVE;
				}
				
				else if (val==MENU_SAVE){
					menu = MENU_SAVE;
					val = 1;
				}
				else if (val==MENU_LOAD){
					menu = MENU_LOAD;
					val = 1;
				}
			break;
			
			case MENU_DURCHMESSER:
				radius = temp;
				radius_q = radius * radius;
				menu = MENU_MAIN;
				val = MENU_START;
			break;
			
			case MENU_FREQUENZ:
				delay = temp;
				menu = MENU_MAIN;
				val = MENU_START;
			break;
			
			case MENU_START:
				menu = MENU_MAIN;
				start = 0;
			break;
			
			case MENU_SAVE:
				lcd_clear();
				lcd_setcursor(0,1);
				lcd_string("Speichern...");
				switch (temp){
					case PARAM_DEFAULT:
						eeprom_write_word(&eeprom_delay,delay);
						eeprom_write_word(&eeprom_radius,radius);
					break;
					case PARAM_1:
						eeprom_write_word(&eeprom_delay_1,delay);
						eeprom_write_word(&eeprom_radius_1,radius);
					break;
					case PARAM_2:
						eeprom_write_word(&eeprom_delay_2,delay);
						eeprom_write_word(&eeprom_radius_2,radius);
					break;
					case PARAM_3:
						eeprom_write_word(&eeprom_delay_3,delay);
						eeprom_write_word(&eeprom_radius_3,radius);
					break;
				}				
				_delay_ms(400);
			
				menu = MENU_MAIN;
				val	= MENU_START;	
			break;
			
			case MENU_LOAD:
				lcd_clear();
				lcd_setcursor(0,1);
				lcd_string("Laden...");
				lcd_setcursor(0,2);
				switch (temp){
					case PARAM_DEFAULT:
						lcd_string("Default");
						radius  = eeprom_read_word(&eeprom_radius);
						delay = eeprom_read_word(&eeprom_delay);
					break;
					case PARAM_1:
						lcd_string("Parametersatz 1");
						radius  = eeprom_read_word(&eeprom_radius_1);
						delay = eeprom_read_word(&eeprom_delay_1);
					break;
					case PARAM_2:
						lcd_string("Parametersatz 2");
						radius  = eeprom_read_word(&eeprom_radius_2);
						delay = eeprom_read_word(&eeprom_delay_2);
					break;
					case PARAM_3:
						lcd_string("Parametersatz 3");
						radius  = eeprom_read_word(&eeprom_radius_3);
						delay = eeprom_read_word(&eeprom_delay_3);
					break;
				}				
				_delay_ms(400);
			
				menu = MENU_MAIN;
				val	= MENU_START;	
			break;
		}
	}
	//**************************************
	

	val_old = val;
}


int main(void)
{
	cli();
	FIRE_ON;
	//EEPROM Lesen*******
	radius  = eeprom_read_word(&eeprom_radius);
	delay = eeprom_read_word(&eeprom_delay);
	//*******************
	
	menu = MENU_MAIN;
	x_pos = radius;
	y_pos = radius;
	radius_q = radius * radius;
	
	
	PULL_DIR_X_ON;
	PULL_DIR_Y_ON;
	PULL_STEP_X_ON;
	PULL_STEP_Y_ON;
	PULL_ENABLE_ON;
	uint16_t xb, yb, distance;
	uint8_t taster;
  	uint8_t old_taster = 0;
		
	
	MCUCR = (1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
	MCUCSR = (0<<ISC2);
	GICR = (1<<INT1) | (1<<INT0) | (1<<INT2);
	DDRD= 1<<PD6; 
	DDRA= 1<<PA4; 
	
	//Alles Initialisieren*************
	lcd_init(); //LCD INIT
  	encode_init(); //ENCODER INIT
	splash_screen();
	//*********************************

	sei();
    while(1)
    {
		cli();
		
		if(ticks >= 200 && !delay_count){
			ticks = 0;
			handle_display();
		}
		
		taster = !(TASTER);
		if(taster && (!old_taster)){
			//Taster wurde gerade gedrückt
			activate = 1;
		}
		
		if (!ENABLE){
			x_pos = radius;
			y_pos = radius;
		}		
		if (start==1 && delay_count){
			
			xb = x_pos - radius;
			yb = y_pos - radius;
			distance = (xb * xb );
			distance = distance + (yb * yb);
			if (distance >= radius_q || fire)
			{
			OVER_ON;		
			}			
		}
		if (ticks == 190){
			OVER_OFF;
		}			
				
		if (start==1 && !delay_count){
			xb = x_pos - radius;
			yb = y_pos - radius;
			distance = (xb * xb );
			distance = distance + (yb * yb);
			if (distance >= radius_q || fire)
			{	
				fire = 0;
				FIRE_OFF;
				_delay_ms(1);
				FIRE_ON;
				delay_count = delay;
				x_pos = radius;
				y_pos = radius;
			}
		}	
		
		old_taster = taster;	
		sei();
		
		
    }
}

ISR(INT0_vect) //X-Achse
{
	cli();
	if (DIR_X && ENABLE) 
	{
		x_pos++;
	}
	else
	{
		x_pos--;
	}
	sei();		
} 

ISR(INT1_vect) //Y-Achse
{
	cli(); 
   	if (DIR_Y && ENABLE) 
	{
		y_pos++;
	}
	else
	{
		y_pos--;
	}	
	sei();
} 

ISR(INT2_vect) //Z-Achse
{
	cli(); 
   	if (ENABLE) 
	{
		fire =  1;
	}
	sei();
} 

ISR( TIMER0_COMP_vect )				// 1ms for manual movement
{
	static int8_t last;
  	int8_t new, diff;

	ticks++;

	if(delay_count)
		delay_count -= 1;
	
  	new = 0;
  	if( PHASE_A )
    	new = 3;
  	if( PHASE_B )
    	new ^= 1;							// convert gray to binary
  	diff = last - new;						// difference last - new
  	if( diff & 1 )							// bit 0 = value (1)
	{				
    	last = new;							// store new as next last
    	enc_delta += (diff & 2) - 1;		// bit 1 = direction (+/-)
	}
}



