/* -*- Mode: C; tab-width: 2 -*- */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "can/can_handler.h"
#include <util/delay.h>


#include "twi_master/twi_master.h"
#include "can/can_handler.h"
#include "../include/PowerCommander.h"

union {
	struct {
		uint8_t hauptschalter:1; // 1 Bit für bStatus_1
		uint8_t power_ok:1;    // Dieses Feld ist 2 Bits breit
		uint8_t rcd_server:1;
		uint8_t rcd_power:1;
		uint8_t rcd_licht:1;
	};
	uint8_t bla;
} stat_switches;



typedef struct{
	uint8_t class;
	uint8_t object;
	uint8_t function;
	uint8_t data;
}i2c_outdata;




volatile uint16_t tickscounter;

ISR(TIMER1_COMPA_vect)
{
	//976,5625 Hz
	
	// ueberlaeufe sind ok!	
	tickscounter++;
}


void switch_timer_init(){
	TCCR1B |=  _BV(WGM12) | 3; // CTC, clk/64

	//1000 Hz
	OCR1A = 250;   // pwm timer compare target
	
	TIMSK |= _BV(OCIE1A);							// Enable Timer1 Overflow Interrupt
}


#define HOLD_THRESHOLD 18
#define CLICK_THRESHOLD 0




void switch_handler()
{
	i2c_outdata outdata;
	
// Hauptschalter
	if (!(PINA & _BV(PA0)) && stat_switches.hauptschalter)
	{
		stat_switches.hauptschalter = 0;

		uint8_t msg[2];
		msg[0] =  stat_switches.bla;
		can_send(msg);
		
		outdata.class    = C_VIRT;
		outdata.object   = VIRT_POWER;
		outdata.function = F_SW_OFF;
		outdata.data     = 0x00;
		
		twi_send(&outdata);

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
		_delay_ms(500);
	}
	if ((PINA & _BV(PA0)) && stat_switches.hauptschalter == 0)
	{
		stat_switches.hauptschalter = 1;

		uint8_t msg[2];
		msg[0] =  stat_switches.bla;
		can_send(msg);

		outdata.class    = C_VIRT;
		outdata.object   = VIRT_POWER;
		outdata.function = F_SW_ON;
		outdata.data     = 0x00;
		
		twi_send(&outdata);
		
		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
		_delay_ms(500);
	}



	static uint16_t last_tickscounter;
	
	cli();
	uint16_t tc = tickscounter;
	sei();
	
	// alle 32 ticks ... 0.032 sekunden
	if((tc - last_tickscounter) >= 20 ){
		last_tickscounter = tc;


		// **** Vortrag Licht schalter ****

		static uint8_t counter_0;
		uint8_t clicked_0 = 0;
		uint8_t held_0    = 0;
		static uint8_t last_held_0;

		if (!(PINB & _BV(PB2))){
			counter_0 ++;
			if(counter_0 > HOLD_THRESHOLD){
				held_0 = 1;
				counter_0 = HOLD_THRESHOLD;
			}
		}else{
			if(counter_0 > CLICK_THRESHOLD){
				if(counter_0 < HOLD_THRESHOLD){
					clicked_0 = 1;
				}
			} 
			counter_0 = 0;
		}
		
		if(clicked_0){
			outdata.class    = C_VIRT;
			outdata.object   = VIRT_VORTRAG;
			outdata.function = F_SW_TOGGLE;
			outdata.data     = 0x00;
		
			twi_send(&outdata);
		}
		
		if(held_0){
			outdata.class    = C_VIRT;
			outdata.object   = VIRT_VORTRAG_PWM;
			outdata.function = F_PWM_MOD;
			outdata.data     = 0x00;
		
			twi_send(&outdata);
			
		}else if(last_held_0){
			outdata.class    = C_VIRT;
			outdata.object   = VIRT_VORTRAG_PWM;
			outdata.function = F_PWM_DIR;
			outdata.data     = 0x00;
		
			twi_send(&outdata);
				
		}
		
		last_held_0 = held_0;


		// **** Lounge Licht schalter ****

		static uint8_t counter_1;
		uint8_t clicked_1 = 0;
		uint8_t held_1    = 0;
		static uint8_t last_held_1;

		if (!(PIND & _BV(PD3))){
			counter_1 ++;
			if(counter_1 > HOLD_THRESHOLD){
				held_1 = 1;
				counter_1 = HOLD_THRESHOLD;
			}
		}else{
			if(counter_1 > CLICK_THRESHOLD){
				if(counter_1 < HOLD_THRESHOLD){
					clicked_1 = 1;
				}
			} 
			counter_1 = 0;
		}
		
		if(clicked_1){
			outdata.class    = C_SW;
			outdata.object   = SWL_LOUNGE;
			outdata.function = F_SW_TOGGLE;
			outdata.data     = 0x00;
		
			twi_send(&outdata);
		}
		
		if(held_1){
			outdata.class    = C_PWM;
			outdata.object   = PWM_LOUNGE;
			outdata.function = F_PWM_MOD;
			outdata.data     = 0x00;
		
			twi_send(&outdata);
			
		}else if(last_held_1){
			outdata.class    = C_PWM;
			outdata.object   = PWM_LOUNGE;
			outdata.function = F_PWM_DIR;
			outdata.data     = 0x00;
		
			twi_send(&outdata);
				
		}
		
		last_held_1 = held_1;
	}
	
	







	/*
// PC2 - 24V power good
	if (!(PINC & _BV(PC2)) && stat_switches.power_ok)
	{

		stat_switches.power_ok = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PINC & _BV(PC2)) && stat_switches.power_ok == 0)
	{
		stat_switches.power_ok = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
// PD6 - RCD Server
	if (!(PIND & _BV(PD6)) && stat_switches.rcd_server)
	{
		stat_switches.rcd_server = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PIND & _BV(PD6)) && stat_switches.rcd_server == 0)
	{
		uint8_t msg[] = {1, 0};
		msg[1] =  stat_switches.bla;
		can_send(msg);
		
		stat_switches.rcd_server = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
// PA1 - RCD Steckdosen 
	if (!(PINA & _BV(PA1)) && stat_switches.rcd_power)
	{
		stat_switches.rcd_power = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
	}
	if ((PINA & _BV(PA1)) && stat_switches.rcd_power == 0)
	{
		stat_switches.rcd_power = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
//  PD7 - RCD Licht
	if (!(PIND & _BV(PD7)) && stat_switches.rcd_licht)
	{
		stat_switches.rcd_licht = 0;

		PORTA |= _BV(PA3); // red
		PORTA &= ~_BV(PA2); // green
		_delay_ms(500);
	}
	if ((PIND & _BV(PD7)) && stat_switches.rcd_licht == 0)
	{
		stat_switches.rcd_licht = 1;

		PORTA |= _BV(PA2); // green
		PORTA &= ~_BV(PA3); // red
	}
	*/
}

