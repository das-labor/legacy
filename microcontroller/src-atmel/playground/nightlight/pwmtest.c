#define F_CPU 16000000UL  // 16 MHz

#include <avr/io.h>
#include <util/delay.h>



int main()
{
	DDRB |= (1 << PB1);
  	TCCR1A =  (1 << WGM10) | (1 << COM1A1) | (0 << COM1A0) ;
  	TCCR1B = (1 << WGM12) | (0 << CS22) | (1 << CS21) | (1 << CS20);
  	TCNT1 = 0;
	OCR1A = 0;
	
	while( 1 )
    {
        _delay_ms(1000);
        OCR1A = 0;
        _delay_ms(1000);
        OCR1A = 50;
        _delay_ms(1000);
        OCR1A = 100;
        _delay_ms(1000);
        OCR1A = 150;
        _delay_ms(1000);
        OCR1A = 200;
        _delay_ms(1000);
        OCR1A = 255;
    }
}
