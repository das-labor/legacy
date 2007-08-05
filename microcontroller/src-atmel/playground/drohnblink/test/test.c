#include <stdint.h>
#include <stdio.h>

int main(){
	uint32_t x;

	uint8_t colorbar[48][3];
	for(x=0;x<(48*3);x++){
		colorbar[0][x] = 0;

	}
	for(x=0;x<16;x++){
		colorbar[15-x][0] = x*4;
		colorbar[32+x][0] = x*4;

		colorbar[31-x][1] = x*4;
		colorbar[0+x][1] = x*4;

		colorbar[47-x][2] = x*4;
		colorbar[16+x][2] = x*4;
	}
	uint8_t y = 0;
	while(1){
		printf("%c", 0x55);
		for(x=0;x<48;x++){
			uint8_t r,g,b;
			r = colorbar[(y+x)%48][0];
			g = colorbar[(y+x)%48][1];
			b = colorbar[(y+x)%48][2];
			printf("%c%c%c",r,g,b);
			if(x==24){
				printf("\n");
				usleep(15000l);
			}
		}
		y++;
		y%=48;
		printf("\n");
		usleep(15000l);
	}
}
