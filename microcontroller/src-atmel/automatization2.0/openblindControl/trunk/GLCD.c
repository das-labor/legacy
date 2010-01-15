#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>

#include "GLCD.h"
#include "ks0108.h"
#include "arial8.h"

void led_backlight(bool led) {
	if (led) { PORTD |= (1<<PD6); }
	else { PORTD &= ~(1<<PD6); }
}

int main(void) {

	//PortD.6 used as LCD backlight switch
	DDRD |= (1<<PD6);
	led_backlight(true);	

	//initialize LCD
	ks0108Init(0);

	blindBarType blindBar[14]; //the blinds x_X
	blindBarType foo; //short version for each blindBar instance

	for (int i=0; i<14; i++) {
		foo = blindBar[i];
		foo.ID = i;
		foo.barWidth = 5;
		foo.barHeight = 40;
		foo.visible = true;
		foo.scale = false;
		foo.locked = false;
		foo.barPos = (rand() % foo.barHeight + 1);
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
		
		blindBar[i] = foo;
	}

	for (int i=0; i<14; i++) {
			foo = blindBar[i];
			foo.barPos = (rand() % foo.barHeight + 1);
			blindBar[i] = foo;
		}
	
	//test text
	ks0108SelectFont(arial8, ks0108ReadFontData, BLACK);
	ks0108GotoXY(5, 4);
	//ks0108Puts(textFoo);
	ks0108Puts_P(PSTR("OpenBlind.Control - positions"));
	//frame
	ks0108DrawRoundRect(0, 0, 127, 13, 5, BLACK);
	ks0108DrawRoundRect(0, 13, 127, 50, 5, BLACK);
	ks0108DrawLine(69, 13, 69, 63, BLACK);
	ks0108DrawLine(98, 13, 98, 63, BLACK);
	
	blindBar[0].visible = true;
	blindBar[0].scale = true;
	//ks0108SetInverted(1);
	
	while(1) {		
		
		led_backlight(false);
		_delay_ms(blindBar[0].barPos*10);
		led_backlight(true);

		//draw position bar
		for (int i=0; i<14; i++) {
			foo = blindBar[i];
			if (foo.visible) {
				ks0108FillRect(foo.xpos+1, foo.ypos+1, foo.barWidth-2, foo.barPos-2, BLACK); //visible position
				ks0108FillRect(foo.xpos+1, foo.ypos+foo.barPos+1, foo.barWidth-2, foo.barHeight-(foo.barPos+1), WHITE); //invisible opposite
				ks0108DrawRect(foo.xpos, foo.ypos, foo.barWidth, foo.barHeight, BLACK); //frame
				
				//draw scale if enabled
				if (foo.scale) {
					ks0108SetDot(foo.xpos+1, foo.ypos+(foo.barHeight/2), BLACK);
					ks0108SetDot(foo.xpos+(foo.barWidth-1), foo.ypos+(foo.barHeight/2), WHITE);
					ks0108SetDot(foo.xpos+1, foo.ypos+(foo.barHeight/4), BLACK);
					ks0108SetDot(foo.xpos+(foo.barWidth-1), foo.ypos+(foo.barHeight/4), WHITE);
					ks0108SetDot(foo.xpos+1, foo.ypos+(foo.barHeight/4*3), BLACK);
					ks0108SetDot(foo.xpos+(foo.barWidth-1), foo.ypos+(foo.barHeight/4*3), WHITE);
				}
				
			}
			foo.barPos = (rand() % foo.barHeight + 1);
			blindBar[i] = foo;
		}

		_delay_ms(1000);
	}		
}
