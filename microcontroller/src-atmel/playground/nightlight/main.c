//gpl v3+

#include <avr/io.h>
#include <avrx-io.h>
#include <avrx-signal.h>

#include "avrx.h"               // AvrX System calls/data structures

#include "config.h"
#include "ls020/ls020.h"
#include "ls020/glcd.h"
#include "xcan.h"
#include "xlap.h"
#include "spi.h"
//#include "switch.h"
#include "menu.h"

AVRX_SIGINT(SIG_OVERFLOW0)
{
    IntProlog();                // Save interrupted context, switch stacks
    TCNT0 = TCNT0_INIT;			// Reload the timer counter
    AvrXTimerHandler();         // Process Timer queue
    Epilog();                   // Restore context of next running task
};

int main(void)
{
    AvrXSetKernelStack(0);

    MCUCR = _BV(SE);      	// Enable "sleep" mode (low power when idle)
    TCNT0 = TCNT0_INIT;		// Load overflow counter of timer0
    //TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256
    TCCR0B = TMC8_CK256;
    TIMSK0 = _BV(TOIE0);		// Enable interrupt flag
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
	
	spi_init(); // Setup SPI
	
	ls020_init_lcd(); // Init LS020 LCD

	xlap_init();

	AvrXRunTask(TCB(laptask));
	
	ls020_8bit_mode(0); // Set 8bit LCD host mode
    ls020_rectangle8(0, 0, 175, 131, 0x80); // Fill Screen with dark red color
    
/*	
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
    } */
    Epilog();                   // Switch from AvrX Stack to first task
    while(1);
    
    return 1;
}
