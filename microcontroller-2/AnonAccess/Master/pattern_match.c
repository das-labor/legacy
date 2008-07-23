/* pattern_match.c */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#define CASE_SENS_BIT ((A)^(a))

/* remove multiple '*' wich follow direct after another */
uint8_t simplify_pattern(char* pattern){
	uint8_t in_ast=0;
	uint8_t slen=strlen(pattern);
	while(*pattern){
		if(*pattern=='*'){
			if(in_ast){
				char *p;
				p=pattern
				while(*p){
					*(p-1) = *p;
					p++;
				}
			} else {
				in_ast=1;
			}
		}
		pattern++;
	}
}

uint8_t check_end(const char* pattern, const char* string, uint8_t casesens){
	char case_mod;
	case_mod=casesens?0:CASE_SENS_BIT;
	uint8_t pidx, sidx;
	pidx=strlen(pattern);
	sidx=strlen(string);
	if(pattern[pidx-1]=='*')
		return 1;
	while(pidx && sidx && pattern[pidx]!='*'){
		sidx--;
		pidx--;
		if(((pattern[pidx]|case_mod) != (string[sidx]|case_mod)) && (pattern[pidx]!='?'))
			return 0;
	}
	if(pattern[pidx]=='*')
		return 2;
	if(pidx==sidx)
		return 3;
	return 0;
} 

uint8_t check_start(const char* pattern, const char* string, uint8_t casesens){
	char case_mod;
	case_mod=casesens?0:CASE_SENS_BIT;
	if(*pattern=='*')
		return 1;
	while(*pattern!='*' && *pattern){
		if(((*pattern|case_mod) != (*string|case_mod)) && (*pattern!='?'))
			return 0;
		++string;
		++pattern;
	}
	return (*pattern)?2:3;
}

uint8_t pattern_match(const char* pattern, const char* string, 
                      uint8_t casesensitive){
	char *s, *p, *last_ast=0;
	uint8_t temp=1, post_ast=0;
	
	s=string; p=pattern;
	if(*pattern=='*' && *(pattern+1)=='\0')
		return 1;
	switch(check_start(pattern, string, casesensitive)){
		case 0: return 0;
		case 3: return 1;
		default: break;
	}
	return check_end(pattern, string, casesensitive);
	
}

uint8_t chaek_pattern_match(char* pattern, const char* string, 
                            uint8_t casesensitive){
	simplify_pattern(pattern);
	return pattern_match(pattern, string, casesensitive);
}