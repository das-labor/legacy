/* crygop.c */
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


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sha256.h"
#include "shabea.h"
#include "exteuclid.h"
#include "fairrnd.h"
#include "crygop.h"
#include "bitfunctions.h"
#include <stdio.h>

#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 #include <stdio.h>
 extern uint8_t interim;
#endif

void crygop_generate_ctx(void* key, uint32_t keysize_b, uint32_t m, uint32_t rounds, crygop_ctx_t* ctx){
	ctx->rounds = rounds;
	ctx->n = m-1;
	ctx->v = 0;
	while(m){
		ctx->v++;
		m>>=1;
	}
	/* 2**v <= n */
	ctx->v -= 1;
	ctx->v &= ~1;
	ctx->limita = (1<<(ctx->v))-1;
	ctx->limitb = ((ctx->limita+1)<<1) -1;
	if(ctx->limitb>ctx->n)
		ctx->limitb=0;
	ctx->limitc = ((ctx->limita+1)*3) -1;
	if(ctx->limitc>ctx->n)
		ctx->limitc=0;
	ctx->limitd = ctx->n - ctx->limita -1;
	ctx->basekey_sizeB = (keysize_b+7)/8;
	ctx->basekey = malloc(ctx->basekey_sizeB+5);
	memcpy(ctx->basekey, key, ctx->basekey_sizeB);
	ctx->a = malloc(rounds*sizeof(uint32_t));
	ctx->q = malloc(rounds*sizeof(uint32_t));
	ctx->b = malloc(rounds*sizeof(uint32_t));
	
	int32_t i,x,r,s;
	for(i=0;i<rounds; ++i){
		ctx->basekey[ctx->basekey_sizeB]='A';
		*((uint16_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		*((uint16_t*)&(ctx->basekey[ctx->basekey_sizeB+3]))= 0;
		do{
			*((uint16_t*)&(ctx->basekey[ctx->basekey_sizeB+3])) = *((uint16_t*)&(ctx->basekey[ctx->basekey_sizeB+3]))+1;
			x=fairrnd_seeded(ctx->n-1, ctx->basekey, (ctx->basekey_sizeB+5)*8); 
			x+=2;
		}while(exteuclid(ctx->n+1,x, &r, &s)!=1);
		while(x<0){
			x+=ctx->n+1;	
		}
		while(s<0){
			s+=ctx->n+1;	
		}
		ctx->a[i] = x;
		ctx->q[i] = s;
		ctx->basekey[ctx->basekey_sizeB]='B';
		ctx->b[i] = fairrnd_seeded(ctx->n+1, ctx->basekey, (ctx->basekey_sizeB+5)*8);
	}
}

void print_ctx(crygop_ctx_t* ctx){
/*
	uint32_t n;
	uint32_t v;
	uint32_t limita, limitb, limitc;
	uint32_t rounds;
	uint8_t* basekey;
	uint32_t basekey_sizeB;
	uint32_t *a, *q, *b;
*/
	uint32_t i;
	printf("\ncrygop-context:\n\tn = %2u\n\tv = %2u\n\tlimita = %6u\n\tlimitb = %6u\n\tlimitc = %6u\n\tlimitd = %6u"
	       "\n\trounds = %2u\n", ctx->n, ctx->v, ctx->limita, ctx->limitb, ctx->limitc, ctx->limitd, ctx->rounds);
	printf("\tbasekey = ");
	for(i=0; i<ctx->basekey_sizeB; ++i){
		printf("%2.2X", ctx->basekey[i]);
	}	
	puts("");
	for(i=0; i<ctx->rounds; ++i){
		printf("\t  r =%2u : a = %4u\t  q = %4u\t  b = %4u\n",
		       i, ctx->a[i], ctx->q[i], ctx->b[i]);   	
	}
}

void crygop_free_ctx(crygop_ctx_t* ctx){
	free(ctx->basekey);
	free(ctx->a);
	free(ctx->q);
	free(ctx->b);
}


uint32_t crygop_enc(uint32_t plain, crygop_ctx_t* ctx){
	uint32_t i;
	uint32_t x;
	x=plain;
	for(i=0; i<ctx->rounds; ++i){
		x = (uint64_t)((uint64_t)((uint64_t)x * (uint64_t)(ctx->a[i])) + (uint64_t)(ctx->b[i])) % ((uint64_t)(ctx->n+1));
		
		#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 		if(interim){
		  printf(" ~> %4u", x);
		}
		#endif
		ctx->basekey[ctx->basekey_sizeB]='a';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x<=ctx->limita){
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 1, 4);
		}
		
		ctx->basekey[ctx->basekey_sizeB]='b';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x<=ctx->limitb && x>ctx->limita){
			x-=ctx->limita+1;
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 1, 4);
			x+=ctx->limita+1;		
		}
		ctx->basekey[ctx->basekey_sizeB]='c';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x<=ctx->limitc && x>ctx->limitb){
			x-=ctx->limitb+1;
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 1, 4);
			x+=ctx->limitb+1;		
		}
		ctx->basekey[ctx->basekey_sizeB]='d';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x>ctx->limitd){
			x-=ctx->limitd+1;
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 1, 4);
			x+=ctx->limitd+1;		
		}
		#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 		if(interim){
		  printf(" -> %4u", x);
		}
		#endif		
	}
	return x;
}


uint32_t crygop_dec(uint32_t plain, crygop_ctx_t* ctx){
	int32_t i;
	int32_t x;
	x=plain;
	for(i=ctx->rounds-1; i>=0; --i){
	
		ctx->basekey[ctx->basekey_sizeB]='d';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x>ctx->limitd){
			x-=ctx->limitd+1;
		        shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 0, 4);
			x+=ctx->limitd+1;
		}

		ctx->basekey[ctx->basekey_sizeB]='c';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x<=ctx->limitc && x>ctx->limitb){
			x-=ctx->limitb+1;
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 0, 4);
			x+=ctx->limitb+1;
		}

		ctx->basekey[ctx->basekey_sizeB]='b';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x<=ctx->limitb && x>ctx->limita){
			x-=ctx->limita+1;
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 0, 4);
			x+=ctx->limita+1;
		}
		
		ctx->basekey[ctx->basekey_sizeB]='a';
		*((uint32_t*)&(ctx->basekey[ctx->basekey_sizeB+1]))= i;
		if(x<=ctx->limita)
			shabean(&x, ctx->v, ctx->basekey, (ctx->basekey_sizeB+5)*8, 0, 4);
	
		
		#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 		if(interim){
		  printf(" <- %4u", x);
		}
		#endif

		x -= ctx->b[i];		
		while(x<0)
			x+=ctx->n+1;
		if(x>(ctx->n))
			x%=ctx->n+1;

		x = (((uint64_t)(x)) * ((uint64_t)(ctx->q[i])))%((uint64_t)(ctx->n+1));		
		while(x<0)
			x+=ctx->n+1;
		if(x>(ctx->n))
			x%=ctx->n+1;

		#ifdef CRYGOP_SUPPORT_INTERIMRESULTS
 		if(interim){
		  printf(" <~ %4u", x);
		}
		#endif

	}
	return x;
}


