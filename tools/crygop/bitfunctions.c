/* bitfunctions.c */
/*
    This sourcefile is part of the crygop programm.
    Copyright (C) 2008  Daniel Otte

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




#include "bitfunctions.h"
#include <stdint.h>

uint8_t getbit(void* buffer, unsigned i){
	return ((((uint8_t*)buffer)[i/8])&(1<<(i%8)))?1:0;
}

void setbit(void* buffer, unsigned i, uint8_t value){
	value=value?1:0;
	((uint8_t*)buffer)[i/8] &= ~(1<<(i%8));
	((uint8_t*)buffer)[i/8] |= value<<(i%8);
}

void membitsplit(void* a, void* b, void* src, uint16_t length_b){
	int i;
	uint8_t t;
	for(i=0; i<length_b; ++i){
		t=getbit(src, i);
		setbit(b, i, t);
	}
	for(i=0; i<length_b; ++i){
		t=getbit(src, length_b+i);
		setbit(a, i, t);
	}
}

void membitmerge(void* dest, void* a, void* b, uint16_t length_b){
	int i;
	uint8_t t;
	for(i=0; i<length_b; ++i){
                t=getbit(b, i);
		setbit(dest, i, t);
	}
	for(i=0; i<length_b; ++i){
		t=getbit(a, i);
		setbit(dest, length_b+i, t);
	}
}

void memand(void* dest, void* src, uint16_t length_B){
	while(length_B--){
		*((uint8_t*)dest) &= *((uint8_t*)src);
		dest = (uint8_t*)dest+1;
		src  = (uint8_t*)src+1;
	}
}


