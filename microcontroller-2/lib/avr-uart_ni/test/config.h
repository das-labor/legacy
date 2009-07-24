/* config.h */
/*
    This file is part of the AVR-Crypto-Lib.
    Copyright (C) 2008  Daniel Otte (daniel.otte@rub.de)

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
#ifndef CONFIG_H_
#define CONFIG_H_
#include "uart_defs.h"

#define F_CPU 16000000

#define UART0 1
#define UART0_BAUD_RATE 38400
#define UART0_STOPBITS  UART_STOPBITS_1
#define UART0_DATABITS  UART_DATABITS_8
#define UART0_PARATY    UART_PARATY_NONE

#define UART1 0
#define UART1_BAUD_RATE 38400
#define UART1_STOPBITS  UART_STOPBITS_2
#define UART1_DATABITS  UART_DATABITS_9
#define UART1_PARATY    UART_PARATY_EVEN



#endif /* CONFIG_H_ */

