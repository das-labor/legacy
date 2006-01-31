/*
    hardware.h

    Define my hardware dependent stuff here
    
    31-May-2002 - Added check for mega128
*/
#define CPUCLK 8000000L     // CPU xtal
#define TICKRATE 1000       // AvrX timer queue Tick rate
#define BAUDRATE 19200L     // Debug monitor baudrate

#define LED PORTB           // Change this per your hardware and jumper setup
#define LEDDDR DDRB

#define SWITCH PINA
#define SWITCHP PORTA

// Peripheral initialization

#define TCNT0_INIT (0xFF-CPUCLK/256/TICKRATE)

// NB: Clock divisor bits are different between classic and mega103 chips!
// NB: IAR chooses to use different CPU identifiers for their C compiler
// NB: GCC changes the IO bit names just for fun...

#if defined (__AVR_ATmega103__) || defined (__ATmega103__)
#define TMC8_CK256 ((1<<CS02) | (1<<CS01))
#elif defined (__AVR_AT90S8535__) || defined (__AVR_AT90S8515__) || defined (__AVR_ATmega163__) || defined (__AT90S8535__) || defined (__AT90S8515__) || defined (__ATmega163__)
#define TMC8_CK256 (1<<CS02)
#elif defined (__AVR_ATmega128__) || defined (__ATmega128__)
#define TMC8_CK256 ((1<<CS2) | (1<<CS1))
#endif

#define UBRR_INIT (CPUCLK/(16*BAUDRATE)-1)


