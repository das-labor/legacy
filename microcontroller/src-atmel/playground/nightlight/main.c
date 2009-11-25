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
	//TCCR0 = TMC8_CK256;		// Set Timer0 to CPUCLK/256 on atmega8 /32
	TCCR0B = TMC8_CK256;		// Set Timer0 to CPUCLK/256 on atmega168
	TIMSK0 = _BV(TOIE0);		// Enable interrupt flag
	SPCR = 0;
	// Disable Analog Comparator (power save)
	ACSR = _BV(ACD);


	DDRB = _BV(PB1);  // set pb1 as output for hardware PWM

	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11);			// FastPWM bit 2, clk/64
	TCNT1 = 0; // pwm timer clear
	OCR1A = 0; // pwm timer compare target
	OCR1B = 0; // pwm timer compare target
	
	CMD_DIR |= _BV(LCD_CS) | _BV(LCD_RESET) | _BV(LCD_RS); // LCD Data Ports
	
	spi_init(); // Setup SPI
	
	ls020_init_lcd(); // Init LS020 LCD

	
	ls020_8bit_mode(0);	// Set 8bit LCD host mode
	ls020_rectangle8(0, 0, 175, 131, 0x80);	// Fill Screen with dark red color

	xlap_init();

	AvrXRunTask(TCB(laptask));
//	AvrXRunTask(TCB(menutask));
    
    Epilog();                   // Switch from AvrX Stack to first task
    while (1);
    
    return 1;
}
