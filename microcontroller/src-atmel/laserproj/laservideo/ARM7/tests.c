/*
 TIMER0 CAP0 -> Enable Timer2(H_SYNC_WAIT) -> call setupTimer2for h_pixel_clk
 TIMER0 CAP1 -> Enable Timer3(V_SYNC_WAIT) -> call setupTimer3for v_pixel_clk

 */
#include "types.h"
#include "lpc2103.h"
#include "config.h"
#include "videoengine.h"

void test1(uint8_t setcolor); //first test,
{
	OUTPORT=setcolor;
}

void test2(uint8_t start, uint8_t stop)		//saw
{
	uint8_t i=0;
	uint16_t k=0;
	uint32_t j=0;
	
	if(start<stop){
		for(k=0;k<2000;k++)
		{
			for(i=start;i<stop;i++)
			{
				OUTPORT=i;
					for(j=0;j<(F_OSC*PLL_MUL)/1000;j++)
					__asm("nop\n\t");
			}
			for(i=stop;i>start;i--)
			{
				OUTPORT=i;
					for(j=0;j<(F_OSC*PLL_MUL)/1000;j++)
					__asm("nop\n\t");
			}
		}
	}
}