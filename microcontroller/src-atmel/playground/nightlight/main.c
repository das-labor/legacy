#include <avr/io.h>
#include <util/delay.h>

#include "can/can.h"
#include "can/spi.h"
#include "can_handler.h"

#include "config.h"
#include "ls020/ls020.h"
#include "ls020/glcd.h"

//#include "switch.h"
//#include "menu.h"

const char pms_klein[] PROGMEM = "klein"; 
const char pms_gross[] PROGMEM = "gross";
const char pms_beide[] PROGMEM = "beide";

const PGM_P pgm_strings[] PROGMEM = {pms_klein, pms_gross, pms_beide};


int main(void)
{
	// Disable Analog Comparator (power save)
	ACSR = _BV(ACD);

	DDRD &= ~(_BV(PD4)| _BV(PD5) | _BV(PD6) | _BV(PD7)); // Taster
	PORTD |= _BV(PD4)| _BV(PD5) | _BV(PD6) | _BV(PD7);   // Taster Pullup

	DDRB = _BV(PB1) | _BV(PB2);  // set pb1 and PB2 as output for hardware PWM

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11);			// FastPWM bit 2, clk/64
	TCNT1 = 0; // pwm timer clear
	OCR1A = 255; // pwm timer compare target
	OCR1B = 255; // pwm timer compare target
	
	CMD_DIR |= _BV(LCD_CS) | _BV(LCD_RESET) | _BV(LCD_RS); // LCD Data Ports
	
	spi_init(); // Setup SPI
	
	can_init();
	read_can_addr();
	
	ls020_init_lcd(); // Init LS020 LCD

	
	ls020_8bit_mode(0);	// Set 8bit LCD host mode
	ls020_rectangle8(0, 0, 175, 131, 0x0f);	// Fill Screen with dark red color
	
	uint8_t stat = 0, dir = 0, rollo = 0;
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
		if (!(PIND & _BV(PD5)))
		{
			uint8_t data[2] = {0, 0};
			if (dir == 1)
			{
				data[1] = 0;
				OCR1B = 255;
				dir = 0;
			}
			else
			{
				data[1] = 1;
				OCR1B = 0;
				dir = 1;
			}
			data[0] = rollo;
			if (rollo == 2) {
				data[0] = 0;
				can_send(0x02, data);
				_delay_ms(1);
				data[0] = 1;
			}
			can_send(0x02, data);
			_delay_ms(300);
		}
		if (!(PIND & _BV(PD6)))
		{
			uint8_t data[2] = {0, 0};
			data[1] = 2;
			can_send(0x02, data);
			_delay_ms(300);
		}
		if (!(PIND & _BV(PD7))) {
			rollo++;
			if (rollo == 3) {
				rollo = 0;
			}
			ls020_put_string8(5, 20, (char *) pgm_read_word(pgm_strings + rollo), 0x00, 0x0f);
			_delay_ms(300);			
		}
		can_handler();
	}
}

