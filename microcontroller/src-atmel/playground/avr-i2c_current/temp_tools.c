#include "temp_tools.h"

/* [Descrition of the tempstruct]
 * 
 */
 

#define HIGH(s) ((s)>>8)
#define LOW(s)  ((s)&0xFF)

double		temp2double(uint16_t t){
	return ((double)HIGH(t)+((double)LOW(t) / 256)); 
}

uint16_t		double2temp(double d){
	uint16_t t;
	((uint8_t*)&t)[1]=(uint8_t)d;
	((uint8_t*)&t)[0]=(uint8_t)(d*256);
	return (t);
}


