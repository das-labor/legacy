
#ifndef CONSOLE_H
#define CONSOLE_H

#include <avr/pgmspace.h>

enum command_type {table, function};

typedef struct{
	enum command_type type;
	PGM_P  name;
	void * target;	
}command;
#define CMD_P command * PROGMEM

typedef struct{
	unsigned char size;
	const command * PROGMEM commands[];
}command_table;
#define CMD_T_P command_table * PROGMEM

typedef enum{
	vd, c, uc, i, ui, str, ptr
}argument_type;

#ifdef CONSOLE_C
const static char str__void[] PROGMEM = "void";
const static char str__char[] PROGMEM = "char";
const static char str__uchar[] PROGMEM = "uchar";
const static char str__int[] PROGMEM = "int";
const static char str__uint[] PROGMEM = "uint";
const static char str__string[] PROGMEM = "string";
const static char str__pointer[] PROGMEM = "pointer";

const char * argument_type_names[] PROGMEM ={
str__void, str__char, str__uchar, str__int, str__uint, str__string, str__pointer
};
#endif


typedef struct{
	argument_type type;
	PGM_P name;
}argument;

typedef struct{
	void (*call)(void);
	argument ret_type;
	unsigned char arg_num;
	argument arguments[];
}c_function;
#define C_FKT_P c_function * PROGMEM



void console();

#endif
