#include "config.h"
#include "avr_compiler.h"
#include "led_driver.h"

//this must be volatile static, dont change
volatile static uint8_t LED_status;


//RED is inverted, GREEN,BLUE not
void LED_initPORTC(void)
{             
	LED_status = 0;
	PORTC.DIRSET = LED_RED | LED_GREEN | LED_BLUE; 
}

void LED_isrunning(void)
{	 
	LED_status = 1;
	LED_on();
}

void LED_off(void)
{
	PORTC.OUT = LED_RED|LED_BLUE|LED_GREEN;
}

void LED_on(void)            
{	                     
	if(LED_status == 1) 
	{					
		PORTC.OUT = LED_RED | LED_BLUE; 
	} 
	if(LED_status == 2) 
	{	
		PORTC.OUT = LED_RED | LED_GREEN;  
	} 
}

void LED_issampling(void)    
{	                       
	LED_status = 2;		
	LED_on();
}

void LED__RED(void)    
{	                       
	PORTC.OUT = LED_GREEN | LED_BLUE; 
}

void LED__YELLOW(void)    
{	                       
	PORTC.OUT = LED_BLUE; 
}

void LED__PURPLE(void)
{
	PORTC.OUT = LED_GREEN; 
}

void LED__GREEN(void)
{
	PORTC.OUT = LED_BLUE|LED_RED; 
}

void LED__BLUE(void)
{
	PORTC.OUT = LED_GREEN|LED_RED; 
}

void LED__cyan(void)
{
	PORTC.OUT = LED_RED; 
}
