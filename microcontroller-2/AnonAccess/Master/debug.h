/* debug.h */
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


#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
	#define DEBUG_INIT() debug_init()
	#define DEBUG_C(_c) debug_char(_c)
	#define DEBUG_S(_s) debug_str(_s)
	#define DEBUG_B(_b) debug_byte(_b)
#else
	#define DEBUG_INIT()
	#define DEBUG_C(_c) 
	#define DEBUG_S(_s) 
	#define DEBUG_B(_b) 
#endif


void debug_init(void);
void debug_char(char);
void debug_str(char*);
void debug_byte(char);

#endif /*DEBUG_H_*/
