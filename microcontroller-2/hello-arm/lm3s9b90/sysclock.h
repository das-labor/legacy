/* sysclock.h */
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

#ifndef SYSCLOCK_H_
#define SYSCLOCK_H_

#include <stdint.h>

#define SYS_FREQ_80MHZ000    0x4
#define SYS_FREQ_66MHZ667    0x5
#define SYS_FREQ_57MHZ143    0x6
#define SYS_FREQ_50MHZ000    0x7
#define SYS_FREQ_44MHZ444    0x8
#define SYS_FREQ_40MHZ000    0x9
#define SYS_FREQ_36MHZ364    0xA
#define SYS_FREQ_33MHZ333    0xB
#define SYS_FREQ_30MHZ769    0xC
#define SYS_FREQ_28MHZ571    0xD
#define SYS_FREQ_26MHZ667    0xE
#define SYS_FREQ_25MHZ000    0xF
#define SYS_FREQ_23MHZ529    0x10
#define SYS_FREQ_22MHZ222    0x11
#define SYS_FREQ_21MHZ053    0x12
#define SYS_FREQ_20MHZ000    0x13
#define SYS_FREQ_19MHZ048    0x14
#define SYS_FREQ_18MHZ182    0x15
#define SYS_FREQ_17MHZ391    0x16
#define SYS_FREQ_16MHZ667    0x17
#define SYS_FREQ_16MHZ000    0x18
#define SYS_FREQ_15MHZ385    0x19
#define SYS_FREQ_14MHZ815    0x1A
#define SYS_FREQ_14MHZ286    0x1B
#define SYS_FREQ_13MHZ793    0x1C
#define SYS_FREQ_13MHZ333    0x1D
#define SYS_FREQ_12MHZ903    0x1E
#define SYS_FREQ_12MHZ500    0x1F
#define SYS_FREQ_12MHZ121    0x20
#define SYS_FREQ_11MHZ765    0x21
#define SYS_FREQ_11MHZ429    0x22
#define SYS_FREQ_11MHZ111    0x23
#define SYS_FREQ_10MHZ811    0x24
#define SYS_FREQ_10MHZ526    0x25
#define SYS_FREQ_10MHZ256    0x26
#define SYS_FREQ_10MHZ000    0x27
#define SYS_FREQ_9MHZ756    0x28
#define SYS_FREQ_9MHZ524    0x29
#define SYS_FREQ_9MHZ302    0x2A
#define SYS_FREQ_9MHZ091    0x2B
#define SYS_FREQ_8MHZ889    0x2C
#define SYS_FREQ_8MHZ696    0x2D
#define SYS_FREQ_8MHZ511    0x2E
#define SYS_FREQ_8MHZ333    0x2F
#define SYS_FREQ_8MHZ163    0x30
#define SYS_FREQ_8MHZ000    0x31
#define SYS_FREQ_7MHZ843    0x32
#define SYS_FREQ_7MHZ692    0x33
#define SYS_FREQ_7MHZ547    0x34
#define SYS_FREQ_7MHZ407    0x35
#define SYS_FREQ_7MHZ273    0x36
#define SYS_FREQ_7MHZ143    0x37
#define SYS_FREQ_7MHZ018    0x38
#define SYS_FREQ_6MHZ897    0x39
#define SYS_FREQ_6MHZ780    0x3A
#define SYS_FREQ_6MHZ667    0x3B
#define SYS_FREQ_6MHZ557    0x3C
#define SYS_FREQ_6MHZ452    0x3D
#define SYS_FREQ_6MHZ349    0x3E
#define SYS_FREQ_6MHZ250    0x3F
#define SYS_FREQ_6MHZ154    0x40
#define SYS_FREQ_6MHZ061    0x41
#define SYS_FREQ_5MHZ970    0x42
#define SYS_FREQ_5MHZ882    0x43
#define SYS_FREQ_5MHZ797    0x44
#define SYS_FREQ_5MHZ714    0x45
#define SYS_FREQ_5MHZ634    0x46
#define SYS_FREQ_5MHZ556    0x47
#define SYS_FREQ_5MHZ479    0x48
#define SYS_FREQ_5MHZ405    0x49
#define SYS_FREQ_5MHZ333    0x4A
#define SYS_FREQ_5MHZ263    0x4B
#define SYS_FREQ_5MHZ195    0x4C
#define SYS_FREQ_5MHZ128    0x4D
#define SYS_FREQ_5MHZ063    0x4E
#define SYS_FREQ_5MHZ000    0x4F
#define SYS_FREQ_4MHZ938    0x50
#define SYS_FREQ_4MHZ878    0x51
#define SYS_FREQ_4MHZ819    0x52
#define SYS_FREQ_4MHZ762    0x53
#define SYS_FREQ_4MHZ706    0x54
#define SYS_FREQ_4MHZ651    0x55
#define SYS_FREQ_4MHZ598    0x56
#define SYS_FREQ_4MHZ545    0x57
#define SYS_FREQ_4MHZ494    0x58
#define SYS_FREQ_4MHZ444    0x59
#define SYS_FREQ_4MHZ396    0x5A
#define SYS_FREQ_4MHZ348    0x5B
#define SYS_FREQ_4MHZ301    0x5C
#define SYS_FREQ_4MHZ255    0x5D
#define SYS_FREQ_4MHZ211    0x5E
#define SYS_FREQ_4MHZ167    0x5F
#define SYS_FREQ_4MHZ124    0x60
#define SYS_FREQ_4MHZ082    0x61
#define SYS_FREQ_4MHZ040    0x62
#define SYS_FREQ_4MHZ000    0x63
#define SYS_FREQ_3MHZ960    0x64
#define SYS_FREQ_3MHZ922    0x65
#define SYS_FREQ_3MHZ883    0x66
#define SYS_FREQ_3MHZ846    0x67
#define SYS_FREQ_3MHZ810    0x68
#define SYS_FREQ_3MHZ774    0x69
#define SYS_FREQ_3MHZ738    0x6A
#define SYS_FREQ_3MHZ704    0x6B
#define SYS_FREQ_3MHZ670    0x6C
#define SYS_FREQ_3MHZ636    0x6D
#define SYS_FREQ_3MHZ604    0x6E
#define SYS_FREQ_3MHZ571    0x6F
#define SYS_FREQ_3MHZ540    0x70
#define SYS_FREQ_3MHZ509    0x71
#define SYS_FREQ_3MHZ478    0x72
#define SYS_FREQ_3MHZ448    0x73
#define SYS_FREQ_3MHZ419    0x74
#define SYS_FREQ_3MHZ390    0x75
#define SYS_FREQ_3MHZ361    0x76
#define SYS_FREQ_3MHZ333    0x77
#define SYS_FREQ_3MHZ306    0x78
#define SYS_FREQ_3MHZ279    0x79
#define SYS_FREQ_3MHZ252    0x7A
#define SYS_FREQ_3MHZ226    0x7B
#define SYS_FREQ_3MHZ200    0x7C
#define SYS_FREQ_3MHZ175    0x7D
#define SYS_FREQ_3MHZ150    0x7E
#define SYS_FREQ_3MHZ125    0x7F


void sysclk_set_rawclock(void);
void sysclk_mosc_verify_enable(void);
void sysclk_mosc_verify_disable(void);
void sysclk_set_80MHz(void);
void sysclk_set_freq(uint8_t freq_id);
uint32_t sysclk_get_freq(void);

#endif /* SYSCLOCK_H_ */
