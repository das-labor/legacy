/* rc6.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

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
/* 
 * File:	rc6.c
 * Author:	Daniel Otte
 * Date: 	06.08.2006
 * License: GPL
 * Description: Implementation of the RC6 cipher algorithm.
 * 	This implementation is restricted to 32-bit words and to keys up to 65535 bit in length (but this is
 *  quite easy to expand), but free in the choice of number of rounds (0 to 125).
 * 	so it is RC6-32/r/b
 * THIS ONLY WORKS FOR LITTEL ENDIAN!!!
 */
 
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rc6.h"
 
#define P32 0xB7E15163		/* e -2 */
#define Q32 0x9E3779B9		/* Golden Ratio -1 */
 
uint32_t rotl32(uint32_t a, uint8_t n){
 	n &= 0x1f; /* higher rotates would not bring anything */
 	return ( (a<<n)| (a>>(32-n)) );
}

uint32_t rotr32(uint32_t a, uint8_t n){
 	n &= 0x1f; /* higher rotates would not bring anything */
 	return ( (a>>n)| (a<<(32-n)) );
}
 
void rc6_init(const void* key, uint16_t keylength_b, rc6_ctx_t *s){
 	rc6_initl(key, keylength_b, 20, s);
}
 
 
uint8_t rc6_initl(const void* key, uint16_t keylength_b, uint8_t rounds, rc6_ctx_t *s){
 	uint8_t i,j;
 	uint16_t v,p,c;
 	uint32_t a,b;
 	c =( keylength_b+31)/32;
 	uint32_t local_key[c];
 	if (rounds>125)
 		return 2;
 	if(!(s->S=malloc((2*rounds+4)*sizeof(uint32_t))))
 		return 1;
 	
 	s->rounds=rounds;
 	local_key[c-1] = 0;
 	memcpy(local_key, key, (keylength_b+7)/8);
 	
 	s->S[0] = P32;
 	for(i=1; i<2*rounds+4; ++i){
		s->S[i] = s->S[i-1] + Q32;
	}
	
	a=b=j=i=0;
	v = 3 * ((c > (2*rounds+4))?c:(2*rounds+4));
	for(p=1; p<=v; ++p){
		a = s->S[i] = rotl32(s->S[i] + a + b, 3);
		b = ((uint32_t*)local_key)[j] = rotl32(((uint32_t*)local_key)[j]+a+b, a+b);
		i = (i+1) % (2*rounds+4);
		j = (j+1) % c;
	}
	return 0;
}
 
void rc6_free(rc6_ctx_t *s){
 	free(s->S);
} 
 
#define LG_W 5
#define A (((uint32_t*)block)[0])
#define B (((uint32_t*)block)[1])
#define C (((uint32_t*)block)[2])
#define D (((uint32_t*)block)[3])
 
void rc6_enc(void* block, rc6_ctx_t *s){
 	uint8_t i;
 	uint32_t t,u,x; /* greetings to Linux? */
 	uint32_t *p;
 	p=s->S;
 	B += *p++;
 	D += *p++;
 	i=s->rounds;
 	do{
 		t = rotl32(B * (2*B+1), LG_W);
 		u = rotl32(D * (2*D+1), LG_W);
 		A = rotl32((A ^ t), u) + *p++;
 		C = rotl32((C ^ u), t) + *p++;
 		x = A;
 		A = B;
 		B = C;
 		C = D;
 		D = x;
 	}while(--i);
 	A += *p++;
 	C += *p;
}
 
void rc6_dec(void* block, rc6_ctx_t *s){
 	uint8_t i;
 	uint32_t t,u,x; /* greetings to Linux? */
 	uint32_t *p;
 	p = &(s->S[2*(s->rounds)+3]);
 	C -= *p--;
 	A -= *p--;
 	
 	i=s->rounds;
 	do{
 		x=D;
 		D=C;
 		C=B;
 		B=A;
 		A=x;
 		u = rotl32(D * (2*D+1), LG_W);
 		t = rotl32(B * (2*B+1), LG_W);
 		C = rotr32(C - *p--, t) ^ u;
 		A = rotr32(A - *p--, u) ^ t;
 	}while(--i);
 	D -= *p--;
 	B -= *p;
}
 
