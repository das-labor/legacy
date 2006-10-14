/*
 * Implementation of DS1631/DS1631A/DS1731 driver
 * TODO:
 *  -config-cache(!)
 * 
 */

#include "ds1631.h"

/* important stuff */
ds1631_temp_t	ds1631_get_temp(i2c_addr_t dev){
	return (i2c_get_16bit_reg(dev, Ta_reg));
}

ds1631_temp_t	ds1631_get_max_lim(i2c_addr_t dev){
	return (i2c_get_16bit_reg(dev, Tmax_reg));
}

void				ds1631_set_max_lim(i2c_addr_t dev, ds1631_temp_t temp){
	#ifdef SPARE_EEPROM
		if (ds1631_get_max_lim(dev)==temp)
			return;	
	#endif
		i2c_set_16bit_reg(dev, Tmax_reg, temp);
}

ds1631_temp_t	ds1631_get_min_lim(i2c_addr_t dev){
	return (i2c_get_16bit_reg(dev, Tmin_reg));
}

void				ds1631_set_min_lim(i2c_addr_t dev, ds1631_temp_t temp){
	#ifdef SPARE_EEPROM
		if (ds1631_get_min_lim(dev)==temp)
			return;	
	#endif
		i2c_set_16bit_reg(dev, Tmin_reg, temp);
}

/* some other commands */
void				ds1631_start_conv(i2c_addr_t dev){
	i2c_start_a(dev);
	i2c_sendbyte(DS1631_START_CONVERT);
	i2c_stop();
}

void				ds1631_stop_conv(i2c_addr_t dev){
	i2c_start_a(dev);
	i2c_sendbyte(DS1631_STOP_CONVERT);
	i2c_stop();
}

void				ds1631_por(i2c_addr_t dev){
	i2c_start_a(dev);
	i2c_sendbyte(DS1631_POR);
	i2c_stop();
}

/* configuration stuff (i2c_addr_t dev, also important) */
void				ds1631_set_oneshot(i2c_addr_t dev, bool val){
	i2c_set_bit_in_8bit_reg(dev, conf_reg, DS1631_ONESHOT, val);
}

bool				ds1631_get_oneshot(i2c_addr_t dev){
	return (i2c_get_bit_in_8bit_ret(dev, conf_reg, DS1631_ONESHOT));
}

void				ds1631_set_resolution(i2c_addr_t dev, ds1631_res_t val){
	i2c_set_bits_in_8bit_reg(dev, conf_reg, DS1631_ONESHOT, 2, val);
}

ds1631_res_t		ds1631_get_resolution(i2c_addr_t dev){
	return (i2c_get_bits_in_8bit_reg(dev, conf_reg, DS1631_RESOLUTION, 2));
}

void				ds1631_set_alert_polarity(i2c_addr_t dev, ds1631_alertpol_t val){
	i2c_set_bit_in_8bit_reg(dev, conf_reg, DS1631_ALERTPOL, val);
}
ds1631_alertpol_t	ds1631_get_alert_polarity(i2c_addr_t dev){
	return (i2c_get_bit_in_8bit_ret(dev, conf_reg, DS1631_ALERTPOL));
}

bool				ds1631_get_done(i2c_addr_t dev){
	return (i2c_get_bit_in_8bit_ret(dev, conf_reg, DS1631_DONE));
}

void				ds1631_set_thf(i2c_addr_t dev, bool val){
	i2c_set_bit_in_8bit_reg(dev, conf_reg, DS1631_THF, val);
}

bool				ds1631_get_thf(i2c_addr_t dev){
	return (i2c_get_bit_in_8bit_ret(dev, conf_reg, DS1631_THF));
}

void				ds1631_set_tlf(i2c_addr_t dev, bool val){
	i2c_set_bit_in_8bit_reg(dev, conf_reg, DS1631_TLF, val);
}

bool				ds1631_get_tlf(i2c_addr_t dev){
	return (i2c_get_bit_in_8bit_ret(dev, conf_reg, DS1631_TLF));
}

bool				ds1631_get_nvbusy(i2c_addr_t dev){
	return (i2c_get_bit_in_8bit_ret(dev, conf_reg, DS1631_NVBUSY));
}


