//*****************************************************************************
//
// File Name		: 'menu.h'
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

#include <avr/pgmspace.h>
#include <inttypes.h>

#include "config.h"

#define MENU_MAXENTRIES 12

#ifndef MENU_H
#define MENU_H

#define MENU_READANDWRITE 1
#define MENU_READONLY 0

volatile static uint8_t menu_ycursor=0;  //y cursor
volatile static uint8_t menu_LCD_MAXLINES=1;
volatile static uint8_t menu_entry_cnt=0;
volatile static uint8_t menu_key_i=0;
volatile static uint8_t menu_key_t=0;

typedef struct
{
	void *p;
	uint8_t size;
	const char * str;
}pointer_size_t;

//link to values, define your values here
static pointer_size_t menu_val[MENU_MAXENTRIES];

//function prototypes
void menu_init(void);
static void menu_draw(uint8_t cls);
void menu_update(void);
void menu_init_entry(void *p,uint8_t size, const char * menu_str, uint8_t iswriteable);

/* example programm:
	in file config.h:
		#define MENU_MAXENTRIES 10

	in file main.c:
		 int main(void){
			 menu_init_entry(&testvar1,sizeof(testvar1),PSTR("testvar1"));
			 menu_init_entry(&testvar2,sizeof(testvar2),PSTR("testvar2"));
			 menu_init();	//init the GLCD and menu

			 while(1){
				menu_draw();	//call multiple times per second
				_delay_ms(10);
			 }
		 }
 */
#endif