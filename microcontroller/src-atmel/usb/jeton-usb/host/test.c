#include "./CDriver/i2cUsb.h"
#include <stdio.h>


i2cusb_dev_handle *h;


void hexdump(uint8_t * buf){
	int x,y;
	for(y=0;y<8;y++){
			for(x=0;x<16;x++){
				printf("%02X ", (uint8_t)*buf++);
			}
			printf("\n");
		}
}



int main(void)
{
	//i2cusb_dev_handle *h = malloc(sizeof(i2cusb_dev_handle));

	if(i2cusb_Connect(&h) != 0)
	{
		printf("Connection error..\n");

		return 1;
	}

	printf("Okay!\n");


	while(1){
		uint8_t status;
			
		do{
			i2cusb_GetCardStatus(h, &status);
		}while(status == 0x80);

		printf("Status %02X\n", status);

		char cb[256];
		i2cusb_ReadCard (h, 0x80, cb);
		hexdump (&cb);

		int x = 0;
		for(x=0;x<128;x++){
			cb[x] = x;
		}

		i2cusb_WriteCard (h, 0x80, cb);


		printf("readback :\n");

		i2cusb_ReadCard (h, 0x80, cb);
		hexdump (&cb);

		i2cusb_ShowMeTheMoney (h);

		i2cusb_SetScannerPower(h,1);

		do{
			i2cusb_GetCardStatus(h, &status);
		}while(status != 0x80);


		i2cusb_SetScannerPower(h,0);

	}
	
	return 0;
}
