TCCR0 |= ( _BV(CS00) | _BV(CS02) ); /* clk/1024 */
TIMSK |= _BV(TOIE0);
