/*
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * license: GPLv3
 * 
 * */

#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>
 
/******************************************************************************/

uint8_t dbz_strcount(const char* str){
	uint8_t ret=1;
	if(*str=='\0')
			return 0;
	for(;;){
		while(*str++)
			;
		if(*str=='\0')
			return ret;
		++ret;
	}	
}

/******************************************************************************/

void dbz_splitup(char* dbzstr, char** strings){
	if(*dbzstr=='\0')
		return;
	*strings++ = dbzstr;
	for(;;){	
		while(*dbzstr++)
			;
		if(*dbzstr=='\0')
			return;
		*strings++ = dbzstr;
	}
}

/******************************************************************************/

uint8_t dbz_strcount_P(PGM_P str){
	uint8_t ret=1;
	if(pgm_read_byte(str)=='\0')
			return 0;
	for(;;){
		while(pgm_read_byte(str++))
			;
		if(pgm_read_byte(str)=='\0')
			return ret;
		++ret;
	}	
}

/******************************************************************************/

void dbz_splitup_P(PGM_P dbzstr, PGM_P* strings){
	if(pgm_read_byte(dbzstr)=='\0')
		return;
	*strings++ = dbzstr;
	for(;;){	
		while(pgm_read_byte(dbzstr++))
			;
		if(pgm_read_byte(dbzstr)=='\0')
			return;
		*strings++ = dbzstr;
	}
}

/******************************************************************************/

