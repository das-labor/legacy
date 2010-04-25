/* uart_lowlevel.h */
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

#ifndef UART_LOWLEVEL_H_
#define UART_LOWLEVEL_H_


#include <stdint.h>
#include "hw_regs.h"
#include "hw_uart_regs.h"
#include "uart_defines.h"

uint8_t uart_init(uint8_t uartno, uint32_t baudrate, uint8_t databits, uint8_t paraty, uint8_t stopbits);

void uart_putc(uint8_t uartno, uint8_t byte);
uint16_t uart_getc(uint8_t uartno);
uint32_t uart_dataavail(uint8_t uartno);

#endif /* UART_LOWLEVEL_H_ */
