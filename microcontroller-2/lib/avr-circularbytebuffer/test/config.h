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

#define UART0_NI 1 /* set this to '1' if you want to use the first uart, otherwise set to '0' */
#define UART0_BAUD_RATE 38400 /* baudrate for uart0 */
#define UART0_STOPBITS  UART_STOPBITS_1 /* stopbit configuration for uart0 */
#define UART0_DATABITS  UART_DATABITS_8 /* databit configuration for uart0 */
#define UART0_PARATY    UART_PARATY_NONE /* paraty configuration for uart0 */


#define UART1_NI 0 /* set this to '1' if you want to use the second uart, otherwise set to '0' */
#define UART1_BAUD_RATE 38400 /* baudrate for uart1 */
#define UART1_STOPBITS  UART_STOPBITS_1 /* stopbit configuration for uart1 */
#define UART1_DATABITS  UART_DATABITS_8 /* databit configuration for uart1 */
#define UART1_PARATY    UART_PARATY_NONE /* paraty configuration for uart1 */

#define CIRCULARBYTEBUFFER_NO_MALLOC 1
#define CIRCULARBYTEBUFFER_NO_INIT2  0


#endif /* CONFIG_H_ */

