#pragma once
/* this file is included by cron.h */

/* Convenience Macros to convert time units to cron ticks
 * DO NOT use these functions on non-constant values, since multiplications
 * are slow operations.
 *
 * In case you really have to, at least use the macros in the following
 * manner: Given you want to execute some code every "var" amount of milliseconds,
 * you can do as follows:
 *
 *   Good: cron_add (_CRON_MS(1)*var, ...);
 *   Bad:  cron_add (_CRON_MS(var), ... );
 *
 * Since shift operations are way faster than multiplications, you may as well use
 * macros with the suffica "_CIRCA" - these will perform a shift operation instead
 * of a multiplication.
 */

#include "roundbit.h"


#if CRON_US_PER_TICK < 1000UL /* >=1us per tick */
	#define T_PER_MS (1000UL / CRON_US_PER_TICK)
	#define T_PER_S (1000000UL / CRON_US_PER_TICK)
	#define T_PER_M (60000000UL / CRON_US_PER_TICK)
	#define T_PER_H (3600000000UL / CRON_US_PER_TICK)
#elif CRON_US_PER_TICK < 1000000UL /* 1ms ... 1s per tick */
	#define MS_PER_T (CRON_US_PER_TICK / 1000UL) 
	#define T_PER_S (1000000UL / CRON_US_PER_TICK)
	#define T_PER_M (60000000UL / CRON_US_PER_TICK)
	#define T_PER_H (3600000000UL / CRON_US_PER_TICK)
#elif CRON_US_PER_TICK < 60000000UL /* 1 ... 60s */
	#define MS_PER_T (CRON_US_PER_TICK / 1000UL)
	#define S_PER_T (CRON_US_PER_TICK / 1000000UL)
	#define T_PER_M (60000000UL / CRON_US_PER_TICK)
	#define T_PER_H (3600000000UL / CRON_US_PER_TICK)
#elif CRON_US_PER_TICK < 3600000000UL /* 1m ... 1h */
	#define MS_PER_T (CRON_US_PER_TICK / 1000UL)
	#define S_PER_T (CRON_US_PER_TICK / 1000000UL)
	#define M_PER_T (CRON_US_PER_TICK / 60000000UL)
	#define T_PER_H (3600000000UL / CRON_US_PER_TICK)
#else
	#define MS_PER_T (CRON_US_PER_TICK / 1000UL)
	#define S_PER_T (CRON_US_PER_TICK / 1000000UL)
	#define M_PER_T (CRON_US_PER_TICK / 60000000UL)
	#define H_PER_T (CRON_US_PER_TICK / 3600000000UL)
#endif

#define _CRON_US(a) (a / CRON_US_PER_TICK)

#if (T_PER_MS)
	#define _CRON_MS(a) (a*T_PER_MS)
#elif (MS_PER_T)
	#define _CRON_MS(a) (a/MS_PER_T)
#endif

#if (T_PER_S)
	#define _CRON_S(a) (a*T_PER_S)
#elif (MS_PER_T)
	#define _CRON_S(a) (MS_PER_T / a)
#endif

#if (T_PER_M)
	#define _CRON_M(a) (T_PER_M * a)
#elif (M_PER_T)
	#define _CRON_M(a) (M_PER_T / a)
#endif

#if (T_PER_H)
	#define _CRON_H(a) (T_PER_H * a)
#elif (H_PER_T)
	#define _CRON_H(a) (a/H_PER_T)
#endif
/*
#if CRON_US_PER_TICK < 1000UL
	#define CRON_MS_MUL 1000/CRON_US_PER_TICK
	#define _CRON_US(a) ((a>CRON_US_PER_TICK)?(a/CRON_US_PER_TICK):1)
	#define _CRON_MS(a) (a*CRON_MS_MUL)
	#define _CRON_S(a) ()
#endif
*/
