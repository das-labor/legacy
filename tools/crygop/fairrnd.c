/* fairrnd.c */
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
#include <string.h>
#include "sha256.h"


uint32_t fairrnd_seeded(uint32_t n, void* seed, uint32_t seedsize_b){
	uint64_t t[4];
	uint64_t limit;
	unsigned i;
	limit=(0xFFFFFFFFFFFFFFFFLL/(uint64_t)n)*(uint64_t)n;
	sha256((sha256_hash_t*)t, seed, seedsize_b);
	for(;;){
		i=0;
		do{
			if(t[i]<limit)
				return (t[i]%n);
			++i;
		}while(i<4);
		sha256((sha256_hash_t*)t, t, 256);
	}
}



