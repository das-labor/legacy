/* percnt2.h */
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


#ifndef PERCNT2_H_
#define PERCNT2_H_

#include <stdint.h>
#include "config.h"


#define PERCNT_OK 0
#define PERCNT_ERROR 1
#define PERCNT_HAVYERROR 2
#define PERCNT_INITERROR 23
#define PERCNT_STRANGEERROR 42

void percnt_reset(uint8_t counter);
uint32_t percnt_get(uint8_t counter);
uint8_t percnt_inc(uint8_t counter);
uint8_t percnt_init(uint8_t counter);

#endif /*PERCNT2_H_*/
