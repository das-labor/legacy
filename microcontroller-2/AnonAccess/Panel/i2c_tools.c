/* keep */
#include "config.h"
#include "i2c_tools.h"

uint8_t	i2c_get_8bit_reg(i2c_addr_t dev, i2c_reg_t reg){
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


uint16_t i2c_get_16bit_reg(i2c_addr_t dev, i2c_reg_t reg){
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

void i2c_set_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t data){
	i2c_start_a(dev+I2C_WRITE);
	i2c_sendbyte(reg);
	i2c_sendbyte(data);
	i2c_stop();
}

void i2c_set_16bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint16_t data){
	i2c_start_a(dev+I2C_WRITE);
	i2c_sendbyte(reg);
	i2c_sendbyte(HIGH(data));
	i2c_sendbyte(LOW(data));
	i2c_stop();
}

/* advanced functions */

void i2c_set_bit_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr, bool val){
	uint8_t temp;	// we could do this without temp, but it would look horrible
	temp = i2c_get_8bit_reg(dev, reg);
	temp = (temp & (~(1 << bitaddr))) | (val << bitaddr); 
	i2c_set_8bit_reg(dev, reg, temp);
}

bool i2c_get_bit_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr){
	return ((i2c_get_8bit_reg(dev, reg) >> bitaddr) & 1 );
}

void i2c_set_bits_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr, uint8_t bitcnt, uint8_t val){
	uint8_t temp;	// we could do this without temp, but it would look horrible
	temp = i2c_get_8bit_reg(dev, reg);
	temp = (temp & (~(SET_N_BITS(bitcnt) << bitaddr))) | (val << bitaddr); // this is evil; explain it!
	i2c_set_8bit_reg(dev, reg, temp);
}

uint8_t i2c_get_bits_in_8bit_reg(i2c_addr_t dev, i2c_reg_t reg, uint8_t bitaddr, uint8_t bitcnt){
	return ((i2c_get_8bit_reg(dev, reg) >> bitaddr) & SET_N_BITS(bitcnt) );
}


void i2c_ack_poll(i2c_addr_t dev){
	uint8_t state;
	do {
		state=i2c_start_a(dev)& 0xF8;
	} while (!((state==0x18) || (state==0x40)));
	i2c_stop();
}

