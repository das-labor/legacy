/*
 *  Copyright (C) 2009 by Soeren Heisrath <forename@surename dot org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 */

#include <stdlib.h>
#include <GL/gl.h>
#pragma once

void show_lines ();
void effect_intro ();
void show_coordsys ();
void show_triangle ();
void draw_landscape (GLuint);
void drawfog();
void effect_handler (uint8_t in_cmd);
void show_repeatexample();

#define EF_SHOW 0x00
#define EF_NEXT 0x01
#define EF_LAST 0x02
#define EF_SWITCH 0x03
#define EF_DISABLE 0x04
#define EF_ENABLE 0x05
#define EF_TICK 0x06
