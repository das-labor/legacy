
#include <avr/interrupt.h>
#include "touchscreen.h"
#include "adc.h"

//define BIG DISPLAY in here
#include "../include/personal_config.h"
#include "config.h"


#define ADC_CHANNEL_BACKLIGHT 4


#define TOUCHSCREEN_AREFS (1<<REFS0)

#define TOUCH_OFF_MSK (~(MSK_YH|MSK_YL|MSK_XH|MSK_XL))

#define TOUCH_CLEAR_PORT() {PORT_TOUCH  &= TOUCH_OFF_MSK;}
#define TOUCH_CLEAR_DDR()  {DDR_TOUCH   &= TOUCH_OFF_MSK;}

#define TOUCH_SET_DDR_TO_READ_Y()         {DDR_TOUCH  |= MSK_YH|MSK_YL;}
#define TOUCH_SET_PORT_TO_READ_Y()        {PORT_TOUCH |= MSK_YH;}

#define TOUCH_SET_DDR_TO_READ_X()         {DDR_TOUCH  |= MSK_XH|MSK_XL;}
#define TOUCH_SET_PORT_TO_READ_X()        {PORT_TOUCH |= MSK_XH;}


#define TOUCH_THRESHOLD 300

#define TOUCH_FILTER 8

pixel touchscreen_position_raw;

void update_touchscreen() {
	static uint8_t state;
	static uint8_t got_data;
	static pixel p;
	static uint16_t akk;
	static uint8_t count;

	switch (state) {
		case 0:
			TOUCH_CLEAR_DDR();
			TOUCH_CLEAR_PORT();

			TOUCH_SET_PORT_TO_READ_Y();
			TOUCH_SET_DDR_TO_READ_Y(); //charge cap
			state = 1;
			break;
		case 1:
			//_delay_us(100);
			TOUCH_CLEAR_DDR();
			TOUCH_SET_DDR_TO_READ_X();
			ADMUX = TOUCHSCREEN_AREFS | TOUCH_Y_CHANNEL;
			state = 2;
			break;
		case 2:
			if (ADC > TOUCH_THRESHOLD) {
				touchscreen_position_raw.x = -1;
				touchscreen_position_raw.y = -1;
				ADMUX = TOUCHSCREEN_AREFS | TOUCH_Y_CHANNEL;
				state = 2;
				got_data = 0;
			} else {
				if(got_data){
					touchscreen_position_raw.x = p.x;
					touchscreen_position_raw.y = p.y;
				}
				TOUCH_CLEAR_DDR();
				TOUCH_CLEAR_PORT();
				TOUCH_SET_DDR_TO_READ_X();
				TOUCH_SET_PORT_TO_READ_X();
				//_delay_us(100);

				//read y channel while X-Resistor is powered
				ADMUX = TOUCHSCREEN_AREFS | TOUCH_Y_CHANNEL;
				akk = 0;
				count = 0;
				state = 3;
			}
			break;
		case 3:
			if (count < TOUCH_FILTER) {
				count++;
				akk += ADC;
				ADMUX = TOUCHSCREEN_AREFS | TOUCH_Y_CHANNEL;
			} else {
				p.x = akk/TOUCH_FILTER;

				TOUCH_CLEAR_DDR();
				TOUCH_CLEAR_PORT();
				TOUCH_SET_DDR_TO_READ_Y();
				TOUCH_SET_PORT_TO_READ_Y();

				//_delay_us(100);

				ADMUX = TOUCHSCREEN_AREFS | TOUCH_X_CHANNEL;
				akk = 0;
				count = 0;
				state = 4;
			}
			break;
		case 4:
			if (count < TOUCH_FILTER) {
				count++;
				akk += ADC;
				ADMUX = TOUCHSCREEN_AREFS | TOUCH_X_CHANNEL;
			} else {
				p.y = akk/TOUCH_FILTER;
				got_data = 1;

				TOUCH_CLEAR_DDR();
				TOUCH_CLEAR_PORT();

				TOUCH_SET_PORT_TO_READ_Y();
				TOUCH_SET_DDR_TO_READ_Y(); //charge cap
				//_delay_us(100);
				state = 1;
			}
			break;
	}
}

uint16_t adc_backlight;

ISR(ADC_vect) {
	static uint8_t state;
	if (state == 0) {
		//read for backlight completed. conversion for touchscreen is going.
		state = 1;
		adc_backlight = ADC;
		backlight();
		//select AVCC reference and backlight channel again for next conversion
		ADMUX = (1<<REFS0) | ADC_CHANNEL_BACKLIGHT;
	} else {
		state = 0;
		//read for touchscreen completed.
		update_touchscreen();
	}

}

void init_adc() {
	//REFS1 REFS0 ADLAR MUX4 MUX3 MUX2 MUX1 MUX0      ADMUX
	//REFS1 REFS0 Voltage Reference Selection
	//0 0 AREF, Internal Vref turned off
	//0 1 AVCC with external capacitor at AREF pin
	//1 0 Reserved
	//1 1 Internal 2.56V Voltage Reference with external capacitor at AREF pin

	ADMUX = 0;

	//ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0      ADCSRA
	//ADPS2 ADPS1 ADPS0 Division Factor
	//0 0 0 2
	//0 0 1 2
	//0 1 0 4
	//0 1 1 8
	//1 0 0 16
	//1 0 1 32
	//1 1 0 64
	//1 1 1 128
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADIE) | 7;
	


}
