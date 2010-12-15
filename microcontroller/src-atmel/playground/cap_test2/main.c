

#include <avr/io.h>
#include <util/delay.h>

// code by tixiv
//
// cap 100n connected between PD5 and PD6.
// PD 6 is additionally connected to capacitive sensor plate.
// code charges plate by applying VCC on PC3, then charges this charge into
// the 100n cap between PC2 and PC3. It is counted how many cycles are needed 
// to charge the 100n cap. 
//
//  Indicator LED connected to PORTA
//
// LEDs in proximity to sensor are connected to PC4(Anode) and PC5(Cathode)


//#define FILTER_CONSTANT 8192ul
#define FILTER_CONSTANT 1024ul


uint8_t analog_reference = 1;

int analogRead(uint8_t pin)
{
        // set the analog reference (high two bits of ADMUX) and select the
        // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
        // to 0 (the default).
        ADMUX = (analog_reference << 6) | (pin & 0x07);
  
#if defined(__AVR_ATmega1280__)
        // the MUX5 bit of ADCSRB selects whether we're reading from channels
        // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
        ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif

		ADCSRA = (1<<ADEN)| 7;
        // start the conversion
        ADCSRA |= (1<<ADSC);

        // ADSC is cleared when the conversion finishes
        while (bit_is_set(ADCSRA, ADSC));

        return ADC;
}


void neu(){
	DDRA |= _BV(PA1) | _BV(PA0) ; //discharge cap

	_delay_us(10);
		
	for(x=0;x<10000;x++){
		DDRA = _BV(PA0);
		_delay_us(1);
		PORTC |= 0x01;
		_delay_us(1);		
		DDRA = _BV(PA1);
		_delay_us(1);		
		PORTC &= ~0x01;
		_delay_us(1);
	}

	printf();	
	
	
	



}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static int uart_putchar(char c, FILE *stream){
	uart_putc(c);	
	return 0;
}

	

int main()
{
	uart_init();
	stdout = &mystdout;

	printf("Hello, world!\r\n");

	DDRB |= _BV(PB0);

	uint32_t filt = FILTER_CONSTANT * 600;
	
	uint16_t thresh;
	uint8_t output = 0;

	while (1)
	{
		uint16_t counter = 0;
		
		
		DDRC |= _BV(PC3) | _BV(PC2) ; //discharge cap

		_delay_us(10);

		do
		{
			counter++;
			DDRC &= ~(_BV(PC2) | _BV(PC3));//discharge off from last cycle

			__asm volatile ( "nop");

			PORTC |= _BV(PC3) | _BV(PC4) ;//charge cap
			if(output) PORTC |=  _BV(PC5); //dont activate illumination if touched
			DDRC |= _BV(PC3) | _BV(PC4) | _BV(PC5);
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			
			PORTC |=  _BV(PC5); //set all lines to high
			
			__asm volatile ( "nop");
			__asm volatile ( "nop");
			
			DDRC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5)); //release lines
			PORTC &= ~(_BV(PC3) | _BV(PC4) | _BV(PC5));
			
			DDRC  |= _BV(PC2);//discharge plate to 100n cap
			__asm volatile ( "nop");
			__asm volatile ( "nop");
		} while ((PINC & _BV(PC3)) == 0);

		DDRC &= ~_BV(PC2);//discharge off


		thresh = filt/FILTER_CONSTANT;
		
		filt -= thresh;
		filt += counter;


		if (counter > (thresh - 10))
		{
			output = 0;
			PORTB &= ~_BV(PB0);
		}
		else if (counter < (thresh - 20))
		{
			output = 1;
			PORTB |= _BV(PB0);
		}
	
	}
}
