//*****************************************************************************
//
// File Name		: 'menu.c'
// Title			: LCD menu
// Author			: Patrick Rudolph mailto:siro@das-labor.org
// Created			: 2011-05-08
// Revised			: 
// Version			: 0.1
// Target MCU	: Atmel AVR series
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "kb.h"	//if you don't use PS2 keyboard comment this line
#include "ks0108.h"	//if you don't use ks0108 comment this line 

#include "arial8.h"
#include "menu.h"

void menu_update(void)
{
	uint8_t add_val=0;
#ifdef __KB_INCLUDED
	char kbchar=0;		//buffer holding the key pressed
	kbchar=getcharx();		//get char from keyboard
	if(!kbchar)		//if 0 is return no key was pressed
		return;	//return, nothing to do
#endif

	//if button UP pressed
	if (BUTTON_UP)
	{
		menu_key_i=0;
		if(menu_ycursor < menu_entry_cnt)
			menu_ycursor ++;
		menu_draw();
	}
	//if Button DOWN pressed
	if (BUTTON_DOWN)
	{
		menu_key_i=0;
		if(menu_ycursor > 0)
			menu_ycursor --;
		menu_draw();
	}
	//If Button RIGHT pressed, enter submenu
	if (BUTTON_RIGHT)
	{
		if(menu_key_i != 0x01)
			menu_key_t=0;
		
		menu_key_i=0x01;
		if(menu_key_t < 255)
			menu_key_t++;
		
		add_val=(menu_key_t>>3)+1;
		
		if(menu_val[menu_ycursor].size == 1)
			(*((uint8_t *)menu_val[menu_ycursor].p))+=add_val;
		if(menu_val[menu_ycursor].size == 2)
			(*((uint16_t *)menu_val[menu_ycursor].p))+=add_val;
		if(menu_val[menu_ycursor].size == 4)
			(*((uint32_t *)menu_val[menu_ycursor].p))+=add_val;
		menu_draw();
	}
	//If button LEFT pressed, leave submenu
	if (BUTTON_LEFT)
	{
		if(menu_key_i != 0x02)
			menu_key_t=0;
		
		menu_key_i=0x02;
		if(menu_key_t < 255)
			menu_key_t++;
		
		add_val=(menu_key_t>>3)+1;

		if(menu_val[menu_ycursor].size == 1)
			(*((uint8_t *)menu_val[menu_ycursor].p))-=add_val;
		if(menu_val[menu_ycursor].size == 2)
			(*((uint16_t *)menu_val[menu_ycursor].p))-=add_val;
		if(menu_val[menu_ycursor].size == 4)
			(*((uint32_t *)menu_val[menu_ycursor].p))-=add_val;
		menu_draw();
	}	
#ifdef __KB_INCLUDED
	if (BUTTON_ENTER)
	{
		if(menu_key_i != 0x03)
		{
			menu_key_t=0;
			menu_key_i=0x03;
		}
		
		menu_draw();
	}	
	if (BUTTON_ISNUMMERIC)
	{
		if(menu_key_i==0x03)
		{	
			//not sure if this is working
			if(menu_val[menu_ycursor].size == 1)
				(*((uint8_t *)menu_val[menu_ycursor].p))=((*((uint8_t *)menu_val[menu_ycursor].p))%100)*10+(uint8_t)(kbchar - '0');
			if(menu_val[menu_ycursor].size == 2)
				(*((uint16_t *)menu_val[menu_ycursor].p))=((*((uint16_t *)menu_val[menu_ycursor].p))%10000)*10+(uint8_t)(kbchar - '0');
			if(menu_val[menu_ycursor].size == 4)
				(*((uint32_t *)menu_val[menu_ycursor].p))=((*((uint32_t *)menu_val[menu_ycursor].p))%1000000000)*10+(uint8_t)(kbchar - '0');
		}
	}
#endif
}


void menu_draw(void)
{
	uint8_t i=0;		//help var for menu drawing
	uint8_t k=0;		//used in for-loop
	char buf[11];		//values goes here
	
	if(menu_entry_cnt - menu_ycursor <= menu_LCD_MAXLINES)	//if ycursor out of range
	{
		i=menu_entry_cnt - menu_LCD_MAXLINES;		//start at line i = maxentries - menu_LCD_MAXLINES
	}
	else
	{
		i=menu_ycursor;		//start at ycursor
	}

#ifdef KS0108_H
	ks0108ClearScreen();							// display clear
	ks0108GotoXY(0,0);  // Set a position
	ks0108Puts_P(PSTR("menu\n"));
	
	for(k=0;k<menu_LCD_MAXLINES;k++)
	{
		if((k+i)==menu_ycursor)
			ks0108Puts_P(PSTR(">"));
		else
			ks0108Puts_P(PSTR(" "));
		
  		ks0108Puts_P(menu_val[k+i].str);  // Print some text
		ks0108Puts_P(PSTR("\t"));
		
		if(menu_val[menu_ycursor].size == 1)
			utoa(*((uint8_t *)menu_val[k+i].p),buf,10);
		
		if(menu_val[menu_ycursor].size == 2)
			utoa(*((uint16_t *)menu_val[k+i].p),buf,10);
		
		if(menu_val[menu_ycursor].size == 4)
			ltoa(*((uint32_t *)menu_val[k+i].p),buf,10);
		ks0108GotoX(LCD_WIDTH - ks0108StringWidth(buf));
		ks0108Puts(buf);
		ks0108Puts_P(PSTR("\n"));
	}
#else
	#warning menu_draw(): only ks0108 display controller is implemented yet
#endif
}

void menu_init()
{         
#ifdef KS0108_H   
	ks0108Init(0);
	//activate ks0108 controller
	ks0108SelectFont(arial8, ks0108ReadFontData, BLACK);  //select Display Font
	menu_LCD_MAXLINES = ((LCD_HEIGHT/ks0108FontHeight()))-1;	//max text lines that can be displayed
	ks0108SetTransparency(OPAQUE);  //set font opaque
  	ks0108GotoXY(0,0);  // Set a position
  	ks0108Puts_P(PSTR("Laserprojector by siro 2011\npress power button"));  // Print some text
#else
	#warning menu_init(): only ks0108 display controller is implemented yet
#endif
}

void menu_init_entry(void *p,uint8_t size, const char * menu_str)
{
	if(menu_entry_cnt < MENU_MAXENTRIES)
	{
		menu_val[menu_entry_cnt].str = menu_str;
		menu_val[menu_entry_cnt].p = p;
		menu_val[menu_entry_cnt].size = size;
		menu_entry_cnt++;
	}
}

/*
not implemented yet
void menu_hide(void)
{


}

void menu_show(void)
{


}
*/


