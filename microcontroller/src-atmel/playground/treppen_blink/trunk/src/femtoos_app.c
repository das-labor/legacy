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

/* This file is solely for testing purposes. */

/* This is not a particular application, it is only meant to see if the
 * core compiles. Most options are switched on. */

#include "femtoos_code.h"

#if (callAppBoot == cfgTrue)
  void appBoot(void) { }
#endif


#if (callAppTick00 == cfgTrue)
  void appTick00(void) { }
#endif


#if (callAppTick08 == cfgTrue)
  void appTick08(void) { }
#endif


#if (callAppTick16 == cfgTrue)
  void appTick6(void) { }
#endif


#if (preTaskDefined(task0))

  #if (callAppInit == cfgTrue)
    void Init_task0(void) { }
  #endif

  void appLoop_task0(void)
  { genFireEvent(7);
    while (true); }

  #if (cfgUseTaskWatchdog == cfgTrue) && (callAppBark == cfgTrue)
    void appBark_task0(void) { }
  #endif

#endif

