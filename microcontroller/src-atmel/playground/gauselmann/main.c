
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ps2.h"
#include "scancodes.inc"

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
#define KEY_1P_B3		PS2_KEY_ENTER
#define KEY_2P_START	PS2_KEY_2
#define KEY_2P_UP		PS2_KEY_W
#define KEY_2P_DOWN		PS2_KEY_S
#define KEY_2P_LEFT		PS2_KEY_A
#define KEY_2P_RIGHT	PS2_KEY_D
#define KEY_2P_B1		PS2_KEY_Y
#define KEY_2P_B2		PS2_KEY_X
#define KEY_2P_B3		PS2_KEY_C


typedef struct{
	volatile uint8_t *port;
	uint8_t mask;
	uint8_t state;
	uint16_t key;
}keymap_t;

keymap_t keymappings[] = {
	{&PINB, (1<<1), 0, KEY_1P_B2},
	{&PINB, (1<<2), 0, KEY_1P_START},
	{&PINB, (1<<3), 0, KEY_2P_LEFT},
	{&PINB, (1<<4), 0, KEY_2P_DOWN},
	{&PINB, (1<<5), 0, KEY_2P_RIGHT},
	{&PINB, (1<<6), 0, KEY_2P_UP},
	{&PINB, (1<<7), 0, KEY_1P_RIGHT},
	{&PINC, (1<<0), 0, KEY_1P_B1},
	{&PINC, (1<<1), 0, KEY_2P_START},
	{&PINC, (1<<2), 0, KEY_2P_B1},
	{&PINC, (1<<3), 0, KEY_2P_B2},
	{&PINC, (1<<4), 0, KEY_1P_B3},
	{&PINC, (1<<5), 0, KEY_2P_B3},
	{&PIND, (1<<4), 0, KEY_1P_UP},
	{&PIND, (1<<5), 0, KEY_1P_DOWN},
	{&PIND, (1<<6), 0, KEY_1P_LEFT},
};


int main(){
	
	UCSRB = 0;
	
	//pullups on
	PORTB = 0xfe;
	PORTC = 0x3f;
	PORTD = 0x70;
	
	DDR_SYNC_ENABLE |= (1<<BIT_SYNC_ENABLE);
	PORT_SYNC_ENABLE |= (1<<BIT_SYNC_ENABLE);
	
	DDR_LED |= (1<<BIT_LED);
	
	init_ps2();
	
	sei();
	
	volatile unsigned int delay;
	for(delay=0;delay<1000;delay++);
	
	ps2_put(1, (uint8_t[]) {0xaa} );
	
	
	
	while(1){
		
		
		unsigned char data;
		if(ps2_get_nb(&data)){
			ps2_put(1, (uint8_t[]) {0xfa} );
			//char buf[10];
			//itoa(data, buf, 16);
			//uart_putstr(buf);
		}
		
		uint8_t x;
		for(x=0; x<(sizeof(keymappings)/sizeof(keymap_t)); x++){
			uint8_t aktstate;
			keymap_t *km = &keymappings[x];
			aktstate = (~(*km->port)) & km->mask;
			if(aktstate != km->state){
				km->state = aktstate;
				if(aktstate){
					LED_ON();
					key_make_b(km->key);
					LED_OFF();
				}else{
					LED_ON();
					key_break_b(km->key);
					LED_OFF();

				}
			}
		}
	}
}

