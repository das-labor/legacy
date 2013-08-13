#include "config.h"
#include "avr_compiler.h"
#include "led_driver.h"

//this must be volatile static, dont change
static volatile uint8_t LED_status;


//RED is inverted, GREEN,BLUE not
void LED_initPORTC(void)
{
	LED_status = 0;
	PORTC.DIRSET = LED_RED | LED_GREEN | LED_BLUE;
}

void LED_off(void)
{
	LED_status = PORTC.OUT & ( LED_RED | LED_GREEN | LED_BLUE );
	PORTC.OUT |= LED_RED | LED_BLUE | LED_GREEN;
}

void LED_on(void)
{
	PORTC.OUT &= ~LED_status;
}

void LED__RED(uint8_t status)
{
	if(status)
		PORTC.OUT &= ~LED_RED;
	else
		PORTC.OUT |= LED_RED;
}

void LED__GREEN(uint8_t status)
{
	if(status)
		PORTC.OUT &= ~LED_GREEN;
	else
		PORTC.OUT |= LED_GREEN;
}

void LED__BLUE(uint8_t status)
{
	if(status)
		PORTC.OUT &= ~LED_BLUE;
	else
		PORTC.OUT |= LED_BLUE;
}


