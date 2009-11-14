#include <util/delay.h>
...
uint8_t i, counter = 0;
for (i=0;i<5;i++)
{
	if (PINA & (_BV(PA0))) counter++;
	_delay_ms(2); /* warte 2ms */
}
if (counter >= 2)
{
	/* tue dinge */
}
