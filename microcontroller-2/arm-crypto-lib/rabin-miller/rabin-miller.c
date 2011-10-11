/* rabin-miller.c */
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

#include <stdint.h>
#include "bigint.h"

uint8_t rabin_miller_iscomposite(bigint_t* n, bigint_t* a){
	if(n->length_B==0){
		return 0;
	}
	if(n->wordv[0]&1==0){
		return 1;
	}
	bigint_t d,t, m1;
	uint16_t j=0;
	uint8_t d_b[n->length_B], t_b[n->length_B], m1_b[n->length_B];
	do{
		j++;
	}while(n->wordv[j/8]&(1<<(j&7))==0);
	d.wordv = d_b;
	t.wordv = t_b;
	m1.wordv = m1_b;
	bigint_copy(&d, n);
	bigint_copy(&m1, n);
	bigint_shiftright(&d, j);
	m1.wordv[0] &= 0xFE;
	bigint_expmod_u(&t, a, &d, n);

	if(t.length_B==1 && t.wordv[0]==1){
		return 0;
	}
	if(bigint_cmp_u(&t, &m1)==0){
		return 0;
	}
	--j;
	while(j){
		bigint_shiftleft(&d, 1);
		bigint_expmod_u(&t, a, &d, n);
		if(bigint_cmp_u(&t, &m1)==0){
			return 0;
		}
		--j;
	}
	return 1;
}
