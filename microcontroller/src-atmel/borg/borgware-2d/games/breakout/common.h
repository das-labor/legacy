/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Author & Copyright (C) 2010: Soeren Heisrath (forename@surename.org)
 *
 */

#ifndef COMMON_H
#define COMMON_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../joystick/joystick.h"
#include "../../config.h"
#include "../../autoconf.h"
#include "../../compat/eeprom.h"
#include "../../random/prng.h"
#include "../../compat/pgmspace.h"
#include "../../util.h"
#include "../../menu/menu.h"
#include "../../pixel.h"
#include "config.h"
#include "playfield.h"
#include "ball.h"
#include "score.h"
#include "level.h"
#include "rebound.h"
#include "messages.h"

#define MAX(a,b) (a > b) ? a : b
#endif /* COMMON_H */
