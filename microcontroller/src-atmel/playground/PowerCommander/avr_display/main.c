//gpl v3+

#include <avr/io.h>
#include <util/delay.h>

#include "config.h"
#include "ls020/ls020.h"
#include "ls020/glcd.h"

#include "menu.h"

int main(void)
{
    SPCR = 0;
    // Disable Analog Comparator (power save)
    ACSR = _BV(ACD);
	
	// Disable Analog to Digital converter (power save)
	ADCSRA = 0;

	DDRB = _BV(PB1);  // set pb1 as output for hardware PWM
//	        | _BV(MCP_CS) // MCP_CS
//	        | _BV(MC_MOSI) | _BV(MC_CLK); // mosi, sck output
  	TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1A0); // pwm setup
  	TCCR1B = _BV(WGM12) | _BV(CS22) | _BV(CS21) | _BV(CS20); // pwm setup
  	TCNT1 = 0; // pwm time clear
	OCR1A = 0; // pwm timer compare target

	CMD_DIR |= _BV(LCD_CS) | _BV(LCD_RESET) | _BV(LCD_RS); // LCD Data Ports
//	SPCR = _BV(MSTR) | _BV(SPE); // Master Mode,  Enable SPI
//	SPSR = _BV(SPI2X); // Double speed on
	
//	spi_init(); // Setup SPI
	
	ls020_init_lcd(); // Init LS020 LCD
	
	ls020_8bit_mode(0); // Set 8bit LCD host mode
    ls020_rectangle8(0, 0, 175, 131, 0x80); // Fill Screen with dark red color
	
	while (1)
    {
        //can_handeler();
        //menu_handler();
        
        //idle_clock;
        //clock;
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
    return 1;
}
