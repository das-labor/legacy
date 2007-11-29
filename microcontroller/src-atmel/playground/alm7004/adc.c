
// wir sollten so ca. max. 50ms lang deintegrieren.
//ad Wandler ist an Port B.

//REF:
//150k 2.5V

//U:
//130.43kOhm  U*0.13043
//Ticks = ((U*0.13043)/130.43k) * 50000 / (2.5V/150k) 
//      = U/V *3000

//IC:
//I*2.585V/A 150k
//Ticks = I*2.585/2.5 * 50000 
//		= IC/A * 51700

//ID:
//Ticks = 2.485/2.5 * 50000
//		= ID/A * 49700

#include <avr/io.h>
#include <avr/interrupt.h>


//der adw pin wird über eine Drahtbrücke
//zusätzlich mit Pin 31 (PE0/ICP1) verbunden.
#define BIT_ADW 1
#define PORT_ADW PORTB
#define PIN_ADW PORTB

#define SET_MUX(v) PORTB = (PORTB & 0xC7) | (v)

#define MUX_FEEDBACK (5<<3)
#define MUX_U (1<<3)
#define MUX_IC (2<<3)
#define MUX_ID (3<<3)
#define MUX_REF (4<<3)

uint16_t werte[2];

uint8_t adc_update;

static uint8_t state;

uint8_t channels[] = {MUX_U, MUX_IC};
uint8_t channel_count, next_channel;


SIGNAL(SIG_INPUT_CAPTURE1){
	werte[channel_count] = ICR1;
	state = 0;
	TIMSK &= ~(1<<TICIE1);//IC int off again
	TCCR1A |= (1<<FOC1B);//froce output compare match
}


SIGNAL(SIG_OUTPUT_COMPARE1B){
	TCNT1 = 0;
	switch (state){
		case 0://reset(feedback)
			SET_MUX(MUX_FEEDBACK);
			OCR1B = 10000;
			channel_count ++;
			if(channel_count == 2){
				channel_count = 0;
				adc_update = 1;
			}
			next_channel = channels[channel_count];
			break;
		case 1://integrate
			SET_MUX(next_channel);
			OCR1B = 50000;
			break;
		case 2://deintegrate / measure
			SET_MUX(MUX_REF);
			OCR1B = 65535;
			TIFR = (1<<ICF1);
			TIMSK |= (1<<TICIE1);
			break;
		case 3://measure took to long - error
			TIMSK &= ~(1<<TICIE1);
			OCR1B = 1000;
			state = -1;
			werte[channel_count] = 0;
			break;
	}
	state++;
	
}


void adc_init(){
	DDRB |= 0x38;				//Mux auf Ausgang
	PIN_ADW |= (1<<BIT_ADW);	//Pullup an ADW an
	PORTE |= (1<<PE0);//2er pullup an adw an	
	SET_MUX(MUX_FEEDBACK);
	
	TCCR1B = (1<<WGM12) | 2 ; //CTC Mode, clk/8
	TCNT1 = 0;
	OCR1B = 10000;
	TIFR = (1<<OCF1B);
	TIMSK |= (1<<OCIE1B);
}
