/* sysclock.c */
/*
    This file is part of the OpenARMWare.
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
#include "sysclock.h"

#define CRYSTAL_FREQ 16000000UL
#define CRYSTAL_CODE 0x15 /* 16 MHz */

#define PIOSC_FREQ 16000000UL

void sysclk_set_rawclock(void){
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

void sysclk_set_freq(uint8_t freq_id){
	uint32_t rcc1, rcc2=0;
	sysclk_set_rawclock();
	rcc1 = HW_REG(SYSCTL_BASE+RCC_OFFSET);
//	rcc2 = HW_REG(SYSCTL_BASE+RCC2_OFFSET);
	rcc1 &= ~(0x1f<<RCC_XTAL);
	rcc1 |= CRYSTAL_CODE<<RCC_XTAL;
	rcc2 = _BV(RCC2_USERCC2) | _BV(RCC2_PWRDN2) | _BV(RCC2_BYPASS2) | _BV(RCC2_USBPWRDN); /* OSCSRC2 is set to 0 */
	HW_REG(SYSCTL_BASE+RCC_OFFSET)  = rcc1;
	HW_REG(SYSCTL_BASE+RCC2_OFFSET) = rcc2;
	rcc2 &= ~_BV(RCC2_PWRDN2);
	HW_REG(SYSCTL_BASE+RCC2_OFFSET) = rcc2;
	rcc2 |= _BV(RCC2_DIV400) | freq_id<<RCC2_SYSDIV2LSB;
	HW_REG(SYSCTL_BASE+RCC2_OFFSET) = rcc2;
	while(!(HW_REG(SYSCTL_BASE+RIS_OFFSET)&_BV(RIS_PLLLRIS))){
	}
	rcc2 &= ~_BV(RCC2_BYPASS2);
	HW_REG(SYSCTL_BASE+RCC2_OFFSET) = rcc2;
}

void sysclk_set_80MHz(void){
	sysclk_set_freq(SYS_FREQ_80MHZ000);
}

uint32_t sysclk_get_freq(void){
	uint32_t rcc1, rcc2, basefreq=400000000UL, divider=1;
	const uint32_t bypass_freq[] = {
			CRYSTAL_FREQ, PIOSC_FREQ,  PIOSC_FREQ/4, 30000,
			0, 0, 4194304, 32768 };
	rcc1 = HW_REG(SYSCTL_BASE+RCC_OFFSET);
	rcc2 = HW_REG(SYSCTL_BASE+RCC2_OFFSET);
	if(rcc2&_BV(RCC2_USERCC2)){
		/* use RCC2 */
		if(rcc2&_BV(RCC2_BYPASS2)){
			basefreq = bypass_freq[(rcc2>>RCC2_OSCSR2)&0x07];
		}
		if(rcc2&_BV(RCC2_DIV400)){
			divider = ((rcc2>>RCC2_SYSDIV2LSB)&0x7F)+1;
		}else{
			divider = ((rcc2>>RCC2_SYSDIV2)&0x3F)+1;
		}
	}else{
		/* use RCC */
		if(rcc1&_BV(RCC_BYPASS)){
			 basefreq = bypass_freq[(rcc1>>RCC_OSCSRC)&0x03];
		}
		divider = ((rcc1>>RCC_SYSDIV)&0xf)+1;
	}
	return basefreq/divider;
}
