#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "touchscreen.h"
#include "dc_com.h"

//define BIG DISPLAY in here
#include "../include/personal_config.h"

#include "config.h"


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
        ADCSRB = (ADCSRB & ~_BV(MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif

	ADCSRA = _BV(ADEN) | 7;
        // start the conversion
        ADCSRA |= _BV(ADSC);

        // ADSC is cleared when the conversion finishes
        while (bit_is_set(ADCSRA, ADSC));

        return ADC;
}


#define TOUCH_OFF_MSK (~(MSK_YH|MSK_YL|MSK_XH|MSK_XL))

#define TOUCH_CLEAR_PORT() {PORT_TOUCH  &= TOUCH_OFF_MSK;}
#define TOUCH_CLEAR_DDR()  {DDR_TOUCH   &= TOUCH_OFF_MSK;}

#define TOUCH_SET_DDR_TO_READ_Y()         {DDR_TOUCH  |= MSK_YH|MSK_YL;}
#define TOUCH_SET_PORT_TO_READ_Y()        {PORT_TOUCH |= MSK_YH;}

#define TOUCH_SET_DDR_TO_READ_X()         {DDR_TOUCH  |= MSK_XH|MSK_XL;}
#define TOUCH_SET_PORT_TO_READ_X()        {PORT_TOUCH |= MSK_XH;}


uint8_t sqrt_table[] = {0, 10, 14, 17, 20, 22, 24, 26, 28, 30, 32, 33};

uint16_t my_sqrt(uint16_t d) {
	if (d <= 11) {
		return sqrt_table[d];
	} else {
		return d + 23;
	}
}

uint32_t variance(uint16_t val, uint16_t * data, uint8_t data_size) {
	uint32_t var = 0;
	uint8_t i;
	for (i = 0; i < data_size; i++) {
		int16_t diff = data[i] - val;
		if (diff < 0) diff = - diff;
		/*
		if (diff > 8) {
			var += 8;
		} else {
			var += diff;
		}
		*/
		var += my_sqrt(diff);
		//var += (data[i] - val) * (data[i] - val);
	}
	return var;
}

#define FILTER 17
#define SCALE 1


uint16_t read_filtered(uint8_t channel) {
	uint16_t readings[FILTER];
	
	uint8_t i;
	
	uint16_t min = 0xffff; uint16_t max = 0;
	
	for (i = 0; i < FILTER; i++) {
		uint16_t adc = analogRead(channel);
		readings[i] = adc;
		if (adc > max) max = adc;
		if (adc < min) min = adc;
	}

	uint32_t min_variance = 0xffffffff;
	
	uint8_t min_variance_i = 0;
	for (i = 0; i < FILTER; i++) {
		uint32_t var = variance(readings[i], readings, FILTER);
		if (var < min_variance) {
			min_variance = var;
			min_variance_i = i;
		}
	}
	return readings[min_variance_i];

	/*
	uint16_t min_variance_guess;
	uint16_t guess;
	for (guess = min; guess <= max; guess++) {
		uint32_t var = variance(guess, readings, FILTER);
		if (var < min_variance) {
			min_variance = var;
			min_variance_guess = guess;
		}
	}
	return min_variance_guess;
	*/	
}

#define TOUCH_THRESHOLD 600

pixel read_touch_raw() {
	pixel p;
	
	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	
	TOUCH_SET_PORT_TO_READ_Y();
	TOUCH_SET_DDR_TO_READ_Y(); //charge cap
	_delay_us(100);
	TOUCH_CLEAR_DDR();

	TOUCH_SET_DDR_TO_READ_X();
  	
	
	_delay_us(100);
	
	if (analogRead(TOUCH_Y_CHANNEL) > TOUCH_THRESHOLD) {
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

	p.y = read_filtered(TOUCH_X_CHANNEL);
	
	TOUCH_CLEAR_DDR();
	TOUCH_CLEAR_PORT();
	
	TOUCH_SET_PORT_TO_READ_Y();
	TOUCH_SET_DDR_TO_READ_Y(); //charge cap
	_delay_us(100);
	TOUCH_CLEAR_DDR();

	TOUCH_SET_DDR_TO_READ_X();
  	
	
	_delay_us(100);
	
	if (analogRead(TOUCH_Y_CHANNEL) > TOUCH_THRESHOLD) {
		p.x = -1;
		p.y = -1;
		return p;
	}
	return p;
}


calibration_values_t calibration_values;

pixel read_touch_screen_coordinates() {
	pixel p;
	p = read_touch_raw();

	if (p.x == -1) {
		return p;
	}

	if (p.x < calibration_values.xz) {
		p.x = 0;
	} else {
		p.x = ((uint32_t)(p.x-calibration_values.xz) * calibration_values.xg) / 1024;
	}

	if (p.y < calibration_values.yz) {
		p.y = 0;
	} else {
		p.y = ((uint32_t)(p.y-calibration_values.yz) * calibration_values.yg) / 1024;
	}
	return p;
}

