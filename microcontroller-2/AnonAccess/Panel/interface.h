#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <stdint.h>
#include "rtc.h"

extern char status_string[6];

#define CHAR_SWITCH_DELAY 1000 /* 1 sec */

typedef struct{
	PGM_P name;                                   /* 2 byte */
	enum {none, submenu, execute, back, terminator} options;  /* 1 byte*/
	void (*fkt)(void);                            /* 2 byte */
} menu_t;

void menuexec(menu_t* menu);
void print_status(void);
void waitforkey(char key);
char waitforanykey(void);
void readnhex(uint8_t line, uint8_t x, uint8_t n,char * str);
timestamp_t abstimedifference(timestamp_t t1, timestamp_t t2);

uint8_t readnstr(uint8_t line, uint8_t x, uint8_t n,char * str);

#endif /*INTERFACE_H_*/
