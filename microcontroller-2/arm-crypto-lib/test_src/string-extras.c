/* string_extras.c */
/*
    This file is part of the ARM-Crypto-Lib.
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

#include "cli.h"

uint32_t stridentcnt(const char* a, const char* b){
	uint16_t i=0;
	for(;;){
		if(*a != *b || *b=='\0')
			return i;
		i++;
		a++;
		b++;
	}
	return 0;
}

uint16_t firstword_length(const char* s){
	uint16_t ret=0;
	if(!s){
		return 0;
	}
	int c;
	while(c=*s++, isgraph(c)){
		ret++;
	}
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
		/* toolchain bug??
		result = div(a, radix);
		*/
		result.quot = a/radix;
		result.rem = a%radix;
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


char* ulltoa(unsigned long long a, char* buffer, uint8_t radix){
	if(radix<2 || radix>36){
		return NULL;
	}
	char* ptr=buffer;
	uint8_t rem;
	unsigned long long quot;
	if(a==0){
		ptr[0] = '0';
		ptr[1] = '\0';
		return buffer;
	}
	while(a){
		rem = a % radix;
		quot = a / radix;
		if(rem<10){
			rem += '0';
		}else{
			rem += 'a'-10;
		}
		*ptr++ =rem;
		a = quot;
	}
	*ptr = '\0';
	str_reverse(buffer);
	return buffer;
}

char* ustoa(unsigned short a, char* buffer, uint8_t radix){
	return ultoa((unsigned long)a, buffer, radix);
}
/*
void strlwr(char* s){
	while(*s){
		*s = tolower(*s);
		s++;
	}
}
*/

char* itoa(int a, char* buffer, uint8_t radix){
	if(a<0){
		*buffer = '-';
		a = -a;
	}
	ultoa(a, buffer + 1, radix);
	return buffer;
}
