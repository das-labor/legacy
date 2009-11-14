#include <avr/interrupt.h>
ISR (TIMER0_OVF_vect)
{
	/* interrupt code */
}
int main ()
{
	TCCR0 |= ( _BV(CS00) | _BV(CS02) ); /* clk/1024 */
	TIMSK |= _BV(TOIE0);
	sei();
	while (23);
}
