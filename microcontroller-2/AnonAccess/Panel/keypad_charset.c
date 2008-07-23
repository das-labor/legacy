/* keypad_charset.c */
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



#include <stdint.h>
#include <avr/pgmspace.h>
#include "keypad_charset.h"

const char alphanum_cs[] PROGMEM = 
	"0\0"
	"1\0"
	"ABC2\0"
	"DEF3\0"
	"GHI4\0"
	"JKL5\0"
	"MNO6\0"
	"PQRS7\0"
	"TUV8\0"
	"XYZ9\0"
	"\0";


const char alphanumext_cs[] PROGMEM = 
	"0 +-*/=\0"
	"1\\?!\"|\0"
	"ABCabc2\0"
	"DEFdef3\0"
	"GHIghi4\0"
	"JKLjkl5\0"
	"MNOmno6\0"
	"PQRSpqrs7\0"
	"TUVtuv8\0"
	"XYZxyz9\0"
	"(){}[]<>\0" /* A */
	".:;,\0";  /* B */

const char polynom_cs[] PROGMEM = 
	"0\0"
	"1\0"
	"2\0"
	"3\0"
	"4\0"
	"5\0"
	"6\0"
	"7\0"
	"8\0"
	"9\0"
	"*\0"  /* A */
	"*\0"  /* B */
	"*\0"; /* D */











