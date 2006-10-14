#ifndef MCP9800_H_
#define MCP9800_H_

#include <stdint.h>
#include "i2c.h"
#include "i2c_tools.h"

/* the bits in the config register */
#define MCP9800_ONESHOT		7
#define MCP9800_RESOLUTION	5 /* resolution is to bit wide */
#define MCP9800_FAULTQUEUE	3 /* fulat-queue is two bit wide */
#define MCP9800_ALERTPOL		2
#define MCP9800_MODE			1
#define MCP9800_SHUTDOWN		0


/* some types */

typedef int16_t mcp9800_temp_t;
typedef enum {Ta_reg=0, conf_reg=1, Tmin_reg=2, Tmax_reg=3} mcp9800_reg_t;
typedef enum { mcp9800_9bit=0, 
			   mcp9800_10bit=1, 
			   mcp9800_11bit=2, 
			   mcp9800_12bit=3 } mcp9800_res_t; 
typedef enum {active_low=0, active_high=1} mcp9800_alertpol_t;
typedef enum {mcp9800_comp_mode=0, mcp9800_int_mode=1} mcp9800_mode_t;




/* important stuff */
mcp9800_temp_t	mcp9800_get_temp(i2c_addr_t);

mcp9800_temp_t	mcp9800_get_max_lim(i2c_addr_t);
void				mcp9800_set_max_lim(i2c_addr_t, mcp9800_temp_t);

mcp9800_temp_t	mcp9800_get_min_lim(i2c_addr_t);
void				mcp9800_set_min_lim(i2c_addr_t, mcp9800_temp_t);



/* configuration stuff (i2c_addr_t, also important) */
void				mcp9800_set_oneshot(i2c_addr_t, bool val);
bool				mcp9800_get_oneshot(i2c_addr_t);

void				mcp9800_set_resolution(i2c_addr_t, mcp9800_res_t);
mcp9800_res_t	mcp9800_get_resolution(i2c_addr_t);

void				mcp9800_set_alert_polarity(i2c_addr_t, mcp9800_alertpol_t);
mcp9800_alertpol_t	mcp9800_get_alert_polarity(i2c_addr_t);

void				mcp9800_set_mode(i2c_addr_t, mcp9800_mode_t);
mcp9800_mode_t	mcp9800_get_mode(i2c_addr_t);

void				mcp9800_set_shutdown(i2c_addr_t, bool);
bool				mcp9800_get_shutdown(i2c_addr_t);

void				mcp9800_set_faultqueue(i2c_addr_t, uint8_t);
int				mcp9800_get_faultqueue(i2c_addr_t);


#endif /*MCP9800_H_*/
