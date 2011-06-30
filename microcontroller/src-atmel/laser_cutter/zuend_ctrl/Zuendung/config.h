/*
 * config.h
 *
 * Created: 23.05.2011 18:13:50
 *  Author: jkerrinnes
 */ 

#define	XTAL		20e6			// 20MHz

#define FIRE_ON (PORTD  |= (1<<PD6))
#define FIRE_OFF (PORTD &= ~(1<<PD6))

#define OVER_ON (PORTA  |= (1<<PA4))
#define OVER_OFF (PORTA &= ~(1<<PA4))

#define DIR_X (PIND & (1<<PIND4))
#define DIR_Y (PIND & (1<<PIND5))
#define STEP_X (PIND & (1<<PIND2))
#define STEP_Y (PIND & (1<<PIND3))
#define ENABLE (PIND & (1<<PIND7))

#define PHASE_A		(PINC & 1<<PC7)
#define PHASE_B		(PINC & 1<<PC6)
#define TASTER		(PINA & 1<<PA0)

#define PULL_DIR_X_ON (PORTD |= (1<<PD4))
#define PULL_DIR_Y_ON (PORTD |= (1<<PD5))
#define PULL_STEP_X_ON (PORTD |= (1<<PD2))
#define PULL_STEP_Y_ON (PORTD |= (1<<PD3))
#define PULL_ENABLE_ON (PORTD |= (1<<PD7))
#define PULL_STEP_Z_ON (PORTB |= (1<<PB2))

#define MENU_MAIN 0
#define MENU_FREQUENZ 1
#define MENU_DURCHMESSER 2
#define MENU_SAVE 3
#define MENU_LOAD 4
#define MENU_START 5

#define MAINMENU_MAX 5

#define PARAM_DEFAULT 1
#define PARAM_1 2
#define PARAM_2 3
#define PARAM_3 4

#define PARAM_MAX 4
