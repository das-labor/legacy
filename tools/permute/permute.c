#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t n=0;
uint16_t *p=NULL;

uint8_t l;

void vtoa(uint16_t v, char * str){
	uint16_t i;
	for(i=0; i<l; ++i){
		*str++=((v&(1<<(l-i-1))))?'1':'0';
	}
	*str='\0';
}

void push(uint16_t v){
	n++;
	p=realloc(p, n*sizeof(uint16_t));
	p[n-1]=v;
}

uint16_t find(uint16_t v){
	uint16_t i;
	for(i=0; i<n; ++i){
		if(v==p[i])
			return i;
	}
	return (uint16_t)-1;
}

uint16_t rot1(uint16_t v){
	uint8_t c;
	c=v>>(l-1);
	v<<=1;
	v|=c;
	v&=(1<<l)-1;
	return v;
}

uint8_t exists_rot(uint16_t v){
	uint8_t i;
	for(i=0; i<l; ++i){
		if(find(v)!=(uint16_t)-1)
			return 1;
		v = rot1(v);
	}
	return 0;
}

void print_stack(void){
	uint16_t i;
	char str[17];
	for(i=0; i<n; ++i){
		vtoa(p[i], str);
		printf("[%4.4d]: %s\n",i, str);
	}
}

void fill_buffer(void){
	uint16_t i;
	for(i=0; i<(1<<l); ++i){
		if(!exists_rot(i)){
			push(i);
		}
	}
}

void clear_stack(void){
	n=0;
	if(p){
		free(p);
		p=NULL;
	}
}

int main(void){
	//l=6;
	for(l=2; l<13; ++l){
	 printf("\n===== %2.2d =====\n", l);
	 fill_buffer();
	// print_stack();
	 printf("total: %d\n", n);
	 clear_stack();
	}
}






