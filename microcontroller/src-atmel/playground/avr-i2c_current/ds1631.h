#ifndef DS1631_H_
#define DS1631_H_

/* Driver for DS1631, DS1631A and DS1731
 * i2c thermo sensors
 * 
 */  
/* use
 *  #define SPARE_EEPROM
 * to spare your eeprom.
 * This will make the driver to check on every setting
 * of an EEPROM-Variable if it already contains that value
 */


#include <stdint.h>
#include "i2c.h"
#include "i2c_tools.h"

/* some commands */
#define DS1631_POR				0x54
#define DS1631_START_CONVERT		0x51
#define DS1631_STOP_CONVERT		0x22
#define DS1631_ACCESS_TEMP		0xAA
#define DS1631_ACCESS_THIGH		0xA1
#define DS1631_ACCESS_TLOW		0xA2
#define DS1631_ACCESS_CONFIG		0xAC


/* the bits in the config register */
#define DS1631_DONE			7	// conversation done
#define DS1631_THF			6	// temp was over temp-high
#define DS1631_TLF			5	// temp was below temp-low
#define DS1631_NVBUSY		4 	// NVRam is busy
#define DS1631_RESOLUTION	2	// 2 bit wide resolution
#define DS1631_ALERTPOL		1	// polarity of alert-pin
#define DS1631_ONESHOT		0	// one-shot mode


/* some types */
typedef int16_t ds1631_temp_t;
typedef enum {	Ta_reg=		DS1631_ACCESS_TEMP, 
				conf_reg=	DS1631_ACCESS_CONFIG, 
				Tmin_reg=	DS1631_ACCESS_THIGH,
				Tmax_reg=	DS1631_ACCESS_TLOW} ds1631_reg_t;
typedef enum {	ds1631_9bit=0, 
				ds1631_10bit=1, 
				ds1631_11bit=2, 
				ds1631_12bit=3 } ds1631_res_t; 
typedef enum {active_low=0, active_high=1} ds1631_alertpol_t;


/* important stuff */
ds1631_temp_t	ds1631_get_temp(i2c_addr_t);

ds1631_temp_t	ds1631_get_max_lim(i2c_addr_t);
void				ds1631_set_max_lim(i2c_addr_t, ds1631_temp_t);

ds1631_temp_t	ds1631_get_min_lim(i2c_addr_t);
void				ds1631_set_min_lim(i2c_addr_t, ds1631_temp_t);

/* some other commands */
void				ds1631_start_conv(i2c_addr_t);
void				ds1631_stop_conv(i2c_addr_t);
void				ds1631_por(i2c_addr_t);

/* configuration stuff (i2c_addr_t, also important) */
void				ds1631_set_oneshot(i2c_addr_t, bool val);
bool				ds1631_get_oneshot(i2c_addr_t);

void				ds1631_set_resolution(i2c_addr_t, ds1631_res_t);
ds1631_res_t		ds1631_get_resolution(i2c_addr_t);

void					ds1631_set_alert_polarity(i2c_addr_t, ds1631_alertpol_t);
ds1631_alertpol_t	ds1631_get_alert_polarity(i2c_addr_t);

bool				ds1631_get_done(i2c_addr_t);

void				ds1631_set_thf(i2c_addr_t, bool);
bool				ds1631_get_thf(i2c_addr_t);

void				ds1631_set_tlf(i2c_addr_t, bool);
bool				ds1631_get_tlf(i2c_addr_t);

bool				ds1631_get_nvbusy(i2c_addr_t);



#endif /*DS1631_H_*/
