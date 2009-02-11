#include <avr/io.h>

/* definitions for the sw interface */
#define BTN_UP     0x00
#define BTN_DOWN   0x01
#define BTN_SELECT 0x02
#define BTN_ANY    0x03
#define NUM_BTNS   4

/* HW config */
#define BV_RT_A       (_BV(PD3))
#define BV_RT_SELECT  (_BV(PC4))
#define BV_RT_B       (_BV(PD4))

#define PORT_RT_A       PORTD
#define PORT_RT_SELECT  PORTC
#define PORT_RT_B       PORTD

#define DDR_RT_A        DDRD
#define DDR_RT_SELECT   DDRC
#define DDR_RT_B 	DDRD

#define PIN_RT_A        PIND
#define PIN_RT_SELECT   PINC
#define PIN_RT_B        PIND

#define RT_TICKS 30


void input_hook (uint8_t in_btn, void ((*in_func)()));
void input_exec (uint8_t in_btn);
void rot_poll ();
void input_init ();
