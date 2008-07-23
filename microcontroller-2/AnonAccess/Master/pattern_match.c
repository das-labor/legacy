
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