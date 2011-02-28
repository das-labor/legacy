/*
 TIMER0 CAP0 -> Enable Timer2(H_SYNC_WAIT) -> call setupTimer2for h_pixel_clk
 TIMER0 CAP1 -> Enable Timer3(V_SYNC_WAIT) -> call setupTimer3for v_pixel_clk

 */
#include "types.h"
#include "lpc2103.h"
#include "config.h"
#include "timer.h"
#include "videoengine.h"
#include "ICcom.h"

void ve_calc_h_pixel_clk(void)
{
	uint32_t i=0;
	uint32_t j=0;
	
	if(ve.frame_time_avr > 0){
		i=(ve.h_line_time_avr * ve.h_pixel_perc)/100;
		j=(ve.h_line_time_avr * ve.h_sync_wait_perc)/100;
		
		ve.h_sync_wait_pre=0;
		if(j > 0xffff){
			ve.h_sync_wait_pre=0xf;
			ve.h_sync_wait=j/0xf;
		}
		if(j > 0xfffff){
			ve.h_sync_wait_pre=0xff;
			ve.h_sync_wait=j/0xff;
		}
		
		if((i/ve.h_pixel_end_val) <= 0xffff)
			ve.h_pixel_clk_pre=(i/ve.h_pixel_end_val);
		else
			ve.h_pixel_clk_pre=0xffff;
	}
}

void ve_calc_v_pixel_clk(void)
{
	uint32_t i=0;
	uint32_t j=0;
	
	if(ve.frame_time_avr > 0){
		i=(ve.frame_time_avr * ve.v_pixel_perc)/100;
		j=(ve.frame_time_avr * ve.v_sync_wait_perc)/100;
		
		ve.v_sync_wait_pre=0;
		/*if(j > 0xffff){
			ve.v_sync_wait_pre=0xf;
			ve.v_sync_wait=j/0xf;
		}
		if(j > 0xfffff){
			ve.v_sync_wait_pre=0xff;
			ve.v_sync_wait=j/0xff;
		}*/
		
		if((i/ve.v_pixel_end_val) <= 0xffff)
			ve.v_pixel_clk_pre=(i/ve.v_pixel_end_val);
		else
			ve.v_pixel_clk_pre=0xffff;
		
	}
}

void ve_set_new_resolution_h(uint16_t res_h)
{
	if((res_h > 32)){
		if(res_h * ve.v_pixel_end_val < VIDEO_BUFFER_SIZE)
		{
			ve.h_pixel_end_val = res_h;
		}
	}
	else
	{
		ve.h_pixel_end_val = 32;
	}
}

void ve_set_new_resolution_v(uint16_t res_v)
{
	if((res_v > 32)){
		if(res_v * ve.h_pixel_end_val < VIDEO_BUFFER_SIZE)
		{
			ve.v_pixel_end_val = res_v;
		}
	}
	else
	{
		ve.v_pixel_end_val = 32;
	}
}

void ve_init(void)
{
	//init default values TODO:read from EEPROM
	ve.h_sync_wait_perc=5;
	ve.v_sync_wait_perc=10;
	
	ve.h_pixel_perc=93;
	ve.v_pixel_perc=80;
	
	ve.v_pixel_end_val=32;
	ve.h_pixel_end_val=32;
	
	ve_calc_v_pixel_clk();
	ve_calc_h_pixel_clk();
}

void ve_set_new_sync_wait_h(uint16_t sync_h)
{
	if((sync_h < 100))
	{
		ve.h_sync_wait_perc=sync_h;
	}
}

void ve_set_new_sync_wait_v(uint16_t sync_v)
{
	if((sync_v < 100))
	{
		ve.v_sync_wait_perc=sync_v;
	}
}

void ve_set_pixel_perc_h(uint16_t pixel_h)
{
	if((pixel_h < 100))
	{
		ve.h_pixel_perc=pixel_h;
	
		if(ve.h_pixel_perc + ve.h_sync_wait_perc > 100)
			ve.h_pixel_perc=100-ve.h_sync_wait_perc;
	}
}

void ve_set_pixel_perc_v(uint16_t pixel_v)
{
	if( (pixel_v < 100))
	{
		ve.v_pixel_perc=pixel_v;
		
		if(ve.v_pixel_perc + ve.v_sync_wait_perc > 100)
			ve.v_pixel_perc=100-ve.v_sync_wait_perc;
	}
}
			

void ve_setpixel(uint16_t h_pos,uint16_t v_pos, uint8_t pixel)
{
	if((h_pos < ve.h_pixel_end_val) && (v_pos < ve.v_pixel_end_val))
		ve.videobuffer[h_pos+(v_pos*ve.v_pixel_end_val)] = pixel;
}
