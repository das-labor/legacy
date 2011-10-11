/* performance_test.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010  Daniel Otte (daniel.otte@rub.de)

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
/*
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * license: GPLv3
 *
 *
 **/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "hw_regs.h"
#include "hw_gptm.h"
#include "cli.h"
#include "string-extras.h"
#include "performance_test.h"


static volatile uint32_t ovfcounter;

static uint32_t const_overhead=0;
static uint32_t int_overhead=0;

void calibrateTimer(void){
	uint64_t t;
	startTimer(1);
	t=stopTimer();
	const_overhead = (uint32_t)t;
}

void initTimer(void){
	gptm_set_timer_32periodic(PERF_TIMER);
}

void startTimer(uint8_t granularity){
	gptm_stop_timer(PERF_TIMER, 0);
	gptm_write_timer(PERF_TIMER, 0, 0);
	if(granularity>0){
		gptm_set_timer_prescaler(PERF_TIMER, 0, granularity-1);
		gptm_start_timer(PERF_TIMER, 0);
	}else{
		gptm_set_timer_prescaler(PERF_TIMER, 0, 0);
	}
}

uint64_t stopTimer(void){
	gptm_stop_timer(PERF_TIMER, 0);
	uint64_t ret;
	ret = gptm_read_timer(PERF_TIMER, 0);
	ret -= const_overhead;
	ret -= ovfcounter * int_overhead;
	return ret;
}

void getOverhead(uint32_t* constoh, uint32_t* intoh){
	*constoh = const_overhead;
	*intoh   = int_overhead;
}

void print_time(const char* s, uint64_t t){
	char sv[22];
	uint8_t c;
	cli_putstr("\r\n");
	cli_putstr(s);
	ulltoa(t, sv, 10);
	for(c=strlen(sv); c<11; ++c){
		cli_putc(' ');
	}
	cli_putstr(sv);
}

void print_overhead(void){
	char str[11];
	cli_putstr("\r\n\r\n=== benchmark ===");
	ultoa(const_overhead, str, 10);
	cli_putstr("\r\n\tconst overhead:     ");
	cli_putstr(str);
	ultoa(int_overhead, str, 10);
	cli_putstr("\r\n\tinterrupt overhead: ");
	cli_putstr(str);
}


