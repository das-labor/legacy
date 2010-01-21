#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

#include "GLCD.h"
#include "ks0108.h"
#include "arial8.h"

#define CLOCK PB0
#define DATA PB1
#define RESET PB2

void led_backlight(bool led) {
	if (led) { PORTD |= (1<<PD6); }
	else { PORTD &= ~(1<<PD6); }
}

void constrain_u8 (uint8_t *ptr, uint8_t min, uint8_t max) {
	uint8_t temp;
	temp = *ptr;
	if (temp < min) temp = min;
	if (temp > max) temp = max;
	*ptr = temp;
}

void blindCalcPos(blindBarType *myBlind) {
	blindBarType temp;
	temp = *myBlind;
	temp.barPos = 1+((((temp.realPos * 100) / temp.realHeight) * temp.barHeight) / 100);
	*myBlind = temp;
}

void drawBlindBar(blindBarType *myBlind) {
	blindBarType temp;
	temp = *myBlind;
	ks0108FillRect(temp.xpos+1, temp.ypos+1, temp.barWidth-2, temp.barPos-2, BLACK); //visible position
	ks0108FillRect(temp.xpos+1, temp.ypos+temp.barPos+1, temp.barWidth-2, temp.barHeight-(temp.barPos+1), WHITE); //invisible opposite
	ks0108DrawRect(temp.xpos, temp.ypos, temp.barWidth, temp.barHeight, BLACK); //frame
	*myBlind = temp;
}

//main
int main(void) {
	_delay_ms(10);

	//Shift register test ... delete for normal usage!!!
	DDRB |= (1<<CLOCK);
	DDRB |= (1<<DATA);
	DDRB |= (1<<RESET);

	PORTB &= ~(1<<CLOCK);
	PORTB &= ~(1<<DATA);
	PORTB &= ~(1<<RESET);
	_delay_ms(1);
	PORTB|= (1<<RESET);



	//PortD.6 used as LCD backlight switch
	DDRD |= (1<<PD6);
	led_backlight(false);	



	PORTB |= (1<<DATA);
	_delay_ms(1);
	PORTB |= (1<<CLOCK);
	_delay_ms(1);
	PORTB &= ~(1<<DATA);
	PORTB &= ~(1<<CLOCK);

	for(int i=0; i<15; i++) {
		_delay_ms(200);
		//PORTB |= (1<<DATA);
		_delay_ms(1);
		PORTB &= ~(1<<CLOCK);
		_delay_ms(200);
		//PORTB |= (1<<DATA);
		_delay_ms(1);
		PORTB |= (1<<CLOCK);
	}
	



	/*
	//initialize LCD
	ks0108Init(0);

	//declaration of control elements
	roomType room[3]; //the rooms
	roomType habitat;//short version for each room instance
	blindBarType blindBar[14]; //the blinds x_X
	blindBarType foo; //short version for each blindBar instance
	
	for (int i=0; i<3; i++) {
		room[i].ID = i;
		room[i].posMax = 255;
		room[i].blindsPos = -1;
		room[i].selected = false;
	}
	room[0].blindsCount = 8;
	room[1].blindsCount = 3;
	room[2].blindsCount = 3;

	for (int i=0; i<14; i++) {
		foo = blindBar[i];

		foo.ID = i;
		foo.barWidth = 5;
		foo.barHeight = 40;
		foo.realHeight = 255;
		//foo.realPos = (rand() % foo.realHeight);
		foo.visible = true;
		foo.locked = false;
		foo.dir = 0;
		
		if (foo.ID >= 0) {
			foo.room = blue;
		}
		if (foo.ID > 7) {
			foo.room = yellow;
		}
		if (foo.ID > 10) {
			foo.room = red;
		}

		//set initial position
		foo.xpos = 4+((3+foo.barWidth)*foo.ID)+(foo.barWidth*(foo.room-1));
		foo.ypos = 18;
		blindCalcPos(&foo);

		blindBar[i] = foo;
	}

	//test text
	ks0108SelectFont(arial8, ks0108ReadFontData, BLACK);
	ks0108GotoXY(5, 4);
	//ks0108Puts(textFoo);
	ks0108Puts_P(PSTR("OpenBlind.Control - random"));
	
	//frame
	ks0108DrawRoundRect(0, 0, 127, 13, 5, BLACK);
	ks0108DrawRoundRect(0, 13, 69, 50, 5, BLACK);
	ks0108DrawRoundRect(69, 13, 29, 50, 5, BLACK);
	ks0108DrawRoundRect(98, 13, 29, 50, 5, BLACK);
	//ks0108DrawRoundRect(99, 14, 27, 48, 3, BLACK);
	
	room[0].selected = true;
	room[0].blindsPos = 200;
	room[1].selected = true;
	room[1].blindsPos = -1;
	room[2].selected = false;
	room[2].blindsPos = 128;

	while(1) {		

		for (int r=0; r<3; r++) {
			habitat = room[r];
			//draw position bar	
			for (int i=0; i<14; i++) {
				
				foo = blindBar[i];
				foo.realPos = (rand() % foo.realHeight);
				blindCalcPos(&foo);
				
				if(habitat.selected && habitat.ID==0) {
					ks0108DrawRoundRect(1, 14, 67, 48, 3, BLACK);
					if(habitat.blindsPos>0) foo.realPos = habitat.blindsPos;
				}
				if(habitat.selected && habitat.ID==1) {
					ks0108DrawRoundRect(70, 14, 27, 48, 3, BLACK);
					if(habitat.blindsPos>0) foo.realPos = habitat.blindsPos;
				}
				if(habitat.selected && habitat.ID==2) {
					ks0108DrawRoundRect(99, 14, 27, 48, 3, BLACK);
					if(habitat.blindsPos>0) foo.realPos = habitat.blindsPos;
				}
				
				blindCalcPos(&foo);
				if (foo.visible && foo.room-1 == habitat.ID) drawBlindBar(&foo);	

				blindBar[i] = foo;
			}
		}
	}
	*/
}
