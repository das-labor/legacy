/* hw_ethernet_regs.h */
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

#ifndef HW_ETHERNET_REGS_H_
#define HW_ETHERNET_REGS_H_

#define ETHERNET0_BASE  0x40048000

#define ETH_MACRIS_OFFSET           0x000
#define ETH_MACIM_OFFSET            0x004
#define ETH_MACRCTL_OFFSET          0x008
#define ETH_MACTCTL_OFFSET          0x00C
#define ETH_MACDATA_OFFSET          0x010
#define ETH_MACIA0_OFFSET           0x014
#define ETH_MACIA1_OFFSET           0x018
#define ETH_MACTHR_OFFSET           0x01C
#define ETH_MACMCTL_OFFSET          0x020
#define ETH_MACMDV_OFFSET           0x024
#define ETH_MACMTXD_OFFSET          0x02C
#define ETH_MACMRXD_OFFSET          0x030
#define ETH_MACNP_OFFSET            0x034
#define ETH_MACTR_OFFSET            0x038
#define ETH_MACLED_OFFSET           0x040
#define ETH_MDIX_OFFSET             0x044

#define ETH_RXEN                    0
#define ETH_AMUL                    1
#define ETH_PRMS                    2
#define ETH_BADCRC                  3
#define ETH_RSTFIFO                 4

#define ETH_TXEN                    0
#define ETH_PADEN                   1
#define ETH_CRC                     2
#define ETH_DUPLEX                  4

#endif /* HW_ETHERNET_REGS_H_ */
