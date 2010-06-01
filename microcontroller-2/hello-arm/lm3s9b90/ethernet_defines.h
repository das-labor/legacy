/* ethernet_defines.h */
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

#ifndef ETHERNET_DEFINES_H_
#define ETHERNET_DEFINES_H_
/*
0x0     Link OK
0x1     RX or TX Activity (Default LED1)
0x2-0x4 Reserved
0x5     100BASE-TX mode
0x6     10BASE-T mode
0x7     Full-Duplex
0x8     Link OK & Blink=RX or TX Activity
0x9-0xF Reserved
*/

#define ETH_LED_LINKOK     0x00
#define ETH_LED_ACTIVITY   0x01
#define ETH_LED_MODE100    0x05
#define ETH_LED_MODE10     0x06
#define ETH_LED_FULLDPLX   0x07
#define ETH_LED_LINKACT    0x08

#endif /* ETHERNET_DEFINES_H_ */
