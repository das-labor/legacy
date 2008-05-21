/* crygop.h */
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


#ifndef CRYGOP_H_
#define CRYGOP_H_

#include <stdint.h>

typedef struct{
	uint32_t n;
	uint32_t v;
	uint32_t limita, limitb, limitc, limitd;
	uint32_t rounds;
	uint8_t* basekey;
	uint32_t basekey_sizeB;
	uint32_t *a, *q, *b;
} crygop_ctx_t;

void print_ctx(crygop_ctx_t* ctx);


void crygop_generate_ctx(void* key, uint32_t keysize_b, uint32_t m, uint32_t rounds, crygop_ctx_t* ctx);

void crygop_free_ctx(crygop_ctx_t* ctx);

uint32_t crygop_enc(uint32_t plain, crygop_ctx_t* ctx);

uint32_t crygop_dec(uint32_t plain, crygop_ctx_t* ctx);

#endif

