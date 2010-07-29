#include <avr/io.h>

#ifndef RFMUSB_HW_H
#define RFMUSB_HW_H

/* Set your hardware setup here. If you've
 * built the old Dil Version, use HWREF = 1
 *
 * HWREF = 2 is the newer SMD-version of this
 * board.
 *
 * HWREF = 3 is for the LABNODE 
 */

#ifndef HWREF
	#define HWREF 2
#endif

#if (HWREF == 3)
	//Labnode
	/* LEDs */
	#define LED_PORT_DDR      DDRB
	#define LED_PORT_OUTPUT   PORTB
	#define LED_BIT_RED       (_BV(PB0))
	#define LED_BIT_GREEN     (_BV(PB0))

	/* RFM12 slave select pin */
	#define DDR_SS DDRB
	#define PORT_SS PORTB
	#define BIT_SS 3

#elif (HWREF == 2)
	/* LEDs */
	#define LED_PORT_DDR      DDRD
	#define LED_PORT_OUTPUT   PORTD
	#define LED_BIT_RED       (_BV(PD5))
	#define LED_BIT_GREEN     (_BV(PD6))

	/* RFM12 slave select pin */
	#define DDR_SS DDRC
	#define PORT_SS PORTC
	#define BIT_SS 2
#elif (HWREF == 1)
	/* LED setup */
	#define LED_PORT_DDR      DDRD
	#define LED_PORT_OUTPUT   PORTD
	#define LED_BIT_RED       (_BV(PD6))
	#define LED_BIT_GREEN     (_BV(PD7))

	/* RFM12 slave select pin */
	#define DDR_SS DDRB
	#define PORT_SS PORTB
	#define BIT_SS 0
#endif

#define LED_STATUS_SET(a) \
	if (a) { LED_PORT_OUTPUT |= LED_BIT_GREEN; LED_PORT_OUTPUT &= ~(LED_BIT_RED); } \
	else { LED_PORT_OUTPUT |= LED_BIT_RED; LED_PORT_OUTPUT &= ~(LED_BIT_GREEN); }

#define LED_STATUS_TOGGLE \
	LED_PORT_OUTPUT ^= (LED_BIT_RED | LED_BIT_GREEN)

//the time after which the red led is turned off
#define LED_OFFTIME_RED		0x0003FFFF

#endif /* IFDEF RFMUSB_HW_H */

