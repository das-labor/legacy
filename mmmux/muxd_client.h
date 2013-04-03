/*   MMMUX - a device access multiplexing library
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
 *   Copyright (C) 2012,2013 Soeren Heisrath (forename at surename dot org)
 */

#include "muxd.h"

#pragma once
int mmmux_client_connect (mmmux_sctx_t *in_c);
int mmmux_send (mmmux_sctx_t *in_c, void* in_buf, size_t in_len);
int mmmux_ctrl (mmmux_sctx_t *in_c, mmmux_ctrl_t in_var, void *in_data);
