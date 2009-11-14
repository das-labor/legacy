#include <avr/io.h>
int main (void)
{
	DDRA = 0xff; /* gesamten Port A auf Ausgang */
	while (1)
	{
		if (PORTA)
		{
			PORTA += 3;
		} else
		{
			PORTA = 1;
		}
	}
}
