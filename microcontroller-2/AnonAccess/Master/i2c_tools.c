/* i2c_tools.c */
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


/* keep */
#include "config.h"
#include "i2c_tools.h"
#include "debug.h"


uint8_t		i2c_get_8bit_reg(i2c_addr_t dev, i2c_reg_t reg){
	uint8_t ret;
	i2c_start_a(dev+I2C_WRITE);
	i2c_sendbyte(reg);
	i2c_restart_a(dev+I2C_READ);
	i2c_do_not_ack();
	ret = i2c_readbyte();
	i2c_do_ack();
	i2c_stop();
	return (ret);
}


uint16_t		i2c_get_16bit_reg(i2c_addr_t dev, i2c_reg_t reg){
	uint16_t ret;
	i2c_start_a(dev+I2C_WRITE);
	i2c_sendbyte(reg);
	i2c_restart_a(dev+I2C_READ);
	((uint8_t*)(&ret))[1] = i2c_readbyte();	// high byte
	i2c_do_not_ack();
	((uint8_t*)(&ret))[0] = i2c_readbyte(); // low byte
	i2c_do_ack();
	i2c_stop();
	return (ret);
}

void			i2c_set_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t data){
	i2c_start_a(dev+I2C_WRITE);
	i2c_sendbyte(reg);
	i2c_sendbyte(data);
	i2c_stop();
}

void			i2c_set_16bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint16_t data){
	i2c_start_a(dev+I2C_WRITE);
	i2c_sendbyte(reg);
	i2c_sendbyte(HIGH(data));
	i2c_sendbyte(LOW(data));
	i2c_stop();
}

/* advanced functions */

void			i2c_set_bit_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr, bool val){
	uint8_t temp;	// we could do this without temp, but it would look horrible
	temp = i2c_get_8bit_reg(dev, reg);
	temp = (temp & (~(1 << bitaddr))) | (val << bitaddr); 
	i2c_set_8bit_reg(dev, reg, temp);
}

bool			i2c_get_bit_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr){
	return ((i2c_get_8bit_reg(dev, reg) >> bitaddr) & 1 );
}

void			i2c_set_bits_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr, uint8_t bitcnt, uint8_t val){
	uint8_t temp;	// we could do this without temp, but it would look horrible
	temp = i2c_get_8bit_reg(dev, reg);
	temp = (temp & (~(SET_N_BITS(bitcnt) << bitaddr))) | (val << bitaddr); // this is evil; explain it!
	i2c_set_8bit_reg(dev, reg, temp);
}

uint8_t		i2c_get_bits_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr, uint8_t bitcnt){
	return ((i2c_get_8bit_reg(dev, reg) >> bitaddr) & SET_N_BITS(bitcnt) );
}


void			i2c_ack_poll(i2c_addr_t dev){
	uint8_t state;
	do {
		state=i2c_start_a(dev)& 0xF8;
//		DEBUG_S("polling: ");
//		DEBUG_B(state);
//		DEBUG_S("\r\n");
	} while (!((state==0x18) || (state==0x40)));
	i2c_stop();
}

