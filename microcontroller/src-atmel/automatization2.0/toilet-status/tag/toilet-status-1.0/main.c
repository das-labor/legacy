#include <avr/io.h>


#include "config.h"

#include "can/can.h"
#include "can_handler.h"
#include "can/spi.h"
#include "can/lap.h"

#include "static_scripts.h"
#include "testscript.h"
#include "lights.h"
#include "motion.h"

volatile static uint16_t countdown = 0;

volatile static can_message msg_light_off =
{
	0x00,
	0x02,
	0x00,
	0x01,
	4,
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

ISR (TIMER0_OVF_vect)
{
        static uint8_t cyclecount = 0, oddeven = 0;

        cyclecount++;

        if (cyclecount < 63) return;

	if (!countdown) return;

        cyclecount = 0;

        if (oddeven & 0x01)
                TCNT0 = 123;
        else
                TCNT0 = 124;

        oddeven++;
	        
	countdown--;
	if (!countdown)
		can_transmit ((can_message*) &msg_light_off);
}

void timer0_init (void)
{
        TCCR0 |= ( _BV(CS00) | _BV(CS02) ); /* clk/1024 */
        TIMSK |= _BV(TOIE0);
}


ISR(TIMER2_OVF_vect) 
{
	uint16_t tmp;

	motiond_tick();

	tmp = motion_check();

	if (tmp)
	{
		countdown = 300; /* 5 min */
	}
}

void timer2_init (void)
{	/* clk / 64 -> ca. 250 compares/s per adc */
	TCCR2 = (_BV(CS22)); // | _BV(CS20));
	TIMSK |= _BV(TOIE2);
}

void init(void)
{
	ACSR = _BV(ACD); // Disable Analog Comparator (power save)

	//initialize spi port
	spi_init();
	
	//initialize can communication
	can_init();
	read_can_addr();
/*
	timer2_init();
	timer0_init();
	mtd_init();
	sei(); */
}


int main(void)
{
	init();
	lights_init();
	
	while (1)
	{
		can_handler();
	}
}
