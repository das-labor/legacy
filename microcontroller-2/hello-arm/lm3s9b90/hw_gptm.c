/* hw_gptm.c */
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

#include <stdint.h>
#include "hw_regs.h"
#include "hw_gptm.h"

static const
uint32_t timer_base[] = { TIMER0_BASE, TIMER1_BASE, TIMER2_BASE, TIMER3_BASE };

uint32_t gptm_read_timer(uint8_t timerno, uint8_t b){
	if(timerno>3){
		return 0;
	}
	return HW_REG(timer_base[timerno]+(b?GPTM_TBV_OFFSET:GPTM_TAV_OFFSET));
}

void gptm_write_timer(uint8_t timerno, uint8_t b, uint32_t value){
	if(timerno>3){
		return;
	}
	HW_REG(timer_base[timerno]+(b?GPTM_TBV_OFFSET:GPTM_TAV_OFFSET)) = value;
}

void gptm_start_timer(uint8_t timerno, uint8_t b){
	if(timerno>3){
		return;
	}
	HW_REG(timer_base[timerno]+GPTM_CTL_OFFSET) |= _BV(b?GPTM_TBEN:GPTM_TAEN);
}

void gptm_stop_timer(uint8_t timerno, uint8_t b){
	if(timerno>3){
		return;
	}
	HW_REG(timer_base[timerno]+GPTM_CTL_OFFSET) &= ~_BV(b?GPTM_TBEN:GPTM_TAEN);
}

void gptm_set_timer_prescaler(uint8_t timerno, uint8_t b, uint8_t prescaler){
	if(timerno>3){
		return;
	}
	HW_REG(timer_base[timerno]+(b?GPTM_TBPR_OFFSET:GPTM_TAPR_OFFSET)) =
			prescaler;
}

void gptm_set_timer_loadvalue(uint8_t timerno, uint8_t b, uint32_t value){
	if(timerno>3){
		return;
	}
	HW_REG(timer_base[timerno]+(b?GPTM_TBILR_OFFSET:GPTM_TAILR_OFFSET)) =
			value;
}

void gptm_set_timer_mode(uint8_t timerno, uint8_t b, uint8_t mode){
	if(timerno>3){
		return;
	}
	uint32_t tmp;
	tmp = HW_REG(timer_base[timerno]+(b?GPTM_TBMR_OFFSET:GPTM_TAMR_OFFSET));
	tmp &= ~3;
	tmp |= mode;
	HW_REG(timer_base[timerno]+(b?GPTM_TBMR_OFFSET:GPTM_TAMR_OFFSET)) = tmp;
}

uint8_t gptm_get_timer_running(uint8_t timerno, uint8_t b){
	if(timerno>3){
		return 0;
	}
	return (HW_REG(timer_base[timerno]+GPTM_CTL_OFFSET)>>(b?GPTM_TBEN:GPTM_TAEN))&1;
}


void gptm_set_timer_32periodic(uint8_t timerno){
	if(timerno>3){
		return;
	}
	volatile uint8_t i;
	HW_REG(SYSCTL_BASE+RCGC1_OFFSET) |= _BV(16+timerno);
	for(i=8; i>0; --i)
		;
	gptm_stop_timer(timerno, 0);
	HW_REG(timer_base[timerno]+GPTM_CTL_OFFSET) = 0;
	gptm_set_timer_mode(timerno, 0, GPTM_MODE_PERIODIC);
	HW_REG(timer_base[timerno]+GPTM_TAMR_OFFSET) |= _BV(4);
	gptm_set_timer_loadvalue(timerno, 0, 0xffffffff);
}


