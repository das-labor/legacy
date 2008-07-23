/* fairrnd.c */
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


/**
 * 
 * 
 * 
 * 
 * 
 * 
 **/
#include <stdint.h>
#include "entropium.h" 
#include "fairrnd.h"

uint32_t fairrnd(uint32_t m){
	uint64_t limit;
	limit=((uint64_t)(0xFFFFFFFFFFFFFFFFLL / (uint64_t)m))*(uint64_t)m;
	uint64_t a;
	do {
		entropium_fillBlockRandom(&a, 8);
	} while (a>=limit);
	return a%m;
}

