/* hw_uart_regs.h */
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

#ifndef HW_UART_REGS_H_
#define HW_UART_REGS_H_

#define UART0_BASE 0x4000C000
#define UART1_BASE 0x4000D000
#define UART2_BASE 0x4000E000

#define UARTDR_OFFSET        0x00
#define UARTRSR_OFFSET       0x04
#define UARTECR_OFFSET       0x04
#define UARTFR_OFFSET        0x18
#define UARTILPR_OFFSET      0x20
#define UARTIBRD_OFFSET      0x24
#define UARTFBRD_OFFSET      0x28
#define UARTLCRH_OFFSET      0x2C
#define UARTCTL_OFFSET       0x30
#define UARTIFLS_OFFSET      0x34
#define UARTIM_OFFSET        0x38
#define UARTRIS_OFFSET       0x3C
#define UARTMIS_OFFSET       0x40
#define UARTICR_OFFSET       0x44
#define UARTDMACTL_OFFSET    0x48
#define UARTLCTL_OFFSET      0x90
#define UARTLSS_OFFSET       0x94
#define UARTLTIM_OFFSET      0x98

#define UARTPeriphID4_OFFSET  0x0FD0
#define UARTPeriphID5_OFFSET  0x0FD4
#define UARTPeriphID6_OFFSET  0x0FD8
#define UARTPeriphID7_OFFSET  0x0FDC
#define UARTPeriphID0_OFFSET  0x0FE0
#define UARTPeriphID1_OFFSET  0x0FE4
#define UARTPeriphID2_OFFSET  0x0FE8
#define UARTPeriphID3_OFFSET  0x0FEC

#define UARTCellID0_OFFSET    0x0FF0
#define UARTCellID1_OFFSET    0x0FF4
#define UARTCellID2_OFFSET    0x0FF8
#define UARTCellID3_OFFSET    0x0FFC

/* bits in UARTCTL */
#define UARTEN 0
#define UART_HSE 5
#define UART_TXE 8
#define UART_RXE 9

/* bits in UARTFR */
#define UART_RI   8
#define UART_TXFE 7
#define UART_RXFF 6
#define UART_TXFF 5
#define UART_RXFE 4
#define UART_BUSY 3
#define UART_DCD  2
#define UART_DSR  1
#define UART_CTS  0

/* bits in UARTLCRH */
#define UART_FEN 4

#endif /* HW_UART_REGS_H_ */
