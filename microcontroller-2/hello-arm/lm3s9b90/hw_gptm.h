/* hw_gpt.h */
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

#ifndef HW_GPT_H_
#define HW_GPT_H_

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3


#define TIMER0_BASE 0x40030000
#define TIMER1_BASE 0x40031000
#define TIMER2_BASE 0x40032000
#define TIMER3_BASE 0x40033000

#define GPTM_CFG_OFFSET            0x000
#define GPTM_TAMR_OFFSET           0x004
#define GPTM_TBMR_OFFSET           0x008
#define GPTM_CTL_OFFSET            0x00C
#define GPTM_IMR_OFFSET            0x018
#define GPTM_RIS_OFFSET            0x01C
#define GPTM_MIS_OFFSET            0x020
#define GPTM_ICR_OFFSET            0x024
#define GPTM_TAILR_OFFSET          0x028
#define GPTM_TBILR_OFFSET          0x02C
#define GPTM_TAMATCHR_OFFSET       0x030
#define GPTM_TBMATCHR_OFFSET       0x034
#define GPTM_TAPR_OFFSET           0x038
#define GPTM_TBPR_OFFSET           0x03C
#define GPTM_TAR_OFFSET            0x048
#define GPTM_TBR_OFFSET            0x04C
#define GPTM_TAV_OFFSET            0x050
#define GPTM_TBV_OFFSET            0x054

#define GPTM_TAEN       0
#define GPTM_TASTALL    1
#define GPTM_TAEVENT    2
#define GPTM_RTCEN      4
#define GPTM_TAOTE      5
#define GPTM_TAPWML     6
#define GPTM_TBEN       8
#define GPTM_TBSTALL    9
#define GPTM_TBEVENT   10
#define GPTM_TBOTE     13
#define GPTM_TBPWML    14

#define GPTM_MODE_ONESHOT  1
#define GPTM_MODE_PERIODIC 2
#define GPTM_MODE_CAPTURE  3



uint32_t gptm_read_timer(uint8_t timerno, uint8_t b);
void gptm_write_timer(uint8_t timerno, uint8_t b, uint32_t value);
void gptm_start_timer(uint8_t timerno, uint8_t b);
void gptm_stop_timer(uint8_t timerno, uint8_t b);
void gptm_set_timer_prescaler(uint8_t timerno, uint8_t b, uint8_t prescaler);
void gptm_set_timer_loadvalue(uint8_t timerno, uint8_t b, uint32_t value);
void gptm_set_timer_mode(uint8_t timerno, uint8_t b, uint8_t mode);
uint8_t gptm_get_timer_running(uint8_t timerno, uint8_t b);
void gptm_set_timer_32periodic(uint8_t timerno);



#endif /* HW_GPT_H_ */
