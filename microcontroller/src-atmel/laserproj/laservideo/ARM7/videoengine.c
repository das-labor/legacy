/*
 TIMER0 CAP0 -> Enable Timer2(H_SYNC_WAIT) -> call setupTimer2for h_pixel_clk
 TIMER0 CAP1 -> Enable Timer3(V_SYNC_WAIT) -> call setupTimer3for v_pixel_clk

 */
#include "types.h"
#include "lpc2103.h"
#include "config.h"
#include "timer.h"
#include "videoengine.h"
#include <math.h>

volatile Videoengine_t ve;

/*
ve.frame_time_avr


*/

void ve_calc_h_pixel_clk(void)
{
	uint32_t i=0;
	uint32_t j=0;
	
	if(ve.h_line_time_avr > 0){
		i=(ve.h_line_time_avr * ve.h_pixel_perc)/100;
		j=(ve.h_line_time_avr * ve.h_sync_wait_perc)/100;

		if(j > ve.h_line_time_avr) //check if MR0 might be out of range (should never happen)
			j = ve.h_line_time_avr -1;

		/* calculate prescaler */
		ve.h_sync_wait_pre=0;
		if(j > 0x0000ffff){
			if(j > 0x000fffff){
				ve.h_sync_wait_pre=0x00ff;
				ve.h_sync_wait=(j>>8);
			}else{
				ve.h_sync_wait_pre=0x000f;
				ve.h_sync_wait=(j>>4);
				
			}
		}

		
		/* used to calculate pixel clock */
		if((uint32_t)(i/ve.h_pixel_end_val) <= 0x0000ffff)
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

		if(j > ve.frame_time_avr) //check if MR0 might be out of range (should never happen)
			j = ve.frame_time_avr -1;
		
		/* calculate prescaler */
		ve.v_sync_wait_pre=0;
		if(j > 0x0000ffff){
			if(j > 0x000fffff){
				ve.v_sync_wait_pre=0x00ff;
				ve.v_sync_wait=(j>>8);
			}else{
				ve.v_sync_wait_pre=0x000f;
				ve.v_sync_wait=(j>>4);
			}
		}
		
		/* used to calculate pixel clock */
		if((uint32_t)(i/ve.v_pixel_end_val) <= 0x0000ffff)
			ve.v_pixel_clk_pre=(i/ve.v_pixel_end_val);
		else
			ve.v_pixel_clk_pre=0xffff;
		
	}
}

void ve_set_new_resolution_h(uint16_t res_h)
{
	if((res_h > 32)){
		if(res_h * ve.v_pixel_end_val < VIDEO_BUFFER_SIZE)		//does it fit into video memory
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
		if(res_v * ve.h_pixel_end_val < VIDEO_BUFFER_SIZE)		//does it fit into video memory
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
	if((ve.v_pixel_end_val == 0) || (ve.h_pixel_end_val == 0) || (ve.h_pixel_perc == 0) || (ve.v_pixel_perc == 0))
		return;

	ve_set_new_resolution_v(ve.v_pixel_end_val);
	ve_set_new_resolution_h(ve.h_pixel_end_val);
		
	if(ve.h_sync_wait_perc>100)
		ve.h_sync_wait_perc=100;

	if(ve.v_sync_wait_perc>100)
		ve.v_sync_wait_perc=100;

	if(ve.h_pixel_perc>(100-ve.h_sync_wait_perc))
		ve.h_pixel_perc=100-ve.h_sync_wait_perc;

	if(ve.v_pixel_perc>(100-ve.v_sync_wait_perc))
		ve.v_pixel_perc=100-ve.v_sync_wait_perc;

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
		if((pixel_h + ve.h_sync_wait_perc) > 100)		//check if its more than 100%
			ve.h_pixel_perc=100-ve.h_sync_wait_perc;		//set maximum possible without changing sync_perc
		else
			ve.h_pixel_perc=pixel_h;
	}
}

void ve_set_pixel_perc_v(uint16_t pixel_v)
{
	if( (pixel_v < 100))
	{
		if((pixel_v + ve.v_sync_wait_perc) > 100)	//check if its more than 100%
			ve.v_pixel_perc=100-ve.v_sync_wait_perc;	//set maximum possible without changing sync_perc
		else
			ve.v_pixel_perc=pixel_v;
	}
}
			

void ve_setpixel(uint16_t h_pos,uint16_t v_pos, uint8_t pixel)
{
	if((h_pos < ve.h_pixel_end_val) && (v_pos < ve.v_pixel_end_val))
		ve.videobuffer[h_pos+(v_pos*ve.v_pixel_end_val)] =pixel;//ve_gammacorrection(pixel,0.7f,200);
}
#if 0
uint8_t ve_gammacorrection(uint8_t val,uint8_t gamma, uint8_t maxval)
{
	uint8_t correction=1; //what is this ??
	float g=(float)gamma/(float)0xFF;
	float v=(float)val/(float)0xFF;

	return (uint8_t)pow((maxval*correction*v),g);
}
#endif
