#include <limits.h>
#include "types.h"
#include "lpc2103.h"
#include "uart.h"
#include "videoengine.h"
#include "ICcom.h"
#include "tests.h"


void handlePacket(void)
{
	int recbuffer[9];
	recbuffer[0]=uart0Getch();
	if(recbuffer[0] > 0)
	{
		if((uint8_t)(recbuffer[0] & 0xF8) == MAGIC_PACKET)
		{
			uint8_t cnt = (recbuffer[0] & 0x0007);		
			uint8_t j =0;
			for(j=0;j<cnt;j++)
			{
				recbuffer[j+1]=uart0Getch();
			}
			if(recbuffer[1] == 0x01)
			{
				ve_set_new_sync_wait_h((recbuffer[2]<<8) | recbuffer[3]);
			}
			if(recbuffer[1] == 0x02)
			{
				ve_set_new_sync_wait_v((recbuffer[2]<<8) | recbuffer[3]);
			}
			if(recbuffer[1] == 0x03)
			{
				ve_set_pixel_perc_h((recbuffer[2]<<8) | recbuffer[3]);
			}
			if(recbuffer[1] == 0x04)
			{
				ve_set_pixel_perc_v((recbuffer[2]<<8) | recbuffer[3]);
			}
			if(recbuffer[1] == 0x05)
			{
				ve_set_new_resolution_h((recbuffer[2]<<8) | recbuffer[3]);
			}
			if(recbuffer[1] == 0x06)
			{
				ve_set_new_resolution_v((recbuffer[2]<<8) | recbuffer[3]);
			}
			if(recbuffer[1] == 0x07)
			{
				uart0Putch(MAGIC_PACKET|0x01);
				uart0Putch(ve.frame_cnt_avr);
			}
			if(recbuffer[1] == 0x08)
			{
				uart0Puts("test1\n");
				test1(recbuffer[2]);
			}
			if(recbuffer[1] == 0x09)
			{
				uart0Puts("test2\n");
				test2(recbuffer[2],recbuffer[3]);
			}
		}
	}
}
