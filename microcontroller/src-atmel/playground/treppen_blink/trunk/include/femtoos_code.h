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

#ifndef FEMTOOS_CODE_H
#define FEMTOOS_CODE_H

#include "femtoos_core.h"
#include "femtoos_order.h"

/* This file is the only header file you need to include in you application code. */

/* DISCUSSION
 * Tick hooks for every tick (appTick00), for every 256 ticks (appTick08) and every
 * 65536 ticks (appTick16) in case of power down the tick00 and tick08 may be lost.
 * Ticks16 may be deferred. In case of variable time slices the hooks may be deferred,
 * tick 00 may come in rapid repetition. to catch up. ticks are not missed.
 * tick16 may for example be used to extend the tick counter. All routines run in
 * OS space with used stacks. Keep the functions shallow and quick, also the
 * tick16 function. API calls are NOT allowed from within. However, tick interrupts
 * are disabled when the functions are called, so share variables are save. Global
 * interrupts may not be disabled.
 */

/**
 * Prototype for the general initialization. This is the first application
 * code called. It is called only after a reset, thus in principle once.
 */
void appBoot(void);

/**
 * Tick hook called every tick. Keep code inside really short! API calls inside
 * are not allowed, even the genXXX are not allowed. The tick counter may not be
 * valid when called. This call is executed inside OS realm. In case of low power
 * down, missed ticks are not catched up.
 */
void appTick00(void);

/**
 * Tick hook called every tick block (256 ticks). Keep code inside short. API calls
 * inside are not allowed, even the genXXX are not allowed. The tick counter may not be
 * valid when called. This call is executed inside OS realm. In case of low power
 * down, missed ticks are not catched up.
 */
void appTick08(void);

/**
 * Tick hook called every tick rollover (65536 ticks). Keep code inside short. API calls
 * inside are not allowed, even the genXXX are not allowed. The tick counter may not be
 * valid when called. This call is executed inside OS realm. In case of low power
 * down, missed ticks are catched up, but may be deferred.
 */
void appTick16(void);


/**
 * Loop definitions of the first and subsequent task.
 */
#ifdef TN_00
  void AppLoop(TN_00) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_01
  void AppLoop(TN_01) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_02
  void AppLoop(TN_02) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_03
  void AppLoop(TN_03) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_04
  void AppLoop(TN_04) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_05
  void AppLoop(TN_05) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_06
  void AppLoop(TN_06) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_07
  void AppLoop(TN_07) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_08
  void AppLoop(TN_08) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_09
  void AppLoop(TN_09) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_10
  void AppLoop(TN_10) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_11
  void AppLoop(TN_11) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_12
  void AppLoop(TN_12) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_13
  void AppLoop(TN_13) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_14
  void AppLoop(TN_14) (void) __attribute__ ( ( noreturn ) ) ;
#endif

#ifdef TN_15
  void AppLoop(TN_15) (void) __attribute__ ( ( noreturn ) ) ;
#endif


#endif /* FEMTOOS_CODE_H */
