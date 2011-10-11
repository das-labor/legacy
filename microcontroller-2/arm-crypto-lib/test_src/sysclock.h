/* sysclock.h */
/*
    This file is part of the ARM-Crypto-Lib.
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

#ifndef SYSCLOCK_H_
#define SYSCLOCK_H_

#include <stdint.h>

#define SYS_FREQ_80MHZ000    0x04UL
#define SYS_FREQ_66MHZ667    0x05UL
#define SYS_FREQ_57MHZ143    0x06UL
#define SYS_FREQ_50MHZ000    0x07UL
#define SYS_FREQ_44MHZ444    0x08UL
#define SYS_FREQ_40MHZ000    0x09UL
#define SYS_FREQ_36MHZ364    0x0AUL
#define SYS_FREQ_33MHZ333    0x0BUL
#define SYS_FREQ_30MHZ769    0x0CUL
#define SYS_FREQ_28MHZ571    0x0DUL
#define SYS_FREQ_26MHZ667    0x0EUL
#define SYS_FREQ_25MHZ000    0x0FUL
#define SYS_FREQ_23MHZ529    0x10UL
#define SYS_FREQ_22MHZ222    0x11UL
#define SYS_FREQ_21MHZ053    0x12UL
#define SYS_FREQ_20MHZ000    0x13UL
#define SYS_FREQ_19MHZ048    0x14UL
#define SYS_FREQ_18MHZ182    0x15UL
#define SYS_FREQ_17MHZ391    0x16UL
#define SYS_FREQ_16MHZ667    0x17UL
#define SYS_FREQ_16MHZ000    0x18UL
#define SYS_FREQ_15MHZ385    0x19UL
#define SYS_FREQ_14MHZ815    0x1AUL
#define SYS_FREQ_14MHZ286    0x1BUL
#define SYS_FREQ_13MHZ793    0x1CUL
#define SYS_FREQ_13MHZ333    0x1DUL
#define SYS_FREQ_12MHZ903    0x1EUL
#define SYS_FREQ_12MHZ500    0x1FUL
#define SYS_FREQ_12MHZ121    0x20UL
#define SYS_FREQ_11MHZ765    0x21UL
#define SYS_FREQ_11MHZ429    0x22UL
#define SYS_FREQ_11MHZ111    0x23UL
#define SYS_FREQ_10MHZ811    0x24UL
#define SYS_FREQ_10MHZ526    0x25UL
#define SYS_FREQ_10MHZ256    0x26UL
#define SYS_FREQ_10MHZ000    0x27UL
#define SYS_FREQ_9MHZ756    0x28UL
#define SYS_FREQ_9MHZ524    0x29UL
#define SYS_FREQ_9MHZ302    0x2AUL
#define SYS_FREQ_9MHZ091    0x2BUL
#define SYS_FREQ_8MHZ889    0x2CUL
#define SYS_FREQ_8MHZ696    0x2DUL
#define SYS_FREQ_8MHZ511    0x2EUL
#define SYS_FREQ_8MHZ333    0x2FUL
#define SYS_FREQ_8MHZ163    0x30UL
#define SYS_FREQ_8MHZ000    0x31UL
#define SYS_FREQ_7MHZ843    0x32UL
#define SYS_FREQ_7MHZ692    0x33UL
#define SYS_FREQ_7MHZ547    0x34UL
#define SYS_FREQ_7MHZ407    0x35UL
#define SYS_FREQ_7MHZ273    0x36UL
#define SYS_FREQ_7MHZ143    0x37UL
#define SYS_FREQ_7MHZ018    0x38UL
#define SYS_FREQ_6MHZ897    0x39UL
#define SYS_FREQ_6MHZ780    0x3AUL
#define SYS_FREQ_6MHZ667    0x3BUL
#define SYS_FREQ_6MHZ557    0x3CUL
#define SYS_FREQ_6MHZ452    0x3DUL
#define SYS_FREQ_6MHZ349    0x3EUL
#define SYS_FREQ_6MHZ250    0x3FUL
#define SYS_FREQ_6MHZ154    0x40UL
#define SYS_FREQ_6MHZ061    0x41UL
#define SYS_FREQ_5MHZ970    0x42UL
#define SYS_FREQ_5MHZ882    0x43UL
#define SYS_FREQ_5MHZ797    0x44UL
#define SYS_FREQ_5MHZ714    0x45UL
#define SYS_FREQ_5MHZ634    0x46UL
#define SYS_FREQ_5MHZ556    0x47UL
#define SYS_FREQ_5MHZ479    0x48UL
#define SYS_FREQ_5MHZ405    0x49UL
#define SYS_FREQ_5MHZ333    0x4AUL
#define SYS_FREQ_5MHZ263    0x4BUL
#define SYS_FREQ_5MHZ195    0x4CUL
#define SYS_FREQ_5MHZ128    0x4DUL
#define SYS_FREQ_5MHZ063    0x4EUL
#define SYS_FREQ_5MHZ000    0x4FUL
#define SYS_FREQ_4MHZ938    0x50UL
#define SYS_FREQ_4MHZ878    0x51UL
#define SYS_FREQ_4MHZ819    0x52UL
#define SYS_FREQ_4MHZ762    0x53UL
#define SYS_FREQ_4MHZ706    0x54UL
#define SYS_FREQ_4MHZ651    0x55UL
#define SYS_FREQ_4MHZ598    0x56UL
#define SYS_FREQ_4MHZ545    0x57UL
#define SYS_FREQ_4MHZ494    0x58UL
#define SYS_FREQ_4MHZ444    0x59UL
#define SYS_FREQ_4MHZ396    0x5AUL
#define SYS_FREQ_4MHZ348    0x5BUL
#define SYS_FREQ_4MHZ301    0x5CUL
#define SYS_FREQ_4MHZ255    0x5DUL
#define SYS_FREQ_4MHZ211    0x5EUL
#define SYS_FREQ_4MHZ167    0x5FUL
#define SYS_FREQ_4MHZ124    0x60UL
#define SYS_FREQ_4MHZ082    0x61UL
#define SYS_FREQ_4MHZ040    0x62UL
#define SYS_FREQ_4MHZ000    0x63UL
#define SYS_FREQ_3MHZ960    0x64UL
#define SYS_FREQ_3MHZ922    0x65UL
#define SYS_FREQ_3MHZ883    0x66UL
#define SYS_FREQ_3MHZ846    0x67UL
#define SYS_FREQ_3MHZ810    0x68UL
#define SYS_FREQ_3MHZ774    0x69UL
#define SYS_FREQ_3MHZ738    0x6AUL
#define SYS_FREQ_3MHZ704    0x6BUL
#define SYS_FREQ_3MHZ670    0x6CUL
#define SYS_FREQ_3MHZ636    0x6DUL
#define SYS_FREQ_3MHZ604    0x6EUL
#define SYS_FREQ_3MHZ571    0x6FUL
#define SYS_FREQ_3MHZ540    0x70UL
#define SYS_FREQ_3MHZ509    0x71UL
#define SYS_FREQ_3MHZ478    0x72UL
#define SYS_FREQ_3MHZ448    0x73UL
#define SYS_FREQ_3MHZ419    0x74UL
#define SYS_FREQ_3MHZ390    0x75UL
#define SYS_FREQ_3MHZ361    0x76UL
#define SYS_FREQ_3MHZ333    0x77UL
#define SYS_FREQ_3MHZ306    0x78UL
#define SYS_FREQ_3MHZ279    0x79UL
#define SYS_FREQ_3MHZ252    0x7AUL
#define SYS_FREQ_3MHZ226    0x7BUL
#define SYS_FREQ_3MHZ200    0x7CUL
#define SYS_FREQ_3MHZ175    0x7DUL
#define SYS_FREQ_3MHZ150    0x7EUL
#define SYS_FREQ_3MHZ125    0x7FUL


void sysclk_set_rawclock(void);
void sysclk_mosc_verify_enable(void);
void sysclk_mosc_verify_disable(void);
void sysclk_set_80MHz(void);
void sysclk_set_freq(uint8_t freq_id);
uint32_t sysclk_get_freq(void);

#endif /* SYSCLOCK_H_ */
