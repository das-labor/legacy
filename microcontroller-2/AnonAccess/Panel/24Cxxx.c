/* 24Cxxx.c */
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


/* 24Cxxx serial EEPROM Driver
 * 
 * Author: Daniel Otte
 * E-Mail:  Daniel.Otte@rub.de
 * 
 * Driver for Microship's 24AA65/24LC65/24C65
 * 
 * TODO:
 * -documentation
 * -support security features
 */

#include "config.h" 	
#include "i2c.h"
#include "24Cxxx.h"

void  E24C_init(void){
//	i2c_init();
}

void E24C_byte_write (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t data){
// ASSUME	devaddr & 1 == 0
	i2c_start_a(devaddr + I2C_WRITE); 	// should ack
	i2c_sendbyte(HIGH(addr)); 			// should ack
	i2c_sendbyte(LOW(addr));  			// should ack
	i2c_sendbyte(data); 					// should ack
	i2c_stop();
}

void E24C_page_write (i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *data, uint16_t length){
// ASSUME	devaddr & 1 == 0
// max 8 pages a 8 byte!
	uint16_t i;
	i2c_start_a(devaddr + I2C_WRITE);	// should ack
	i2c_sendbyte(HIGH(addr));			// should ack
	i2c_sendbyte(LOW(addr)); 			// should ack
	for (i=0; i<length; ++i){
		i2c_sendbyte(data[i]);			// should ack 
	}
	i2c_stop();
}

uint8_t E24C_cur_byte_read(i2c_addr_t devaddr){
// ASSUME	devaddr & 1 == 1
	uint8_t ret;
	i2c_start_a(devaddr + I2C_READ);	// should ack
	i2c_do_not_ack();
	ret = i2c_readbyte();				// do not ack
	i2c_do_ack();
	i2c_stop();
	return ret;
}

void E24C_cur_block_read(i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *buffer, uint16_t length){
// ASSUME	devaddr & 1 == 0
	uint16_t i;
	i2c_start_a(devaddr + I2C_READ);	// should ack
	i2c_do_ack();
	for (i=0; i<length-1; ++i) {
		buffer[i] = i2c_readbyte(); 	// do ack
	}
	i2c_do_not_ack();
	buffer[length-1] = i2c_readbyte(); 		// do not ack
	i2c_do_ack();
	i2c_stop();
}

uint8_t E24C_byte_read(i2c_addr_t devaddr, E24C_addr_t addr){
// ASSUME	devaddr & 1 == 0
	uint8_t ret;
	i2c_start_a(devaddr + I2C_WRITE);	// should ack
	i2c_sendbyte(HIGH(addr));			// should ack
	i2c_sendbyte(LOW(addr));			// should ack
	i2c_restart_a(devaddr + I2C_READ);	// should ack
	i2c_do_not_ack();
	ret = i2c_readbyte();				// don't ack
	i2c_do_ack();
	i2c_stop();
	return ret;
}

void E24C_block_read(i2c_addr_t devaddr, E24C_addr_t addr, uint8_t *buffer, uint16_t length){
// ASSUME	devaddr & 1 == 0
	uint16_t i;
	i2c_start_a(devaddr + I2C_WRITE);	// should ack
	i2c_sendbyte(HIGH(addr));			// should ack
	i2c_sendbyte(LOW(addr));			// should ack
	i2c_restart_a(devaddr + I2C_READ); //may wait ...
	i2c_do_ack();
	for (i=0; i<length-1; ++i) {
		buffer[i] = i2c_readbyte();		// do ack
	}
	i2c_do_not_ack();
	buffer[length-1] = i2c_readbyte();			// don't ack
	i2c_do_ack();
	i2c_stop();
}

