/* string_extras.c */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * \file	string_extras.c
 * \author  Daniel Otte 
 * \date    2006-05-16
 * \license	GPLv3 or later
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

uint32_t stridentcnt(const char* a, const char* b){
	uint16_t i=0;
	for(;;){
		if(*a != *b || *b=='\0')
			return i;
		i++;
		a++;
		b++;
	}
}

uint16_t firstword_length(const char* s){
	uint16_t ret=0;
	while(isgraph(*s++))
		ret++;
	return ret; 
}

char* strstrip(char* str){
	if(!str)
		return str;
	char* endptr;
	while(*str && (*str==' ' || *str=='\t'))
		++str;
	endptr=str;
	while(*endptr)
		++endptr;
	do{
		--endptr;
	}while(*endptr==' ' || *endptr=='\t');
	endptr[1]='\0';
	return str;
}

void str_reverse(char* buffer){
	char *i, *j;
	char c;
	i=buffer;
	j=buffer + strlen(buffer)-1;
	while(i<j){
		c = *i;
		*i = *j;
		*j = c;
		++i;
		--j;
	}
}

char* ultoa(unsigned long a, char* buffer, uint8_t radix){
	if(radix<2 || radix>36){
		return NULL;
	}
	char* ptr=buffer;
	div_t result;
	if(a==0){
		ptr[0] = '0';
		ptr[1] = '\0';
		return buffer;
	}
	while(a){
		result = div(a, radix);
		*ptr = result.rem;
		if(result.rem<10){
			*ptr += '0';
		}else{
			*ptr += 'a'-10;
		}
		++ptr;
		a = result.quot;
	}
	*ptr = '\0';
	str_reverse(buffer);
	return buffer;
}
/*
void strlwr(char* s){
	while(*s){
		*s = tolower(*s);
		s++;
	}
}
*/
