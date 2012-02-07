/*   NAKKALOADER - a bootloader for AVR microcontrollers & RFM12
 *   transceivers. (flasher utility)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   Copyright (C) 2012
 *       Soeren Heisrath (forename at surename dot org)
 *       Hans-Gert Dahmen (hansinator at das-labor dot org)
 */
#include "nl_flash.h"
#include <errno.h>
#include <sys/stat.h>

#pragma once

/* read a given file as binary firmware image 
 * returns values < 0 upon failure, the firmware length on success.
 */
int nflash_read_binary (nflash_ctx_t *in_c, char* in_name);
