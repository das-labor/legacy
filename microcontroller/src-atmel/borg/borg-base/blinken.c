#include "blinken.h"
#include "pixel.h"

//blinken backbuffer
//NOTE: someone should make this only available via define - it may be big
unsigned char blinkbuf[NUMPLANE][NUM_ROWS][LINEBYTES];

//auto position increment flag
unsigned char blink_autopos = 0;
unsigned char blink_pos = 0;

//clears the current blinken backbuffer to color
void blink_clearbuf(unsigned char value)
{
	unsigned char p,*pix,v=0xff;
	unsigned int i;
	for(p=0;p<NUMPLANE;p++){
		pix=&blinkbuf[p][0][0];
		if(p==value)
			v=0;
		for(i=0;i<NUM_ROWS*LINEBYTES;i++)
			pix[i]=v;
	}
}

//sets auto position increment on or off
void blink_setautopos(unsigned char val)
{
	blink_autopos = val?1:0;
}

//set the current blinkenbuffer offset position
void blink_setpos(unsigned char pos)
{
	//set pos and clip to buffer len
	blink_pos = pos % (NUMPLANE * NUM_ROWS * LINEBYTES);
}

//puts the current blinkenbuffer to the frontbuffer
void blink_show(void)
{
	unsigned char p,*pix, *buf;
	
	buf = (void *)blinkbuf;
	pix = (void *)pixmap;

	//copy data
	for(p = 0;p < (NUMPLANE * NUM_ROWS * LINEBYTES); p++)
	{	
		pix[p] = buf[p];
	}
}

//puts data into the blinkenbuffer
void blink_data(unsigned char *data, unsigned char len)
{
	unsigned char p, *buf;
	
	//clip to buffer length
	len %= ((NUMPLANE * NUM_ROWS * LINEBYTES) - blink_pos);
	
	//offset buffer
	buf = (unsigned char *)((unsigned char *)blinkbuf + (unsigned int)blink_pos);

	//copy data
	for(p = 0; p < len; p++)
	{	
		buf[p] = data[p];
	}
	
	if(blink_autopos)
	{
		blink_pos += len;
	}
}
