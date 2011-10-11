/* performance_test.h */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PERFORMANCE_TEST_H_
#define PERFORMANCE_TEST_H_

#include <stdint.h>
#include "hw_gptm.h"

#define PERF_TIMER TIMER0
#define PERF_TIMER_BASE TIMER0_BASE

#define START_TIMER  gptm_start_timer(PERF_TIMER, 0) /* HW_REG(PERF_TIMER_BASE+GPTM_CTL_OFFSET) |= _BV(GPTM_TAEN) */
#define STOP_TIMER   gptm_stop_timer(PERF_TIMER, 0) /* HW_REG(PERF_TIMER_BASE+GPTM_CTL_OFFSET) &= ~_BV(GPTM_TAEN) */

void calibrateTimer(void);
void startTimer(uint8_t granularity);
uint64_t stopTimer(void);
void getOverhead(uint32_t* constoh, uint32_t* intoh);

void print_time_P(const char* s, uint64_t t);
void print_overhead(void);

#endif /*PERFORMANCE_TEST_H_*/
