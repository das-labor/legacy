#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "util.h"
#include "uart.h"

#define TIMETABLE_SIZE 32

/* VARIABLES AS STORED IN MIR */
#define B_ARMS_ON			0x80  // right; bit #7
#define B_ARMS_OPEN			0x40  // right; bit #6

#define B_WHEEL_LEFT_ON		0x20  // right; bit #5
#define B_WHEEL_LEFT_FW		0x10  // right; bit #4

#define B_WHEEL_RIGHT_ON	0x08  // right; bit #3
#define B_WHEEL_RIGHT_FW	0x04  // right; bit #2

#define B_BODY_ON			0x02  // right; bit #1
#define B_BODY_FW			0x01  // right; bit #0

/* TIMETABLE ACTIONS */

#define T_WHEEL_LEFT_FW		0x00
#define T_WHEEL_LEFT_BW		0x01
#define T_WHEEL_LEFT_OFF	0x02

#define T_WHEEL_RIGHT_FW	0x03
#define T_WHEEL_RIGHT_BW	0x04
#define T_WHEEL_RIGHT_OFF	0x05

#define T_ARMS_OPEN			0x06
#define T_ARMS_CLOSE		0x07
#define T_ARMS_OFF			0x08

#define T_GUN_ON			0x09
#define T_GUN_OFF			0x0A

#define T_BODY_FW			0x0B
#define T_BODY_BW			0x0C
#define T_BODY_OFF			0x0D


#define DBG	1


volatile uint8_t MIR = 0x00; // mechanics/motor instruction register
volatile uint8_t timetable[TIMETABLE_SIZE] = {0x00};


ISR(TIMER0_OVF_vect)
{
	static uint8_t counter = 0;
//	if (counter == 0)
	{
		uint8_t temp_mir = 0x00;
		char uart_cmd;
		
		temp_mir = MIR;
		if (uart_getc_nb(&uart_cmd))
		{
			uart_putc('\r');
			uart_putc('\n');
			uart_putc(uart_cmd);
			switch (uart_cmd)
			{
				case 0x1b: // ascii ESC
					uart_getc_nb(&uart_cmd);
					if (uart_cmd == '[')
					{
						uart_getc_nb(&uart_cmd);
						switch (uart_cmd)
						{
							case 'A': // up
								temp_mir |= ( B_WHEEL_LEFT_ON | B_WHEEL_LEFT_FW );
								temp_mir |= ( B_WHEEL_RIGHT_ON | B_WHEEL_RIGHT_FW );
							break;
							case 'B': // down
								temp_mir |= ( B_WHEEL_LEFT_ON );
								temp_mir |= ( B_WHEEL_RIGHT_ON );
							break;
							case 'C': // right
								temp_mir |= ( B_WHEEL_LEFT_ON | B_WHEEL_LEFT_FW );
								temp_mir |= ( B_WHEEL_RIGHT_ON );
							break;
							case 'D': // left
								temp_mir |= ( B_WHEEL_LEFT_ON );
								temp_mir |= ( B_WHEEL_RIGHT_ON | B_WHEEL_RIGHT_FW );
							break;
						}
					}
				break;
				case ' ':
					temp_mir = 0x00;
				break;
				case 'l':
					uart_putstr_P (PSTR("+lw"));
					temp_mir |= ( B_WHEEL_LEFT_ON );
					temp_mir &= ~( B_WHEEL_LEFT_FW );
					break;
				case 'k':
					uart_putstr_P (PSTR("+rw"));
					temp_mir |= ( B_WHEEL_LEFT_ON | B_WHEEL_LEFT_FW );
					break;
				case 'L':
				case 'K':
					temp_mir &= ~B_WHEEL_LEFT_ON;
					uart_putstr_P (PSTR("-lw"));
					break;
				case 'e':
					uart_putstr_P (PSTR("+rw"));
					temp_mir |= ( B_WHEEL_RIGHT_ON | B_WHEEL_RIGHT_FW );
					break;
				case 'r':
					uart_putstr_P (PSTR("+rw"));
					temp_mir |= ( B_WHEEL_RIGHT_ON );
					temp_mir &= ~( B_WHEEL_RIGHT_FW );
					break;
				case 'R':
				case 'E':
					temp_mir &= ~B_WHEEL_RIGHT_ON;
					uart_putstr_P (PSTR("-rw"));
					break;
				case 'a':
					uart_putstr_P (PSTR("+ae"));
					temp_mir |= ( B_ARMS_OPEN | B_ARMS_ON );
					break;
				case 's':
					uart_putstr_P (PSTR("+ac"));
					temp_mir |= ( B_ARMS_ON );
					break;
				case 'A':
				case 'S':
					temp_mir &= ~( B_ARMS_ON | B_ARMS_OPEN );
					uart_putstr_P (PSTR("-ao"));
					break;
				case 'v':
					uart_putstr_P (PSTR("+bf"));
					temp_mir |= ( B_BODY_FW | B_BODY_ON );
					break;
				case 'b':
					uart_putstr_P (PSTR("+ba"));
					temp_mir |= ( B_BODY_ON );
					temp_mir &= ~ ( B_BODY_FW );
					break;
				case 'B':
				case 'V':
					temp_mir &= ~B_BODY_ON;
					temp_mir &= ~ ( B_BODY_FW );
					uart_putstr_P (PSTR("-bo"));
					break;
				default:
					uart_putstr_P (PSTR("???"));
					break;
			}
		}
	
	/*** (1) TIMETABLE READING ***
	 * Reads the timetable and stores changes/actions in the
	 * temporary motor instruction register.
	 * TODO: write the above... some time
	 */

	/*** (2) MOTOR & MECHANICS FUCKUP PROTECTION ***
	 * if portc0 (block-pin) is set to low, disable
	 * all ports and add currently running commands
	 * to list of forbidden commands to prevent their
	 * execution in the next cycle.
	 */
	if ( ~PINC & _BV(PC0) )
	{
		#ifdef DBG
			uart_putstr("\r\n* MOTOR BLOCKED! ");
		#endif
		MIR = temp_mir & ~(MIR);
	} else
	{
		MIR = temp_mir;
	}

	// (3) EXECUTION OF COMMANDS: read MIR and execute it.
	/* ARMS CONTROL */
	if (MIR & B_ARMS_ON)
	{
		PORTD &= ~(_BV(PD6)); // disable bits in case they're on already
		PORTD &= ~(_BV(PD7)); // (avoid collisions ;-)
		PORTD |= (MIR & B_ARMS_OPEN) ? _BV(PD7) : _BV(PD6);
	} else
	{
		PORTD &= ~(_BV(PD6));
		PORTD &= ~(_BV(PD7));
	}

	/* LEFT WHEEL */
	if (MIR & B_WHEEL_LEFT_ON)
	{
		PORTB &= ~(_BV(PB2)); // (same here)
		PORTB &= ~(_BV(PB1)); // 
		PORTB |= (MIR & B_WHEEL_LEFT_FW) ? _BV(PB1) : _BV(PB2);
	} else
	{
		PORTB &= ~(_BV(PB2));
		PORTB &= ~(_BV(PB1));
	}

	/* RIGHT WHEEL */
	if (MIR & B_WHEEL_RIGHT_ON)
	{
		PORTB &= ~(_BV(PB3)); // dito
		PORTB &= ~(_BV(PB4)); // 
		PORTB |= (MIR & B_WHEEL_RIGHT_FW) ? _BV(PB3) : _BV(PB4);
	} else
	{
		PORTB &= ~(_BV(PB4));
		PORTB &= ~(_BV(PB3));
	}
	
	/* BODY */
	if (MIR & B_BODY_ON)
	{
		PORTB &= ~(_BV(PB6)); // "
		PORTB &= ~(_BV(PB5)); // 
		PORTB |= (MIR & B_BODY_FW) ? _BV(PB5) : _BV(PB6);
	} else
	{
		PORTB &= ~(_BV(PB5));
		PORTB &= ~(_BV(PB6));
	}
	}
	counter++;
}

// MOTOR SPEED REGULATION ISR
ISR(TIMER1_COMPA_vect)
{
/*	static uint8_t count;
	

	if (count < speed_ml && speed_ml > 0)
	{
		PORTB |= 0x02;
	} else
	{
		PORTB &= ~(0x02);
	}
	
	if (count < speed_mr && speed_mr > 0)
	{
		PORTB |= 0x08;
	} else
	{
		PORTB &= ~(0x08);
	}

	count++;
*/
}

// add timetable entry
uint8_t add_action ( uint8_t in_action, uint8_t in_delay )
{
	uint8_t i=0;
	for (i=0;i<TIMETABLE_SIZE;i++)
	{
		if (timetable[i] == 0x00) {
			timetable[i] = (in_delay<<4) | in_action;
			return 0; // success.
		}
	}
	return 1; // no free space in timetable
}

void main (void){
	// INIT
	DDRC  &= ~(_BV(DDC0)); // set C0 to input
	PINC |= _BV(PC0);  // enable pullup
	UCSRB |= ( 1 << RXEN ); // Enable uart RX

	PORTB=0x00;
	PORTD &= ~(_BV(PD6) | _BV(PD7));
	uart_init();
	wait (100);
	
	#ifdef DBG
		uart_putstr("\r\nhi.\r\n");
	#endif

	cli();
	// TimerCounter0 Motor runtime measurement
//	TCCR0 |= (1<<CS02)|(1<<CS00);	// Prescaler 8 kHz
	TCCR0 |= 0x05;	// Prescaler 8 kHz
	

	// TimerCounter1 (PWM)
	TCCR1A = (1<<WGM10);		// PWM mode (=PWM12)
//	TCCR1B = 0x01;				// prescaler
	

	OCR1AH = 0x00;
	OCR1AL = 0x60;
	
	wait(1000); // wait for eventual programmer

	TIMSK |= (1<<TOIE0);		// Overflow interrupt timer 0
//	TIMSK |= (1<<OCIE1A);		// Compare interrupt timer 1
	
	sei();
	
	MIR = ( B_WHEEL_LEFT_ON );
	wait(100);
	MIR = ( 0x00 );
	while (1)
	{
	}
}
