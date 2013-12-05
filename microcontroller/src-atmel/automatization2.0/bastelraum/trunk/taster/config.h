#ifndef _CONFIG_H
#define _CONFIG_H

#define BAUD 9600


#define LED_R (_BV(PD7))
#define LED_G (_BV(PD5))
#define LED_B (_BV(PD6))
#define RGB_LED_PORT (&(PORTD))
#define RGB_LED_DDR DDRD


#define LED_1 (_BV(PD5))
#define LED_2 (_BV(PD6))
#define LED_3 (_BV(PD7))
#define LED_PORT (&(PORTB))
#define LED_DDR DDRB

#define TASTER_LICHT	(_BV(PD2))
#define TASTER_ROT		(_BV(PD3))
#define TASTER_GRUEN	(_BV(PD4))
#define TUER_KONTAKT	(_BV(PC0))
#define NUM_INPUTS 3


#define O_OUT00_PORT RGB_LED_PORT
#define O_OUT01_PORT RGB_LED_PORT
#define O_OUT02_PORT RGB_LED_PORT
#define O_OUT03_PORT LED_PORT
#define O_OUT04_PORT LED_PORT
#define O_OUT05_PORT LED_PORT
#define NUM_OUTPUTS 6

#define O_OUT00_BIT LED_R
#define O_OUT01_BIT LED_G
#define O_OUT02_BIT LED_B
#define O_OUT03_BIT LED_1
#define O_OUT04_BIT LED_2
#define O_OUT05_BIT LED_3

#endif // ifndef CONFIG_H

