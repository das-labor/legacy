
#define CONSOLE_UART

#define BUFFER_SIZE 80
#define MAX_ARGS 10
#define PROMPT "\n\r>"


#include <string.h>


#ifdef CONSOLE_UART
#include "uart.h"
#define stdin_getline_nb uart_getline_nb
#define stdout_putc uart_putc
#define stdout_putstr uart_putstr
#define stdout_putstr_P uart_putstr_P
#endif

enum command_type {table, function};

typedef struct{
	enum command_type type;
	PGM_P name;
	void * target;	
}command;
#define CMD_P command * PROGMEM

typedef struct{
	unsigned char size;
	command *PROGMEM commands[];
}command_table;
#define CMD_T_P command_table * PROGMEM

typedef enum{
	vd, c, uc, i, ui, str, pstr
}argument_type;


typedef struct{
	argument_type type;
	PGM_P name;
}argument;

typedef struct{
	void (*call)(void);
	unsigned char argument_num;
	argument arguments[];
}c_function;
#define FKT_P c_function * PROGMEM

static char buffer[BUFFER_SIZE];


void echo(){
	uart_putstr("Hallo\r\n");
}

char str__void[] PROGMEM = "void";
c_function cf_echo PROGMEM={
	&echo, 1,{{vd, str__void}}
};

char str__echo[] PROGMEM = "echo";

static command c_echo PROGMEM={
	function, str__echo, &cf_echo
};

static command_table main_table PROGMEM={
	1, {&c_echo}
};

void command_table_print(CMD_T_P tbl){
	unsigned char x;
	unsigned char tbl_size = pgm_read_byte(&tbl->size);
	for(x=0;x<tbl_size;x++){
		stdout_putstr_P(PSTR("\r\n"));
		stdout_putstr_P((PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name ));
	}
	stdout_putstr_P(PSTR("\r\n"));
}


unsigned char begin_cmp_P(char * s1, PGM_P s2){
	char tmp1, tmp2;
	while( ((tmp1=*s1)) && ((tmp2=pgm_read_byte(s2))) ){
		if (tmp1 != tmp2){
			return 0;
		}
		s1++; s2++;
	}
	if(tmp1 == 0){
		return 1;
	}else{
		return 0;
	}
}

//returns 0 on multiple or no results, and pointer to new end of string, if it could be completed
char * command_table_search(CMD_T_P tbl, char * string){
	unsigned char x, found = 0;
	unsigned char tbl_size = pgm_read_byte(&tbl->size);
	PGM_P found_s;
	for(x=0;x<tbl_size;x++){
		PGM_P command_name = (PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name);
		if( begin_cmp_P (string, command_name)){
			found++;
			found_s = command_name;
		}
	}
	if(found == 0){
		return 0;
	}else if (found == 1){
		stdout_putstr_P(found_s+strlen(string));
		strcpy_P(string, found_s);
		return(string + strlen_P(found_s));
	}else{
		for(x=0;x<tbl_size;x++){
			PGM_P command_name = (PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name);
			if( begin_cmp_P (string, command_name)){
				stdout_putstr_P(PSTR("\r\n"));
				stdout_putstr_P((PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name ));
			}
		}
		stdout_putstr_P(PSTR(PROMPT));
	}
	return 0;	
}

void function_info_print(FKT_P func){
	unsigned char arg_num = pgm_read_byte(&func->argument_num);
	unsigned char x;
	for (x=0; x<arg_num;x++){
		stdout_putstr_P(PSTR("\r\n"));
		stdout_putstr_P( (PGM_P) pgm_read_word(&func->arguments[x].name ));
	}
	stdout_putstr_P(PSTR("\r\n"));
}


unsigned char split(char * string, char splitchar, char * substrings[]){
	unsigned char tmp, num=0;
	char * str_start = string;
	do{
		tmp = *string;
		if ((tmp == splitchar) || (tmp == 0)){
			*string = 0;
			if( (string!=str_start) && (num < MAX_ARGS) ){
				substrings[num] = str_start;
				num++;
			}
			str_start = string+1;
		}
		string++;
	}while(tmp);
	return num;
}


enum resolve_result{r_incomplete, r_table, r_function};

enum resolve_result resolve_command(char * cmd, char ** rest, void ** target){
	char * args[MAX_ARGS];
	unsigned char argnum = split(cmd, ' ', args);
	command_table * my_table = &main_table;
	*target = my_table;
	unsigned char x, y;
	for(x=0;x<argnum;x++){
		unsigned char found = 0;
		for(y=0;y<pgm_read_byte(&(my_table->size));y++){
			PGM_P command_name = (PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&my_table->commands[y]))->name );
			if (!strcmp_P(args[x], command_name)){
				enum command_type type = pgm_read_byte(&((CMD_P)pgm_read_word(&my_table->commands[y]))->type);
				if(type == table){
					my_table = (CMD_T_P)pgm_read_word(&((CMD_P)pgm_read_word(&my_table->commands[y]))->target );
					*target = my_table;
					found = 1;
					y = 255;
					break;
				}else if(type == function){
					*target = (void*)pgm_read_word(&((CMD_P)pgm_read_word(&my_table->commands[y]))->target);
					if(x<argnum-1){
						*rest = args[x+1];
					}else{
						*rest = 0;
					}
					return r_function;
				}
			}
		}
		if (!found){
			*rest = args[x];
			return r_incomplete;
		}
	}
	*rest = 0;
	return r_table;
}


void console(){
	static char * buf_pos = buffer;
	static char * buf_parse = buffer;
	char tmp;
	//unsigned char menu_size = pgm_read_byte(&(menu->size));
	while(uart_getc_nb(&tmp)){
		if(tmp == '\r'){
			*buf_pos = 0;	//terminate line
			buf_pos = buffer;   //reset pointer
			buf_parse = buffer;
			stdout_putstr(buffer);
			stdout_putstr_P(PSTR(PROMPT));
		}else if(tmp == '\t'){
			*buf_pos = 0;
			char * rest; void * target; char * bufend;
			switch(resolve_command(buffer, &rest, &target )){
				case r_incomplete:
					if((bufend = command_table_search((CMD_T_P)target, rest))){
						buf_pos = bufend;
					}
					break;
				case r_table:
					command_table_print(target);
					break;
				case r_function:
					function_info_print(target);
					break;
			}
			
			
			
			
		}else if(buf_pos < buffer+BUFFER_SIZE-1){ //buffer full?
			*buf_pos++ = tmp;		//no: write character to buffer
			uart_putc (tmp);
		}
	}		
}
