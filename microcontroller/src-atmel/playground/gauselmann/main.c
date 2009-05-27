
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include "ps2.h"
#include "scancodes.inc"
#include "uart.h"

/*
	PB0 SYNC_SENSE
	PB1 1P_B2
	PB2 1P_START
	PB3 2P_LEFT
	PB4 2P_DOWN
	PB5 2P_RIGHT
	PB6 2P_UP
	PB7 1P_RIGHT

	PC0 1P_B1
	PC1 2P_START
	PC2 2P_B1
	PC3 2P_B2
	PC4 1P_B3
	PC5 2P_B3

	PD0 RXD
	PD1 TXD/LED
	PD2 CLK
	PD3 DATA
	PD4 1P_UP
	PD5 1P_DOWN
	PD6 1P_LEFT
	PD7 SYNC_ENABLE
*/

#define PORT_SYNC_ENABLE PORTD
#define DDR_SYNC_ENABLE DDRD
#define BIT_SYNC_ENABLE PD7

#define SYNC_OFF() PORT_SYNC_ENABLE &= ~(1<<BIT_SYNC_ENABLE)
#define SYNC_ON() PORT_SYNC_ENABLE |= (1<<BIT_SYNC_ENABLE)


#define PORT_LED PORTD
#define DDR_LED DDRD
#define BIT_LED PD1

#define LED_ON()  PORT_LED |= (1<<BIT_LED)
#define LED_OFF() PORT_LED &= ~(1<<BIT_LED)


#define KEY_1P_START	PS2_KEY_1
#define KEY_1P_UP		PS2_KEY_CRSR_UP
#define KEY_1P_DOWN		PS2_KEY_CRSR_DOWN
#define KEY_1P_LEFT		PS2_KEY_CRSR_LEFT
#define KEY_1P_RIGHT	PS2_KEY_CRSR_RIGHT
#define KEY_1P_B1		PS2_KEY_LCTRL
#define KEY_1P_B2		PS2_KEY_LALT
#define KEY_1P_B3		PS2_KEY_SPACE

#define KEY_2P_START	PS2_KEY_2
#define KEY_2P_UP		PS2_KEY_R
#define KEY_2P_DOWN		PS2_KEY_F
#define KEY_2P_LEFT		PS2_KEY_D
#define KEY_2P_RIGHT	PS2_KEY_G
#define KEY_2P_B1		PS2_KEY_A
#define KEY_2P_B2		PS2_KEY_S
#define KEY_2P_B3		PS2_KEY_Q

#define KEY_COIN_SLOT1	PS2_KEY_5
#define KEY_COIN_SLOT2	PS2_KEY_6
#define KEY_ESCAPE		PS2_KEY_ESC


#define DEBOUNCE_DELAY 255

typedef struct{
	volatile uint8_t *port;
	uint8_t mask;
	uint8_t state;
	uint16_t release_key;	//key to be released(for key combinations)
	uint16_t key;
	uint8_t timer;
}keymap_t;


//the first two keynums are offsets into keymappings table
typedef struct{
	uint8_t modifier_keynum;
	uint8_t action_keynum;
	uint16_t result_key;
}keycomb_t;


keymap_t keymappings[] = {
	{&PINB, (1<<2), 0, 0, KEY_1P_START},//0
	{&PIND, (1<<4), 0, 0, KEY_1P_UP},	//1
	{&PIND, (1<<5), 0, 0, KEY_1P_DOWN},	//2
	{&PIND, (1<<6), 0, 0, KEY_1P_LEFT},	//3
	{&PINB, (1<<7), 0, 0, KEY_1P_RIGHT},//4
	{&PINC, (1<<0), 0, 0, KEY_1P_B1},	//5
	{&PINB, (1<<1), 0, 0, KEY_1P_B2},	//6
	{&PINC, (1<<4), 0, 0, KEY_1P_B3},	//7
	{&PINC, (1<<1), 0, 0, KEY_2P_START},//8
	{&PINB, (1<<6), 0, 0, KEY_2P_UP},	//9
	{&PINB, (1<<4), 0, 0, KEY_2P_DOWN},	//10
	{&PINB, (1<<3), 0, 0, KEY_2P_LEFT},	//11
	{&PINB, (1<<5), 0, 0, KEY_2P_RIGHT},//12
	{&PINC, (1<<2), 0, 0, KEY_2P_B1},	//13
	{&PINC, (1<<3), 0, 0, KEY_2P_B2},	//14
	{&PINC, (1<<5), 0, 0, KEY_2P_B3},	//15
};

keycomb_t keycombinations[] = {
	{1, 0, KEY_COIN_SLOT1},
	{1, 8, KEY_COIN_SLOT2},
	{0, 8, KEY_ESCAPE},
};


void wait(uint16_t ms){
	for(;ms>0;ms--){
		_delay_ms(1);
	}
}

volatile uint16_t line_time;

ISR(TIMER1_CAPT_vect){
	static uint16_t last_capture, this_capture;
	uint16_t line_t;
	this_capture = ICR1;
	line_t = this_capture-last_capture;
	line_time = line_t;
	last_capture = this_capture;
	
	if( (line_time > 520) || (line_time < 504) ){
		LED_ON();
		SYNC_OFF();
	}

}

void init_timer1(){
	//input capture noise canceler, falling edge
	TCCR1B = (1<<ICNC1) | (0<<ICES1) | 1;
	
	TIMSK |= (1<<TICIE1);
}

int main(){
	
	UCSRB = 0;
	
	//pullups on
	PORTB = 0xfe;
	PORTC = 0x3f;
	PORTD = 0x70;
	
	DDR_SYNC_ENABLE |= (1<<BIT_SYNC_ENABLE);
	DDR_LED |= (1<<BIT_LED);
	
	init_timer1();
	init_ps2();
	//uart_init();
	
	
	sei();

	wait(500);

	key_make(0xaa); //bat passed
	
	while(1){
		
		cli();
		if( (line_time <= 520) && (line_time >= 504) ){
			SYNC_ON();
			LED_OFF();
		}
		sei();
	

//		char buf[10];
//		itoa(line_time, buf, 10);
//		uart_putstr(buf);
//		uart_putstr("\r\n");

		
		
		unsigned char data;
		if(ps2_get_nb(&data)){
			//static uint8_t id = 0xab;
			switch(data){
				case 0xee: //echo
					key_make(0xee);
					break;
				case 0xff: //reset
					wait(500);
					key_make(0xaa);
					break;
				case 0xf2:  //id
					key_make(0xfa);
					key_make(0xab);
					key_make(0x83);
					//id ^= (0xab ^ 0x83);
					break;
				default: //just acknowledge anything else the keyboard might be telling us...
					key_make(0xfa);
					break;
			
			}
				
			//char buf[10];
			//itoa(data, buf, 16);
			//uart_putstr(buf);
			//uart_putstr("\r\n");
		}
		
		uint8_t x, y;
		for(x=0; x<(sizeof(keymappings)/sizeof(keymap_t)); x++){
			uint8_t aktstate;
			keymap_t *km = &keymappings[x];
			
			if(km->timer > 0){
				km->timer --;
			}else{
				aktstate = ((*km->port) & km->mask)?0:1;
				if( (!!aktstate) == (! km->state) ){
					km->timer = DEBOUNCE_DELAY;
					if(aktstate){
						uint8_t comb_taken = 0;
						
						LED_ON();
						for(y=0; y<(sizeof(keycombinations)/sizeof(keycomb_t)); y++){
							if(	(keycombinations[y].action_keynum == x) && 
							   (keymappings[keycombinations[y].modifier_keynum].state) ){
								comb_taken = 1;
								key_make_b(keycombinations[y].result_key);
								km->state = 2; //remember keypress triggered combination for release
								km->release_key = keycombinations[y].result_key;
							}
						}
						if(!comb_taken){
							key_make_b(km->key);
							km->state = 1;
						}
						LED_OFF();
					}else{
						LED_ON();
							
						if(km->state != 2){
							key_break_b(km->key);
						}else{
							key_break_b(km->release_key);
						}
						LED_OFF();
						km->state = 0;
					}
				}
			}
		}
	}
}

