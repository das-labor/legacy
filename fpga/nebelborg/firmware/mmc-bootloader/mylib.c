
#include <stdlib.h>

void *memset(void *s, int c, size_t n){
	char * p = s; 
	
	while(n--){
		*p++ = c;
	}

	return s;
}

void *memcpy(void *dest, const void *src, size_t n){
	char *destp;
	const char * srcp;
	srcp = src; destp = dest;
	while(n--){
		*destp++ = *srcp++;
	}
	
	return dest;
}

int memcmp(const void *s1, const void *s2, size_t n){
	const char *p1, *p2;
	p1 = s1; p2 = s2;
	while(n--){
		if(*p1 == *p2){
			p1++; p2++;
		}else{
			if(*p1 > *p2)
				return 1;
			return -1;
		}
	}
	return 0;
}
