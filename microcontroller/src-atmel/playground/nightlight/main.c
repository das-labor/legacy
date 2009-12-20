#include <avr/io.h>
#include <util/delay.h>

#include "can/can.h"
#include "can/spi.h"

#include "config.h"
#include "ls020/ls020.h"
#include "ls020/glcd.h"

//#include "switch.h"
//#include "menu.h"


int main(void)
{
	// Disable Analog Comparator (power save)
	ACSR = _BV(ACD);

	DDRD &= ~(_BV(PD4)| _BV(PD5) | _BV(PD6) | _BV(PD7));
	PORTD |= _BV(PD4)| _BV(PD5) | _BV(PD6) | _BV(PD7);

	DDRB = _BV(PB1) | _BV(PB2);  // set pb1 and PB2 as output for hardware PWM

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11);			// FastPWM bit 2, clk/64
	TCNT1 = 0; // pwm timer clear
	OCR1A = 255; // pwm timer compare target
	OCR1B = 255; // pwm timer compare target
	
	CMD_DIR |= _BV(LCD_CS) | _BV(LCD_RESET) | _BV(LCD_RS); // LCD Data Ports
	
	spi_init(); // Setup SPI
	
	can_init();
	
	ls020_init_lcd(); // Init LS020 LCD

	
	ls020_8bit_mode(0);	// Set 8bit LCD host mode
	ls020_rectangle8(0, 0, 175, 131, 0x30);	// Fill Screen with dark red color
	
	uint8_t stat = 0;
	while (1)
	{
		if (!(PIND & _BV(PD4)))
		{
			if (stat == 1)
			{
				OCR1A = 0;
				stat = 0;
			}
			else
			{
				OCR1A = 255;
				stat = 1;
			}
		_delay_ms(300);
		}
	}
}
