/* uart_defines.h */
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

#ifndef UART_DEFINES_H_
#define UART_DEFINES_H_

#define UART_0 0
#define UART_1 1
#define UART_2 2
#define UART_MAX 2

#define UART_DATABITS_5 0
#define UART_DATABITS_6 1
#define UART_DATABITS_7 2
#define UART_DATABITS_8 3

#define UART_PARATY_NONE  0
#define UART_PARATY_EVEN  1
#define UART_PARATY_ODD   2
#define UART_PARATY_MARK  3
#define UART_PARATY_SPACE 4

#define UART_STOPBITS_ONE 0
#define UART_STOPBITS_TWO 1

#define UART_ERROR_OK              0
#define UART_ERROR_WRONG_UART      1
#define UART_ERROR_WRONG_DATABITS  2
#define UART_ERROR_WRONG_PARATY    3
#define UART_ERROR_WRONG_STOPBITS  4
#define UART_ERROR_RX_BUFFER_INIT  5
#define UART_ERROR_TX_BUFFER_INIT  6

#endif /* UART_DEFINES_H_ */
