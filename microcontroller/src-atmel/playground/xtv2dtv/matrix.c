
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "util.h"
#include "ps2.h"
#include "scancodes.inc"


typedef struct{
	volatile uint8_t * port;
	uint8_t	bit;
}iopin;


#define DDR(p) ((volatile uint8_t *)p-1)
#define PIN(p) ((volatile uint8_t *)p-2)

#define PB(a) (pgm_read_byte(&(a)))
#define PW(a) (pgm_read_word(&(a)))

#define PIN_RESET PC5
#define PORT_RESET PORTC

iopin rows[8]={
	{&PORTA, 1},//1
	{&PORTA, 2},//2
	{&PORTA, 3},//3
	{&PORTA, 4},//4
	{&PORTB, 2},//16
	{&PORTB, 3},//17
	{&PORTB, 4},//18
	{&PORTB, 5},//19
};

iopin cols[16]={
	{&PORTA, 5},//5
	{&PORTA, 6},
	{&PORTA, 7},
	{&PORTE, 0},
	{&PORTC, 4},
	{&PORTC, 3},
	{&PORTC, 2},
	{&PORTC, 1},
	{&PORTC, 0},
	{&PORTB, 0},
	{&PORTB, 1},//15
	{&PORTB, 6},//20
	{&PORTB, 7},
	{&PORTD, 2},
	{&PORTD, 3},//23
	{&PORTA, 0},//0
};


/*
unsigned char keymatrix[8][16]=
	{	{'k','j',0,'ö',0,0,0,0,'l','s','f',0,'d','strg','a',0,},
		{',','m','rshift',0,'pause',0,0,0,'.','x','v','enter','c',0,'y',0,},
		{'8','7',0,'0','ende','scroll','num','print','9','2','4','f10','3','bildrunter','1',0,},
		{'i','u',0,'p',0,0,0,0,'o','w','r',0,'e',0,'q',0,},
		{'apostroph','6',0,'ß','pos1','einfg','entf',0,'f8','f1','5','f9','f2','bildrauf','^','fn',},
		{'*','z','lshift','ü',0,0,0,0,'f7','capslock','t','backspace','f3',0,'tab',0,},
		{'f6','h',0,'ä','rauf','>','#','alt',0,0,'g','f5','f4',0,'esc',0,},
		{'anykey','n',0,'-','links','rechts','runter',0,0,0,'b','space','altgr',0,0,0,},
	};
*/



unsigned char keymatrix[8][16] PROGMEM =
	{	{'K','J',0,40,0,0,0,0,'L','S','F',0,'D',25,'A',0,},
		{',','M',22,0,34,0,0,0,'.','X','V',10,'C',0,'Y',0,},
		{'8','7',0,'0',36,29,30,33,'9','2','4',20,'3',6,'1',0,},
		{'I','U',0,'P',0,0,0,0,'O','W','R',0,'E',0,'Q',0,},
		{58,'6',0,'?',35,37,38,0,18,11,'5',19,12,5,'^',26,},
		{'*','Z',21,41,0,0,0,0,17,28,'T',8,13,0,9,0,},
		{16,'H',0,39,1,'>','#',23,0,0,'G',15,14,0,27,0,},
		{7,'N',0,'-',3,4,2,0,0,0,'B',' ',24,0,0,0,},
	};


/*
up				1
down			2
left			3
right			4
bild rauf		5
bild runter		6
anykey			7
backspace		8
tab				9
enter			10

F1-F10			11-20

lshift			21
rshift			22
alt				23
altgr			24
strg			25
fn				26
esc				27
capslock		28
rollen			29
numlock			30
druck			33
pause			34
pos1			35
ende			36
einfg			37
entfernen		38
ä				39
ö				40
ü				41
apostroph		58
=				61

*/

#define PS2_KEY_AT 3

unsigned int scancodes[] PROGMEM =
{
	0,PS2_KEY_CRSR_UP,PS2_KEY_CRSR_DOWN,PS2_KEY_CRSR_LEFT,PS2_KEY_CRSR_RIGHT,0,0,0,PS2_KEY_BS,PS2_KEY_TAB,PS2_KEY_ENTER,PS2_KEY_F1,PS2_KEY_F2,PS2_KEY_F3,PS2_KEY_F4,PS2_KEY_F5,
	PS2_KEY_F6,PS2_KEY_F7,PS2_KEY_F8,0,0,PS2_KEY_LSHIFT,PS2_KEY_RSHIFT,0,0,PS2_KEY_RCTRL,0,PS2_KEY_ESC,0,0,0,0,
	PS2_KEY_SPACE,0,0,0,PS2_KEY_PAGE_DOWN,0,0,PS2_KEY_SEMICOLON,PS2_KEY_SEMICOLON|0x200,PS2_KEY_END,PS2_KEY_KP_STAR,0,PS2_KEY_COMMA,PS2_KEY_SLASH,PS2_KEY_PERIOD,0,
	PS2_KEY_0,PS2_KEY_1,PS2_KEY_2,PS2_KEY_3,PS2_KEY_4,PS2_KEY_5,PS2_KEY_6,PS2_KEY_7,PS2_KEY_8,PS2_KEY_9,PS2_KEY_KP_MINUS,0,0,PS2_KEY_EQUALS,PS2_KEY_LGUI,PS2_KEY_KP_PLUS,
	PS2_KEY_AT,PS2_KEY_A,PS2_KEY_B,PS2_KEY_C,PS2_KEY_D,PS2_KEY_E,PS2_KEY_F,PS2_KEY_G,PS2_KEY_H,PS2_KEY_I,PS2_KEY_J,PS2_KEY_K,PS2_KEY_L,PS2_KEY_M,PS2_KEY_N,PS2_KEY_O,
	PS2_KEY_P,PS2_KEY_Q,PS2_KEY_R,PS2_KEY_S,PS2_KEY_T,PS2_KEY_U,PS2_KEY_V,PS2_KEY_W,PS2_KEY_X,PS2_KEY_Z,PS2_KEY_Y,0,0,0,0,0,
};


unsigned int scancodes_shift[] PROGMEM =
{
	0,PS2_KEY_CRSR_UP,PS2_KEY_CRSR_DOWN,PS2_KEY_CRSR_LEFT,PS2_KEY_CRSR_RIGHT,0,0,0,PS2_KEY_BS,PS2_KEY_TAB,PS2_KEY_ENTER,PS2_KEY_F1,PS2_KEY_F2,PS2_KEY_F3,PS2_KEY_F4,PS2_KEY_F5,
	PS2_KEY_F6,PS2_KEY_F7,PS2_KEY_F8,0,0,PS2_KEY_LSHIFT,PS2_KEY_RSHIFT,0,0,PS2_KEY_RCTRL,0,PS2_KEY_ESC,0,0,0,0,
	PS2_KEY_SPACE,0,0,0,PS2_KEY_PAGE_DOWN,0,0,PS2_KEY_RBRACKET,PS2_KEY_LBRACKET,PS2_KEY_END,PS2_KEY_KP_STAR,0,PS2_KEY_COMMA,PS2_KEY_SLASH,PS2_KEY_PERIOD,0,
	PS2_KEY_EQUALS|0x200,PS2_KEY_1,PS2_KEY_APOSTROPHE,PS2_KEY_3,PS2_KEY_4,PS2_KEY_5,PS2_KEY_6,PS2_KEY_7,PS2_KEY_9,PS2_KEY_0,PS2_KEY_KP_MINUS,0,0,0,PS2_KEY_LGUI,PS2_KEY_KP_PLUS,
	PS2_KEY_AT,PS2_KEY_A,PS2_KEY_B,PS2_KEY_C,PS2_KEY_D,PS2_KEY_E,PS2_KEY_F,PS2_KEY_G,PS2_KEY_H,PS2_KEY_I,PS2_KEY_J,PS2_KEY_K,PS2_KEY_L,PS2_KEY_M,PS2_KEY_N,PS2_KEY_O,
	PS2_KEY_P,PS2_KEY_Q,PS2_KEY_R,PS2_KEY_S,PS2_KEY_T,PS2_KEY_U,PS2_KEY_V,PS2_KEY_W,PS2_KEY_X,PS2_KEY_Z,PS2_KEY_Y,0,0,0,0,0,
};


void keypress(uint8_t ascii){
	uint16_t scancode = PW(scancodes[ascii]);

	wait(20);
	key_make(scancode);
	wait(20);
	key_break(scancode);
}


void typestring(uint8_t * string){
	uint8_t c;
	while( (c = *string++) ){
		keypress(c);
	}
}

void typestring_P(uint8_t * string){
	uint8_t c;
	while( c = PB(*string++) ){
		keypress(c);
	}
}

#include "hypraload.c"

void pokeit(){
	uint16_t p=0;
	uint8_t buf[6], l=5, n;
	
	
	typestring_P(PSTR("POKE44,16\nNEW\n1 FORX="));
	utoa(0x801,buf,10);
	typestring(buf);
	typestring_P(PSTR("TO"));
	utoa(sizeof(programm)+0x801-1,buf,10);
	typestring(buf);
	typestring_P(PSTR("\n2 READA\n3 POKEX,A\n4 NEXT\n"));

	while(p<sizeof(programm)){
		utoa(l,buf,10);
		typestring(buf);
		typestring_P(PSTR(" DATA"));
		n = 0;
		while((n<8) && (p<sizeof(programm))){
			utoa(PB(programm[p]),buf,10);
			typestring(buf);
			if(n!=7)typestring_P(PSTR(","));
			p++;n++;
		}
		l++;
		typestring_P(PSTR("\n"));

	}
	typestring_P(PSTR("2006 POKE44,8\nRUN\n"));


}


int main(){
	uint8_t row, col;
	
	uint8_t shift=0;

	static uint16_t states[8];
	static uint16_t shift_state[8];


	init_ps2();
	sei();

	//DTV reset auslösen
	*DDR(&PORT_RESET) |= 1<<PIN_RESET;
	wait(40);
	*DDR(&PORT_RESET) &= ~(1<<PIN_RESET);

	//Auf die STRG Taste hauen, bis das DTV im cevi Modus ist
	uint8_t x;
	for(x=0;x<80;x++){
		key_make(PS2_KEY_LCTRL);
		wait(40);
	}
	
	key_break(PS2_KEY_LCTRL);



	//turn pullups on cols on
	for(col=0;col<16;col++){
		*(cols[col].port) |= (1<<cols[col].bit);
	}

	while(1){
		for(row=0;row<8;row++){
			uint16_t tmp1, tmp2;
		
			//Zeile elektrisch auf 0 setzen
			*DDR(rows[row].port) |= (1<<rows[row].bit);


			//alten Status holen
			tmp1 = states[row];
			tmp2 = 0;

			//Die Spalten abfragen und als Bitmaske in tmp2 speichern
			for(col=0;col<16;col++){
				uint8_t ascii;
				uint16_t scancode;

				tmp2 <<=1;
				if ( ! ( *PIN(cols[col].port) & (1<<cols[col].bit) ) ){
					tmp2 |= 1;
				}

				//Taste wurde zusätzlich gedrückt
				if( (tmp2 & 1) && (!(tmp1 & 0x8000)) ){
					ascii = PB(keymatrix[row][col]);
					if(ascii == 21){
						shift |= 1;
					}

					if(ascii == 22){
						shift |= 2;
					}

					if(shift){
						//merken, dass die taste aus der geshifteten keymap ist,
						//damit wir beim Loslassen den richtigen breakcode senden.
						shift_state[row] |= 1<<col;
						scancode = PW(scancodes_shift[ascii]);

						if(scancode & 0x200){
							if(shift & 1){
								key_break(PS2_KEY_LSHIFT);
							}else{
								key_break(PS2_KEY_RSHIFT);
							}
						}

					}else{
						shift_state[row] &= ~(1<<col);
						scancode = PW(scancodes[ascii]);

						if(scancode & 0x200){
							key_make(PS2_KEY_RSHIFT);
						}
					}

					if(ascii == 20){
						pokeit();
					}

					key_make(scancode);
				}

				//Taste wurde losgelassen
				if( (!(tmp2 & 1)) && (tmp1 & 0x8000) ){
					ascii = PB(keymatrix[row][col]);
					if(ascii == 21){
						shift &= ~1;
						//hackhack so shift doesn't get stuck for unknown reason
						if (shift == 0) key_break(PS2_KEY_RSHIFT);
					}
					if(ascii == 22){
						shift &= ~2;
					}

					if(shift_state[row] & (1<<col))
						scancode = PW(scancodes_shift[ascii]);
					else
						scancode = PW(scancodes[ascii]);

					key_break(scancode);

					if(scancode & 0x200){
						if(shift && 1){
							key_make(PS2_KEY_LSHIFT);
						}
						if(shift && 2){
							key_make(PS2_KEY_RSHIFT);
						}
						if(!shift){
							key_break(PS2_KEY_RSHIFT);
						}
					}
				}
		

				tmp1<<=1;
			}

			//Zeile loslassen
			*DDR(rows[row].port) &= ~(1<<rows[row].bit);


			states[row] = tmp2;

		}
	}
	
	
}
