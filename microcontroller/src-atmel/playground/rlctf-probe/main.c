#include <avr/io.h>
#include <avr/interrupt.h>

#include "rfm12_config.h"
#include "rfm12.h"

static char msg[] = "Nakka Nakka   ";

ISR(TIMER0_OVF_vect)
{
        static uint8_t qms = 1;
	static uint16_t seconds = 0;
	static char msg[] = "Nakka Nakka   ";

        if (qms < 61) /* the timer produces approx. 61 overflows per sec */
        {
                qms++;
                return;
        }

        /* approx. 1 sec - timing on the avr is not as accurate.. */
        qms = 0;
	seconds++;

	msg[12] = (seconds >> 8);
	msg[13] = (seconds & 0xff);

	if (seconds & 0x01)
	{	
		rfm12_tx (sizeof(msg), 0, msg);
	}
}


void timer0_init()
{
        TCCR0 = (_BV(CS02) | _BV(CS00)); /* /1024 */
        TIMSK |= (_BV(TOIE0));
}

int main (void)
{
	DDRC |= (_BV(PC0) | _BV(PC1));
	timer0_init();
	rfm12_init();

	while (23)
	{
		rfm12_tick();
	}
}
