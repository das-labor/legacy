
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

AVRX_EXTERNTASK(console_task);


void console();

#endif
