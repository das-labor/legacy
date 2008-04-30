#ifndef UI_PRIMITIVES_H_
#define UI_PRIMITIVES_H_

#include <stdint.h>
#include <avr/pgmspace.h>
#include "rtc.h"

/* we use a integer instead of void* 'cause the compiler keepr rumoring about 
 * data_ptr <-> func_ptr conversation 
 */
typedef uint16_t superp;

typedef struct menu_st{
	PGM_P name;                 /* 2 byte */
	enum {none, 
		  autosubmenu,
		  submenu, 
		  execute, 
		  back, 
		  terminator} options;  /* 1 byte*/
	superp x;
//	void* x;
//	union { 
//		void (*fkt)(void);
//		void* submenu; } x;      /* 2 byte */
} menu_t;


#define LCD_LABORLOGO     7
#define LCD_RADIOSELECT   5
#define LCD_CHECKSELECT   6
#define LCD_RADIONOSELECT 'o'
#define LCD_CHECKNOSELECT 0xDB

#define LCD_FULLCHAR  255
#define LCD_EMPTYCHAR  ' '
#define LCD_RARROW 0x7E
#define LCD_LARROW 0x7F

#define NO_KEY     ' '
#define ENTER_KEY  'F'
#define SELECT_KEY 'E'
#define UP_KEY     '0'
#define DOWN_KEY   'C'
#define CORRECT_KEY 'C'

extern char ui_statusstring[6];

void    ui_primitives_init(void);
void    ui_progressbar(double percent, uint8_t xpos, uint8_t ypos, uint8_t width);
uint8_t ui_radioselect(const char* opts);
uint8_t ui_radioselect_P(PGM_P opts);
void    ui_checkselect(const char* opts, uint8_t* config);
void    ui_checkselect_P(PGM_P opts, uint8_t* config);
void    ui_menuexec(menu_t* menu);
void    ui_drawframe(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char framechar);
void    ui_hexdump(const void* data, uint16_t length);
void    ui_hexdump_P(PGM_VOID_P data, uint16_t length);
void    ui_textwindow(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, char* text);
void    ui_textwindow_P(uint8_t posx, uint8_t posy, uint8_t width, uint8_t height, PGM_P text);

char    ui_waitforkeypress(void);
char    ui_waitforkeypresstimed(timestamp_t* tdiff);
void    ui_waitforkey(char key);

uint8_t read_decimaln(uint8_t xpos, uint8_t ypos, char* str, uint8_t n);
uint8_t read_hexn(uint8_t xpos, uint8_t ypos, char* str, uint8_t n);
uint8_t read_strn(uint8_t xpos, uint8_t ypos, PGM_P charset,char * str, uint8_t n);

void ui_printstatusline(void);

#endif /*UI_PRIMITIVES_H_*/
