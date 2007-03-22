
#include <string.h>
#include <avrx.h>

#include <stdio.h>

#include <avr/io.h>

TimerControlBlock my_timer1, my_timer2;


AVRX_GCC_TASKDEF(task1, 200, 4)
{
	
	printf("Debug\r");
	
	DDRD |= (1<<PD4);
	
	while(1){
		AvrXDelay(&my_timer1, 500);
		PORTD ^= (1<<PD4);
	}
}



AVRX_GCC_TASKDEF(task2, 200, 5)
{
	
	printf("Debug\r");
	
	//Motor-Ports initialisieren
	DDRD |= 0xE0;
	DDRB |= 0x07;
	//PWM pins erstmal an
	PORTB |= 0x06;
	
	while(1){
		AvrXDelay(&my_timer2, 400);
		//PORTB ^= (1<<PD0);
	}
}
