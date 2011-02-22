/* ethernet.c */
/*
    This file is part of the ARM-Crypto-Lib.
    Copyright (C) 2006-2010 Daniel Otte (daniel.otte@rub.de)

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
#include "hw_ethernet_regs.h"
#include "ethernet_defines.h"
#include "sysclock.h"

void ethernet_init(uint8_t* macaddr){
	uint32_t tmp;

	tmp = HW_REG(SYSCTL_BASE+RCGC2_OFFSET);
	tmp |= _BV(RCGC2_EMAC0) | _BV(RCGC2_EPHY0) | _BV(RCGC2_GPIOF);
	HW_REG(SYSCTL_BASE+RCGC2_OFFSET) = tmp;
	HW_REG(SYSCTL_BASE+RCGC2_OFFSET) |= tmp;

	tmp = HW_REG(GPIOF_BASE+GPIO_DEN_OFFSET);
	tmp |= _BV(2) | _BV(3);
	HW_REG(GPIOF_BASE+GPIO_DEN_OFFSET) = tmp;

	tmp = HW_REG(GPIOF_BASE+GPIO_AFSEL_OFFSET);
	tmp |= _BV(2) | _BV(3);
	HW_REG(GPIOF_BASE+GPIO_AFSEL_OFFSET) = tmp;

	tmp = HW_REG(GPIOF_BASE+GPIO_PCTL_OFFSET);
	tmp &= ~((0xf<<(2*4)) | (0xf<<(3*4)));
	tmp |= (1<<(2*4)) | (1<<(3*4));
	HW_REG(GPIOF_BASE+GPIO_PCTL_OFFSET) = tmp;

	tmp = sysclk_get_freq()/(2*2500000);
	HW_REG(ETHERNET0_BASE+ETH_MACMDV_OFFSET) = tmp;

	tmp=(macaddr[2]<<24)|(macaddr[3]<<16)|(macaddr[4]<<8)|(macaddr[5]);
	HW_REG(ETHERNET0_BASE+ETH_MACIA0_OFFSET);
	tmp=(macaddr[0]<<8)|(macaddr[1]);
	HW_REG(ETHERNET0_BASE+ETH_MACIA1_OFFSET);

	HW_REG(ETHERNET0_BASE+ETH_MACIM_OFFSET) = 0;

	HW_REG(ETHERNET0_BASE+ETH_MACTCTL_OFFSET) |= _BV(ETH_DUPLEX) | _BV(ETH_CRC) | _BV(ETH_PADEN);
	HW_REG(ETHERNET0_BASE+ETH_MACRCTL_OFFSET) |= _BV(ETH_AMUL) | _BV(ETH_BADCRC) | _BV(ETH_RSTFIFO);

	HW_REG(ETHERNET0_BASE+ETH_MACTCTL_OFFSET) |= _BV(ETH_TXEN);
	HW_REG(ETHERNET0_BASE+ETH_MACRCTL_OFFSET) |= _BV(ETH_RXEN);
}

void ethernet_tx_packet(uint16_t payload_length, void* packet){
	uint32_t tmp, len;
	tmp = payload_length;

	tmp |= (*((uint16_t*)packet))<<16;
	HW_REG(ETHERNET0_BASE+ETH_MACDATA_OFFSET) = tmp;
	packet = (uint8_t*)packet +2;

	len = payload_length+3*4+3;
	do{
		tmp = *((uint32_t*)packet);
		packet = (uint8_t*)packet +4;
		HW_REG(ETHERNET0_BASE+ETH_MACDATA_OFFSET) = tmp;
	}while((len-=4)>4);
	while((HW_REG(ETHERNET0_BASE+ETH_MACTR_OFFSET)&1) == 0){
	}
	HW_REG(ETHERNET0_BASE+ETH_MACTR_OFFSET) = 1;
}
