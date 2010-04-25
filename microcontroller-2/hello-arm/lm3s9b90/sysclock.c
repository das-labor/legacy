/* sysclock.c */
/*
    This file is part of the OpenARMWare.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

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

void sysclk_setrawclock(void){
	uint32_t tmp_rcc;
	tmp_rcc = 0; //HW_REG(SYSCTL_BASE+RCC_OFFSET);
	tmp_rcc &= ~(_BV(RCC_IOSCDIS) | _BV(RCC_MOSCDIS) | _BV(RCC_USESYSDIV));
	tmp_rcc |= _BV(RCC_BYPASS) | _BV(RCC_PWRDN);
	tmp_rcc &= ~(3<<RCC_OSCSRC);
	tmp_rcc |=  (0<<RCC_OSCSRC);
	HW_REG(SYSCTL_BASE+RCC_OFFSET) = tmp_rcc;
	HW_REG(SYSCTL_BASE+RCC2_OFFSET) &= ~(_BV(31));

}

void sysclk_mosc_verify_enable(void){
	HW_REG(SYSCTL_BASE+MOSCCTL_OFFSET) |= 1; // turn on main oscillator verify circuit
}

void sysclk_mosc_verify_disable(void){
	HW_REG(SYSCTL_BASE+MOSCCTL_OFFSET) &= ~1UL; // turn on main oscillator verify circuit
}
