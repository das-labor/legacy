/* bitfunctions.h */
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


/**
 * \file	bitfunctions.h
 * \author	Daniel Otte 
 * \date	2008-05-17
 * \brief	functions for manipulating bits and non byte quantities
 * \par License	
 * GPLv3
 * 
 */
 
#ifndef BITFUNCTIONS_H_
#define BITFUNCTIONS_H_

#include <stdint.h>

uint8_t getbit(void* buffer, unsigned i);
void setbit(void* buffer, unsigned i, uint8_t value);
void membitsplit(void* a, void* b, void* src, uint16_t length_b);
void membitmerge(void* dest, void* a, void* b, uint16_t length_b);
void memand(void* dest, void* src, uint16_t length_B);

#endif /*BITFUNCTIONS_H_*/

