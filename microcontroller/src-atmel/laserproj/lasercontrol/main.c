/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"   //rs232 
#include "config.h"
#include "iccom.h"
//#include "kb.h"  //external PS/2 keyboard
//#include "menu.h"
#include "videoengine.h"
#include "eeprom.h"

/*
	lasercontrol
	made by siro 2011

*/

volatile uint16_t mode;
volatile uint16_t testvar=0;

/* prototypes */

void disable_safty(void);
void enable_safty(void);
void stop_timer1(void);
void start_timer1(void);
void stop_timer2(void);
void start_timer2(void);
void save_eeprom_values(void);
void load_eeprom_values(void);

void init(void)
{

	//******************
	DDRA=0xFF; //all pins output
	PORTA = 0x00;	//Internal Pullups all off

	//******************
	DDRB=0; //can be ignored, nothing here
	PORTB = 0x00;	//Internal Pullups all off

	//******************
	DDRC=0xFF; //output
	PORTC = 0;//Internal Pullups all off

	//******************
	DDRD=_BV(PD5); //PD5 BLINK_LED
	PORTD =_BV(PD6);	//Internal Pullups all off,except power button


//	MCUCR |= _BV(SE); // Enable "sleep" mode (low power when idle)

	ACSR = _BV(ACD); // Disable Analog Comparator (power save)


}

int main (void)
{
	uint8_t main_delay=0;
	uint8_t eeprom_wait_write=0;	//counter up to 10 seconds eeprom write delay
	void* menu_val_changed;
		
		init();		//init ports

		PORTD|=_BV(PIN_BLINKLED); //toggle PIN_BLINKLED

		uart_init(); //init uart
		enable_safty(); //laser off
	//	init_kb();	//init ps2 keyboard
		load_eeprom_values(); //load all values from eeprom and place in correct vars
	
		/*	
		menu_init_entry((void *)&mode,sizeof(mode),PSTR("mode"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.h_sync_wait_perc,sizeof(ve.h_sync_wait_perc),PSTR("h_sync_wait %"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.v_sync_wait_perc,sizeof(ve.v_sync_wait_perc),PSTR("v_sync_wait %"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.h_pixel_perc,sizeof(ve.h_pixel_perc),PSTR("h_viewfield %"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.v_pixel_perc,sizeof(ve.v_pixel_perc),PSTR("v_viewfield %"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.h_pixel_end_val,sizeof(ve.h_pixel_end_val),PSTR("h_pixel"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.v_pixel_end_val,sizeof(ve.v_pixel_end_val),PSTR("v_pixel"),MENU_READANDWRITE);
		menu_init_entry((void *)&ve.h_line_cnt_avr,sizeof(ve.h_line_cnt_avr),PSTR("h lines ps"),MENU_READONLY);
		menu_init_entry((void *)&ve.frame_cnt_avr,sizeof(ve.frame_cnt_avr),PSTR("frames ps"),MENU_READONLY);
		menu_init_entry((void *)&testvar,sizeof(testvar),PSTR("testvar"),MENU_READANDWRITE);
		menu_init_entry((void *)&OCR1A,sizeof(OCR1A),PSTR("Timer 1 V"),MENU_READANDWRITE);
		menu_init_entry((void *)&OCR2,sizeof(OCR2),PSTR("Timer2 H"),MENU_READANDWRITE);
				                  
		menu_init();	//init the GLCD and menu
		*/
		icc_init((void *)&mode,sizeof(mode), ICC_DIRECTION_BOTH);	 //link var to icc
		icc_init((void *)&ve.h_sync_wait_perc,sizeof(ve.h_sync_wait_perc), ICC_DIRECTION_BOTH); //link var to icc
		icc_init((void *)&ve.v_sync_wait_perc,sizeof(ve.v_sync_wait_perc), ICC_DIRECTION_BOTH); //link var to icc
		icc_init((void *)&ve.h_pixel_perc,sizeof(ve.h_pixel_perc), ICC_DIRECTION_BOTH); //link var to icc
		icc_init((void *)&ve.v_pixel_perc,sizeof(ve.v_pixel_perc), ICC_DIRECTION_BOTH); //link var to icc
		icc_init((void *)&ve.h_pixel_end_val,sizeof(ve.h_pixel_end_val), ICC_DIRECTION_BOTH); //link var to icc
		icc_init((void *)&ve.v_pixel_end_val,sizeof(ve.v_pixel_end_val), ICC_DIRECTION_BOTH); //link var to icc
//		icc_init((void *)&ve.h_line_cnt_avr,sizeof(ve.h_line_cnt_avr), ICC_DIRECTION_WRITEONLY); //link var to icc
//		icc_init((void *)&ve.frame_cnt_avr,sizeof(ve.frame_cnt_avr), ICC_DIRECTION_WRITEONLY); //link var to icc
//		icc_init((void *)&testvar,sizeof(testvar),ICC_DIRECTION_READONLY); //link var to icc
		PORTD&=~_BV(PIN_BLINKLED); //toggle PIN_BLINKLED
		sei();  //global enable interrupts 
		
		icc_send((void *)&mode);
		icc_send((void *)&ve.h_sync_wait_perc);
		icc_send((void *)&ve.v_sync_wait_perc);
		icc_send((void *)&ve.h_pixel_perc);
		icc_send((void *)&ve.v_pixel_perc);
		icc_send((void *)&ve.h_pixel_end_val);
		icc_send((void *)&ve.v_pixel_end_val);

	
	while (1) {
		main_delay++;	 //internal counter
				
			if(uart_rxbuffer_notempty()){		//check for byte arrived on uart_init
				if(icc_handle_uart()){		//handle it
					eeprom_wait_write=1;//menu_val_changed=1;//menu_draw(0);	//redraw menu_init
					PORTD|=_BV(PIN_BLINKLED); //toggle PIN_BLINKLED
				}
			}
			//menu_val_changed= menu_update();	//update menu
			//if(menu_val_changed != NULL)
			//{
			//	icc_send(menu_val_changed);//send if something was updated
			//	eeprom_wait_write=1;
			//}
				if(!(main_delay%10)){

					if(POWER_BUTTON_PRESSED)	 //check if powerbutton is pressed
					{
							while(POWER_BUTTON_PRESSED)	//loop while powerbutton is pressed
							{
								if(uart_rxbuffer_notempty())		//check for byte arrived on uart_init
									if(icc_handle_uart())		//handle it
										eeprom_wait_write=1;
							}

						if(	PORTC & _BV(PC5)) //is on ?
						{
							enable_safty(); //laser off
							stop_timer2();	//stop timer 2
							stop_timer1();  //stop timer 1
							PORTD&=~_BV(PIN_BLINKLED); //turn off PIN_BLINKLED
						}
							else
						{
							start_timer1();
							start_timer2();
							_delay_ms(5000);
							disable_safty();
						}
			
					}
				}

				if(main_delay >= 250){
					main_delay=0;
					if(	PORTC & _BV(PC5)) //is on ?
						PORTD^=_BV(PIN_BLINKLED); //toggle PIN_BLINKLED
					else
							PORTD&=~_BV(PIN_BLINKLED); //off PIN_BLINKLED
					if(eeprom_wait_write){
						eeprom_wait_write++;
						if(eeprom_wait_write > 40)	//10 seconds
						{
								save_eeprom_values();
								eeprom_wait_write=0;
						}
					}
				}

			_delay_ms(1);
	}


}

void start_timer2(void)
{
	/*	16.000.000/256 = 62500
	*   OCR2 = 62500 / 800 = 78
	*/
	DDRD|=_BV(PD7); //make OC2 output
	TCCR2=_BV(COM20)|_BV(WGM21)|_BV(CS22)|_BV(CS21);		//toggle on OCR match, Clear on OCR match, prescaler 1:256
	TCNT2=0;	//clear timer2
	OCR2= 78; //set to 800 hz toggle
}

void stop_timer2(void)
{
	DDRD&=~_BV(PD7);
	TCCR2=0;	//reset
	TCNT2=0; //reset
}

void start_timer1(void)
{
	DDRD|=_BV(PD4); //make OC1B output
	TCCR1A=_BV(COM1B1)|_BV(WGM11)|_BV(WGM10);		//set pin on 0,clear at top, Fast PWM 10 bit
	TCCR1B=_BV(WGM12)|_BV(CS11)|_BV(CS10); //1:64 prescaler
	TCNT1=0;		//reset
}

void stop_timer1(void)
{
	DDRD&=~_BV(PD4);
	TCCR1A=0;		//reset
	TCCR1B=0; //reset
	TCNT1=0;		//reset
}

void enable_safty(void)//disallow laser on
{
				PORTC&=~_BV(PC5);
}

void disable_safty(void)		//allow laser on
{
				PORTC|=_BV(PC5);
}

void load_eeprom_values(void)
{
		eeprom_t predefvalues;
		eeprom_read(&predefvalues);
		mode=predefvalues.mode;
		ve.h_sync_wait_perc=predefvalues.h_sync_wait_perc;
		ve.v_sync_wait_perc=predefvalues.v_sync_wait_perc;
		ve.h_pixel_perc=predefvalues.h_pixel_perc;
		ve.v_pixel_perc=predefvalues.v_pixel_perc;
		ve.h_pixel_end_val=predefvalues.h_pixel_end_val;
		ve.v_pixel_end_val=predefvalues.v_pixel_end_val;
		OCR1A=predefvalues.timer1_OCR1A;
	//	OCR2=predefvalues.timer2_OCR2;
}

void save_eeprom_values(void)
{
		uint8_t i=0;
		for(i=0;i<10;i++){
			PORTD^=_BV(PIN_BLINKLED); //toggle PIN_BLINKLED
			_delay_ms(50);
		}
		
		eeprom_t predefvalues;
		predefvalues.mode=mode;
		predefvalues.h_sync_wait_perc=ve.h_sync_wait_perc;
		predefvalues.v_sync_wait_perc=ve.v_sync_wait_perc;
	  predefvalues.h_pixel_perc=ve.h_pixel_perc;
	  predefvalues.v_pixel_perc=ve.v_pixel_perc;
		predefvalues.h_pixel_end_val=ve.h_pixel_end_val;
		predefvalues.v_pixel_end_val=ve.v_pixel_end_val;
		predefvalues.timer1_OCR1A=OCR1A;
	//	predefvalues.timer2_OCR2=OCR2;
		eeprom_write(&predefvalues);
}
