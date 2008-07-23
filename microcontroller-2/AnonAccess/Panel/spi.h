/* spi.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "config.h"
#ifndef SPI_H
#define SPI_H

/**
 * this enables the use of Hardware SPI on ATMEGA 
 *
 * #define SPI_HARDWARE 1
 * #define SPI_PORT PORTB
 * #define SPI_PIN PINB
 * #define SPI_DDR DDRB
 * 
 * #define SPI_PIN_MOSI PB5
 * #define SPI_PIN_MISO PB6
 * #define SPI_PIN_SCK PB7
 * #define SPI_PIN_SS PB4
 *
 */


void spi_init(void);

unsigned char spi_data(unsigned char c);



#endif
