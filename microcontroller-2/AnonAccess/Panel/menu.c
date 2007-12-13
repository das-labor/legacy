#include "config.h"
#include "lcd_tools.h"
#include "uart.h"
#include "menu.h"
#include "keypad.h"
#include <stdint.h>
#include <avr/delay.h>

typedef struct{
	char *name;
	enum {submenu, execute} options;
	void (*fkt)(void*, ...);
}menu_t;

char* status_string = "";

void open_door(void* data){}
void lock_door(void* data){}
void admin_menu(void* data){}
void stat_menu(void* data){}
void bootstrap_menu(void* data){}
void debug_menu(void* data){}

menu_t main_menu[6] = {
	{"open door",execute, open_door},
	{"lock door",execute, lock_door},
	{"admin menu",submenu, admin_menu},
	{"statistic menu",submenu, stat_menu},
	{"bootstrap menu",submenu, bootstrap_menu},
	{"debug menu",submenu, debug_menu}
};



/*
untermenu allgemein[] ={
	{"test", cmdtest},
	{"muh", cmdtest}
};

untermenu uadmin[] ={
	{"add user", cmdtest},
	{"rem user", cmdtest},
	{"lock user", cmdtest},
	{"unlock user", cmdtest},
};

untermenu cardopt[] ={
	{"react card", cmdtest},
	{"del card", cmdtest}
};

untermenu adminmenu[] ={
	{"add admin", cmdtest},
	{"rem admin", cmdtest}
};

menu haupt[] ={
	{"Allgemein", NULL, &(allgemein[0])},
	{"User Admin", NULL, &(uadmin[0])},
	{"Karten Optionen", NULL, &(cardopt[0])},
	{"Admin Menu", NULL, &(adminmenu[0])}
};
*/

void menu_handler(char akey){

	
	
	switch (akey)
	{	
		case 0x41: 
		{						//A - next
			
			break;
		}
		case 0x42:
		{						//B - back
		
			break;
		}
		case 0x43:
		{						//C - Cancel
		
			break;
		}
		case 0x45:
		{						//E - Enter
				
			break;
		}
		case 0x46:
		{						//F - Menu Entry
	
			break;
		}
	}
	
	//Debug
//	lcd_gotopos(3,5);
//	lcd_writetext("Hello World!\x7E\x7F ");
	
}

void menueakt(int a){
}

#define R_ARROW 0x7E
#define L_ARROW 0x7F

void menuexec(uint8_t n, menu_t* menu){
  reset:
  	;
	uint8_t i,idx=0,selpos=2;
  redraw:
	lcd_cls();
	lcd_gotopos(1,1);
	lcd_writetext("=AnonAccess=");
	lcd_writetext(status_string);
	
	for(i=0; i<((n<3)?n:3); ++i){
		lcd_gotopos(i+2,2);
		lcd_writetext(menu[(idx+i)%n].name);
	}
	lcd_gotopos(selpos,1);
	lcd_writechar(R_ARROW);
		
  rescan:	
	switch (read_keypad()){
		case '0': 
			if(selpos==2){
				idx = (n+idx-1)%n;
			} else {
				selpos--;
			}
			_delay_ms(1);
			while(read_keypad()=='0')
				;
			_delay_ms(1);
			goto redraw;
			break;		
		case 'C':
			if(selpos==4){
				idx = (n+idx+1)%n;
			} else {
				selpos++;
			}
			_delay_ms(1);
			while(read_keypad()=='C')
				;
			_delay_ms(1);
			goto redraw;
			break;		
		case 'E':
			_delay_ms(1);
			while(read_keypad()=='E')
				;
			_delay_ms(1);
			goto reset;
			break;
		default: goto rescan; 
			break;
	}
}

void master_menu(void){
	menuexec(6, main_menu);
}






