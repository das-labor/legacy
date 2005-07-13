
#define CONSOLE_UART

//the Buffer must exist 2 times. Once for the input buffer, and once for splitting by resolve
#define BUFFER_SIZE 80

//command history Buffer size
#define HISTORY_SIZE 100

//number of args resolve functioytes each.n can seperate. 2 bytes each.
#define MAX_ARGS 10

#define PROMPT "\n\r>"


#include <string.h>
#include <stdlib.h>

#define CONSOLE_C
#include "console.h"

#ifdef CONSOLE_UART
#include "uart.h"
#define stdin_getc_nb uart_getc_nb
#define stdout_putc uart_putc
#define stdout_putstr uart_putstr
#define stdout_putstr_P uart_putstr_P
#endif

extern const command_table main_table PROGMEM;

char getc_tr_nb(char * out){
	char tmp;
	unsigned char esc_buf=0;
	static unsigned char esc = 0;
	if(stdin_getc_nb(&tmp)){
		if(esc){
			if(esc == 2){
				esc = 0;
				switch ( tmp ){
					case ('A'):
						*out = 0xF0;
						return 1;
						break;
					case ('B'):
						*out = 0xF1;
						return 1;
						break;
						
					default:
						return 0;
						break;
				}
			}else{
				esc_buf = tmp;
				esc = 2;
			}
		}
		else if(tmp != 0x1b){
			*out = tmp;
			return 1;
		}else{
			esc = 1;
		}
	}
	return 0;
}


void command_table_print(CMD_T_P tbl){
	unsigned char x;
	unsigned char tbl_size = pgm_read_byte(&tbl->size);
	for(x=0;x<tbl_size;x++){
		stdout_putstr_P(PSTR("\r\n"));
		stdout_putstr_P((PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name ));
	}
}

void function_info_print(C_FKT_P func){
	unsigned char arg_num = pgm_read_byte(&func->arg_num);
	unsigned char x;
	PGM_P tmp;
	stdout_putstr_P(PSTR("\r\n"));
	stdout_putstr_P((PGM_P) pgm_read_word ( &argument_type_names[pgm_read_byte(&func->ret_type.type)] ));
	stdout_putc(' ');
	if((tmp = (PGM_P) pgm_read_word(&func->ret_type.name )) != 0){
		stdout_putstr_P(tmp);
		stdout_putc(' ');
	}
	stdout_putc('(');
	for (x=0; x<arg_num;x++){
		stdout_putstr_P((PGM_P) pgm_read_word ( &argument_type_names[pgm_read_byte(&func->arguments[x].type)] ));
		stdout_putc(' ');
		if((tmp = (PGM_P) pgm_read_word(&func->arguments[x].name )) != 0){
			stdout_putstr_P(tmp);
		}
		stdout_putc(',');
	}
	stdout_putc(')');
}

//returns how many characters of the strings match 
unsigned char strcmp_num_PP(PGM_P s1, PGM_P s2){
	char tmp1, tmp2;
	unsigned char count = 0;
	while( ((tmp1=pgm_read_byte(s1))) && ((tmp2=pgm_read_byte(s2))) ){
		if (tmp1 != tmp2){
			return count;
		}
		s1++; s2++;
		count++;
	}
	return count;

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

//returns 0 on multiple or no results, 1 on one result
unsigned char command_table_search(CMD_T_P tbl, unsigned char * string, unsigned char ** bufend){
	unsigned char x, found = 0;
	unsigned char tbl_size = pgm_read_byte(&tbl->size);
	PGM_P found_s = 0;
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
		*bufend = (string + strlen_P(found_s));
		return 1;
	}else{
		unsigned char min_match_len = 255;
		unsigned char tmp;
		PGM_P ref_str=0;
		for(x=0;x<tbl_size;x++){
			PGM_P command_name = (PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name);
			if( begin_cmp_P (string, command_name)){
				stdout_putstr_P(PSTR("\r\n"));
				stdout_putstr_P((PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&tbl->commands[x]))->name ));
				if(!ref_str){
					ref_str = command_name;
				}else{
					tmp = strcmp_num_PP(command_name, ref_str);
					if(tmp < min_match_len) min_match_len = tmp;
				}
			}
		}
		memcpy_P(string, ref_str, min_match_len);
		*bufend = (string + min_match_len);
		**bufend = 0;
	}
	return 0;	
}


unsigned char split(unsigned char * string, unsigned char splitchar, unsigned char * substrings[]){
	
	unsigned char tmp, num=0;
	unsigned char * str_start = string;
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

//searches command tree for an object fitting the "path" typed in
//returns target and rest
enum resolve_result resolve_command(unsigned char * cmd, unsigned char ** rest, void ** target){
	unsigned char * args[MAX_ARGS];
	unsigned char split_buffer[BUFFER_SIZE];
	strcpy(split_buffer, cmd);
	unsigned char argnum = split(split_buffer, ' ', args);
	const command_table * PROGMEM my_table = &main_table;
	*target = (void *)my_table;
	unsigned char x, y;
	for(x=0;x<argnum;x++){
		unsigned char found = 0;
		for(y=0;y<pgm_read_byte(&(my_table->size));y++){
			PGM_P command_name = (PGM_P) pgm_read_word(&((CMD_P)pgm_read_word(&my_table->commands[y]))->name );
			if (!strcmp_P(args[x], command_name)){
				enum command_type type = pgm_read_byte(&((CMD_P)pgm_read_word(&my_table->commands[y]))->type);
				if(type == table){
					my_table = (CMD_T_P)pgm_read_word(&((CMD_P)pgm_read_word(&my_table->commands[y]))->target );
					*target = (void *)my_table;
					found = 1;
					y = 255;
					break;
				}else if(type == function){
					*target = (void*)pgm_read_word(&((CMD_P)pgm_read_word(&my_table->commands[y]))->target);
					if(x<argnum-1){
						*rest = args[x+1]+(cmd-split_buffer);
					}else{
						*rest = 0;
					}
					return r_function;
				}
			}
		}
		if (!found){
			*rest = args[x]+(cmd-split_buffer);
			return r_incomplete;
		}
	}
	*rest = 0;
	return r_table;
}

void function_do_call(int * registers, void(*call)() ){
asm volatile(
	"	push	r28\n"
	
	"	push	r26\n"
	"	push	r27\n"
	
	"	ldi	r28, 8\n"
	"	eor	r29, r29\n"
	"lp1:	ld	r7, X+\n"
	"	st	Y+, r7\n"
	"	cpi	r28, 26\n"
	"	brlo	lp1\n"
	
	"	icall\n"
	
	"	pop	r27\n"
	"	pop	r26\n"
	
	"	ldi	r28, 18\n"
	"	eor	r29, r29\n"
	"lp2:	ld	r7, Y+\n"
	"	st	X+, r7\n"
	"	cpi	r28, 26\n"
	"	brlo	lp2\n"
	
	"	pop	r28\n"
	:
	:"z" (call), "x" (registers)
	:"r29", "r25", "r24", "r23", "r22", "r21", "r20", "r19", "r18", "r17", "r15", "r14", "r13", "r12", "r10", "r9", "r8", "r7"
	);
}



void function_call(C_FKT_P target, char * argstring){
	unsigned char strbuffer[BUFFER_SIZE];
	unsigned char * args[MAX_ARGS];
	unsigned char arg_num;
	
	char buf_tmp[10];
	
	int registers[9];//will be copied to register r8:r25 before function call
	
	if(argstring){
		strcpy(strbuffer, argstring);
		arg_num = split (strbuffer, ',', args );
	}else{
		arg_num = 0;
	}
	if(arg_num != pgm_read_byte(&target->arg_num)){
		stdout_putstr_P(PSTR("Error: Wrong number of Arguments"));
	}else{
		//stdout_putstr_P(PSTR("klappt"));
		unsigned char x, rp=8;
		for(x=0;x!=0xFF;x++){
			switch ((argument_type) pgm_read_byte(&target->arguments[x].type)){
				case ptr:
				case ui:
				case i:
					registers[rp] =  (int)strtol(args[x], (char **)0, 0); 
					rp--;
					break;
				case c:
				case uc:
					registers[rp] =  (int)strtol(args[x], (char **)0, 0);
					rp--;
					break;
				case str:
					registers[rp] = (int)args[x];
					rp--;
					break;
				case vd:
					break;
			}
		}
		void (*call)(void) = (void(*)(void))pgm_read_word(&target->call);
		function_do_call(registers, call);
		argument_type tmparg = (argument_type) pgm_read_byte(&target->ret_type.type);
		if(tmparg != vd){
			stdout_putstr_P(PSTR("\r\nreturn: "));
			switch (tmparg){
				case uc:
					utoa(registers[3]&0xFF, buf_tmp, 10);
					stdout_putstr(buf_tmp);
					stdout_putstr_P(PSTR(" = 0x"));
					utoa(registers[3]&0xFF, buf_tmp, 16);
					stdout_putstr(buf_tmp);
					break;
				case ptr:
				case ui:
					utoa(registers[3], buf_tmp, 10);
					stdout_putstr(buf_tmp);
					stdout_putstr_P(PSTR(" = 0x"));
					utoa(registers[3], buf_tmp, 16);
					stdout_putstr(buf_tmp);
					break;
				case c:
					itoa((char)registers[3], buf_tmp, 10);
					stdout_putstr(buf_tmp);
					stdout_putstr_P(PSTR(" = 0x"));
					itoa((char)registers[3], buf_tmp, 16);
					stdout_putstr(buf_tmp);
					break;
				case i:
					itoa(registers[3], buf_tmp, 10);
					stdout_putstr(buf_tmp);
					stdout_putstr_P(PSTR(" = 0x"));
					itoa(registers[3], buf_tmp, 16);
					stdout_putstr(buf_tmp);
					break;
				case str:
					stdout_putstr((char*)registers[3]);
					break;
				case vd:
					break;
			}
		}
	}
}


static unsigned char history_buf[HISTORY_SIZE];
static unsigned char * history_pos = history_buf;

void history_init(){
	unsigned char x;
	for(x=0;x<HISTORY_SIZE;x++){
		history_buf[x]=0;
	}
}

void history_save(unsigned char * string){
	unsigned char tmp;
	do{
		*history_pos++ = tmp = *string++;
		if(history_pos == history_buf+HISTORY_SIZE) history_pos = history_buf;
	}while(tmp);
}

void history_load(unsigned char * buffer, unsigned char ** bufend, unsigned char num){
	unsigned char * pos = history_pos;
	unsigned char tmp;
	unsigned char x;
	for(x=0;x<num+1;x++){
		do{
			if(--pos<history_buf) pos = history_buf+HISTORY_SIZE-1;
		}while(*pos);
	}
	if(++pos == history_buf+HISTORY_SIZE) pos = history_buf;
	do{
		tmp = *pos;
		if(++pos == history_buf+HISTORY_SIZE) pos = history_buf;
		*buffer++ = tmp;
	}while(tmp);
	*bufend = buffer-1;
}


void console(){
	static unsigned char buffer[BUFFER_SIZE];
	static unsigned char * buf_pos = buffer;
	static unsigned char history_num;
	unsigned char tmp;
	unsigned char * rest; void * target;
	
	while(getc_tr_nb(&tmp)){
		if(tmp == '\r'){
			history_num = 0;
			if(buf_pos != buffer){
				*buf_pos = 0;	//terminate line
				history_save (buffer);
				buf_pos = buffer;   //reset pointer
				stdout_putstr_P(PSTR("\r\n"));
				switch(resolve_command(buffer, &rest, &target)){
					case r_incomplete:
						stdout_putstr_P(PSTR("Error: No such command"));
						break;
					case r_table:
						stdout_putstr_P(PSTR("Error: Command incomplete"));
						break;
					case r_function:
						function_call(target, rest);
						break;
				}
			}
			stdout_putstr_P(PSTR(PROMPT));
		}else if(tmp == '\t'){
			*buf_pos = 0;
			switch(resolve_command(buffer, &rest, &target )){
				case r_incomplete:
					if((command_table_search((CMD_T_P)target, rest, &buf_pos))){
						*buf_pos++ = ' ';
						uart_putc (' ');
					}else{
						stdout_putstr_P(PSTR(PROMPT));
						stdout_putstr(buffer);
					}
					break;
				case r_table:
					command_table_print(target);
					stdout_putstr_P(PSTR(PROMPT));
					stdout_putstr(buffer);
					break;
				case r_function:
					function_info_print(target);
					stdout_putstr_P(PSTR(PROMPT));
					stdout_putstr(buffer);
					break;
			}
		}else if(tmp == 0x08){	//backspace
			if (buf_pos > buffer){
				buf_pos--;
				stdout_putstr_P(PSTR("\x8 \x8"));
			}
		}else if(tmp == 0xF0){  //up
			history_load(buffer, &buf_pos, ++history_num);
			stdout_putstr_P(PSTR(PROMPT));
			stdout_putstr(buffer);
		}else if(tmp == 0xF1){  //down
			if(history_num > 1){
				history_load(buffer, &buf_pos, --history_num);
				stdout_putstr_P(PSTR(PROMPT));
				stdout_putstr(buffer);
			}
		}else if(buf_pos < buffer+BUFFER_SIZE-1){ //buffer full?
			*buf_pos++ = tmp;		//no: write character to buffer
			stdout_putc (tmp);
		}
	}		
}

void console_init(){
	history_init();
	uart_init();

}
