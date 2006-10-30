
#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "pl_dim.h"
#include "config.h"

TimerControlBlock   timer1,             // Declare the control blocks needed for timers
                    timer2;


unsigned int getnum(char ** str){
	unsigned int num=0;
	unsigned char gotnum = 0;
	
	while( (**str >= '0') && (**str <='9') ){
		gotnum = 1;
		num *= 10;
		num += **str - '0';
		(*str)++;
	}
	
	if(gotnum){
		return num;
	}else{
		return 0xffff;
	}
}


uint16_t brightnes[8];

char moodstring[] = "g=255 b=0 r=0 r+8 r?255 g-8 g?0 b+8 b?255 r-8 r?0 g+8 g?255 b-8 b?0";

AVRX_GCC_TASKDEF(moodtask, 100, 4)
{
	char *moodstr = moodstring;
    while (1)
    {
		
		
		int8_t deltas[8];
		uint8_t wait=0, waitchannel=0xff, waitbrightnes=0;
		uint8_t x;
		for(x=0;x<8;x++){
			deltas[x]=0;
		}
		
		while(!wait){
			uint8_t channel;
			uint16_t num;
			if(*moodstr == 0){
				moodstr = moodstring;	
			}
			switch (*moodstr++){
				case 'r':
					channel = 2;
					break;
				case 'g':
					channel = 1;
					break;
				case 'b':
					channel = 0;
					break;
				default:
					channel=0xff;
			}
			if (channel != 0xff){
				switch(*moodstr++){
					case '+':
							if((num = getnum(&moodstr)) != 0xFFFF){ 
								deltas[channel] = num;	
							}
							break;
					case '-':
							if((num = getnum(&moodstr)) != 0xFFFF){ 
								deltas[channel] = -num;	
							}
							break;
					case '?':
							if((num = getnum(&moodstr)) != 0xFFFF){ 
								waitchannel = channel;
								waitbrightnes = num;
								wait = 1;
							}
					case '=':
							if((num = getnum(&moodstr)) != 0xFFFF){ 
								brightnes[channel] = num<<6;	
							}
							break;
				}				
			}
		}
		
		while(wait){
			uint8_t y;
			for(y=0;y<8;y++){
				uint8_t bright;
				int16_t b;
				b = brightnes[y] + 8 * deltas[y];
				if ( b > 0x3FFF ) b = 0x3FFF;
				if ( b < 0 ) b = 0;
					
				brightnes[y] = b;
				bright = b >>6;
				
				lampe_set(y, bright);
				if ( (y == waitchannel) && (bright == waitbrightnes) ){
					wait=0;
				}
			}
			AvrXStartTimer(&timer1, 100);
			AvrXWaitTimer(&timer1);
    	}
	}
};
