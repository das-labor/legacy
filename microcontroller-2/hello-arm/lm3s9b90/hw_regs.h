/* hw_regs.h */
/*
    This file is part of the AVR-Crypto-Lib.
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

#ifndef HW_REGS_H_
#define HW_REGS_H_

#include <stdint.h>

#define SET_REG(r,v) (*((volatile uint32_t*)(r))) = (v)
#define GET_REG(r)   (*((volatile uint32_t*)(r)))
#define HW_REG(r)    (*((volatile uint32_t*)(r)))
#define _BV(x)       (1<<(x))

#define SYSCTL_BASE 0x400FE000
#define DID0_OFFSET           0x000
#define DID1_OFFSET           0x004
#define DC0_OFFSET            0x008
#define DC1_OFFSET            0x010
#define DC2_OFFSET            0x014
#define DC3_OFFSET            0x018
#define DC4_OFFSET            0x01C
#define DC5_OFFSET            0x020
#define DC6_OFFSET            0x024
#define DC7_OFFSET            0x028
#define DC8_OFFSET            0x02C
#define PBORCTL_OFFSET        0x030
#define SRCR0_OFFSET          0x040
#define SRCR1_OFFSET          0x044
#define SRCR2_OFFSET          0x048
#define RIS_OFFSET            0x050
#define IMC_OFFSET            0x054
#define MISC_OFFSET           0x058
#define RESC_OFFSET           0x05C
#define RCC_OFFSET            0x060
#define PLLCFG_OFFSET         0x064
#define GPIOHBCTL_OFFSET      0x06C
#define RCC2_OFFSET           0x070
#define MOSCCTL_OFFSET        0x07C
#define RCGC0_OFFSET          0x100
#define RCGC1_OFFSET          0x104
#define RCGC2_OFFSET          0x108
#define SCGC0_OFFSET          0x110
#define SCGC1_OFFSET          0x114
#define SCGC2_OFFSET          0x118
#define DCGC0_OFFSET          0x120
#define DCGC1_OFFSET          0x124
#define DCGC2_OFFSET          0x128
#define DSLPCLKCFG_OFFSET     0x144
#define PIOSCCAL_OFFSET       0x150
#define PIOSCSTAT_OFFSET      0x154
#define I2SMCLKCFG_OFFSET     0x170
#define DC9_OFFSET            0x190
#define NVMSTAT_OFFSET        0x1A0

#define RCC_ACG       27
#define RCC_SYSDIV    23
#define RCC_USESYSDIV 22
#define RCC_PWRDN     13
#define RCC_BYPASS    11
#define RCC_XTAL       6
#define RCC_OSCSRC     4
#define RCC_IOSCDIS    1
#define RCC_MOSCDIS    0

#define RCC2_USERCC2     31
#define RCC2_DIV400      30
#define RCC2_SYSDIV2     23
#define RCC2_SYSDIV2LSB  22
#define RCC2_USBPWRDN    14
#define RCC2_PWRDN2      13
#define RCC2_BYPASS2     11
#define RCC2_OSCSR2       4

#define RIS_MOSCPUPRIS   8
#define RIS_USBPLLLRIS   7
#define RIS_PLLLRIS      6
#define RIS BORRIS       1

#define GPIOA  0
#define GPIOB  1
#define GPIOC  2
#define GPIOD  3
#define GPIOE  4
#define GPIOF  5
#define GPIOG  6
#define GPIOH  7
#define GPIOJ  8

#define GPIOA_BASE  0x40004000
#define GPIOB_BASE  0x40005000
#define GPIOC_BASE  0x40006000
#define GPIOD_BASE  0x40007000
#define GPIOE_BASE  0x40024000
#define GPIOF_BASE  0x40025000
#define GPIOG_BASE  0x40026000
#define GPIOH_BASE  0x40027000
#define GPIOJ_BASE  0x4003D000

#define GPIO_DATA_OFFSET         0x000
#define GPIO_DIR_OFFSET          0x400
#define GPIO_IS_OFFSET           0x404
#define GPIO_IBE_OFFSET          0x408
#define GPIO_IEV_OFFSET          0x40C
#define GPIO_IM_OFFSET           0x410
#define GPIO_RIS_OFFSET          0x414
#define GPIO_MIS_OFFSET          0x418
#define GPIO_ICR_OFFSET          0x41C
#define GPIO_AFSEL_OFFSET        0x420
#define GPIO_DR2R_OFFSET         0x500
#define GPIO_DR4R_OFFSET         0x504
#define GPIO_DR8R_OFFSET         0x508
#define GPIO_ODR_OFFSET          0x50C
#define GPIO_PUR_OFFSET          0x510
#define GPIO_PDR_OFFSET          0x514
#define GPIO_SLR_OFFSET          0x518
#define GPIO_DEN_OFFSET          0x51C
#define GPIO_LOCK_OFFSET         0x520
#define GPIO_CR_OFFSET           0x524
#define GPIO_AMSEL_OFFSET        0x528
#define GPIO_PCTL_OFFSET         0x52C
#define GPIO_PeriphID4_OFFSET    0xFD0
#define GPIO_PeriphID5_OFFSET    0xFD4
#define GPIO_PeriphID6_OFFSET    0xFD8
#define GPIO_PeriphID7_OFFSET    0xFDC
#define GPIO_PeriphID0_OFFSET    0xFE0
#define GPIO_PeriphID1_OFFSET    0xFE4
#define GPIO_PeriphID2_OFFSET    0xFE8
#define GPIO_PeriphID3_OFFSET    0xFEC
#define GPIO_PCellID0_OFFSET     0xFF0
#define GPIO_PCellID1_OFFSET     0xFF4
#define GPIO_PCellID2_OFFSET     0xFF8
#define GPIO_PCellID3_OFFSET     0xFFC


#define ISR_ENABLE_VECTOR    0xE000E100

#endif /* HW_REGS_H_ */
