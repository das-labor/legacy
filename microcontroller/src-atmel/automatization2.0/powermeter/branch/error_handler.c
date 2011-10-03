#include "avr_compiler.h"
#include "error_handler.h"
#include "led_driver.h"

volatile uint8_t errorlevel; 
uint8_t errortimeout;

void ERROR_blink()
{
	if(errorlevel){		//check for errors

		if(!errortimeout){
			errortimeout=16;
			LED_on();
		}
		errortimeout--;
		if(errortimeout == 1)
			LED__RED();
	}
}

void setERROR(uint8_t error){
	errorlevel=error&0x0F;
}
uint8_t getERROR(){
	return errorlevel&0x0F;
}
void clearERROR(void){
	errorlevel = ERROR_NONE;
	LED_on();		//turn status LED on
}