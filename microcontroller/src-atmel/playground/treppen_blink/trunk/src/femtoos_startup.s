/*
 * Femto OS v 0.92 - Copyright (C) 2008-2010 Ruud Vlaming
 *
 * This file is part of the Femto OS distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Please note that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */

#include "femtoos_globals.h"
#include "femtoos_constants.h"
#include "femtoos_order.h"
#include "femtoos_locals.h"

/* Note, it is a hack to include assembly code via the header file femtoos_device.h. Still i think
 * it is the right thing to do. For avr architectures it makes it possible to contain all device
 * dependant information in one file only. We activate the assembly code by defining the macro below.
 * The femtoos_device.h headerfile contains NO once-macro construction.
 */
#define FEMTOOS_DEVICE_ASSEMBLY
#include "femtoos_device.h"

