/* uart_ni.h */
/*
    This file is part of the AVR-uart_ni.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

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
#ifndef UART_NI_H_
#define UART_NI_H_

#include "config.h"
#include <stdint.h>

void uart0_init(void);

void uart0_putc(unsigned char c);

unsigned char uart0_getc(void);

#endif /* UART_NI_H_ */
