#include "api.h" 

void police() 
{ 
	int y, x, z, color0, color1, i = -1; 
	clearScreen(black);
	blink0:
        color0 = R; color1 = B;
    blink1: 
	for (y = 0; y < (MAX_Y + ((i & 0x2)>>1))/2; y++) {
		for (x = 0; x < MAX_X; x++) {
			for (z = 0; z < MAX_Z; z++)
			{
				imag[z][y][x][color0] = 255;
				imag[z][y][x][color1] = 0;
			}
		}
	}
	for (y = (MAX_Y + ((i & 0x2)>>1))/2; y < MAX_Y; y++) {
		for (x = 0; x < MAX_X; x++) {
			for (z = 0; z < MAX_Z; z++)
			{
				imag[z][y][x][color0] = 0;
				imag[z][y][x][color1] = 255;
			}
		}
	}
	swapAndWait(120);
	clearScreen(black);
	swapAndWait(100);
	i++;
	if (0x1 & i)
	{
		goto blink1;
	}
	swapAndWait(300);
	if (i > 20) goto end;
	if (0x1 & i>>1)
	{
		color0 = B;
		color1 = R;
		goto blink1;
	}
	goto blink0;
	end:
	swapAndWait(5000);
}
