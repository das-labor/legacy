


#include <avr/io.h>

#define PIN_CLK (PINB-32)
#define BIT_CLK PB0

#define PORT_ASTB (PORTB-32)
#define BIT_ASTB  PB4


//uint8_t bus_read_helper(void)


	.global bus_read_helper
bus_read_helper:

lb1:
	//wait until clock goes low - this should put us somewhere within 3 cycles of the pinchange
	sbic 	PIN_CLK, BIT_CLK
	rjmp	lb1

	//at this point we are 2,3 or 4 cycles after the pinchange
	
	//12 nops to bring us to next high->low transition
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	
	//the transition is going to happen in one of the next 3 cycles	
	in		r24, PIN_CLK		//now? save value in reg.
	sbis	PIN_CLK, BIT_CLK	//if the transistion wasn't yet burn one more cycle
	rjmp	lb2
	rjmp	lb2

lb2:
	//we are 3 or 4 clocks after the transition now. the bit in r24 is 1 if it's 3.
	bst		r24, BIT_CLK //4/5
	brtc	lb3		// 5/7   jump if we don't need to burn cycle
	
	//5
	nop//6
	nop//7
lb3:
	//there we go - now we're exactly 7 cycles after the transition
	
	//9 more nops to burn the rest of the cycles
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	//nop
	in		r24,PORT_ASTB

	//here we are at the next transition
	//manual says we are supposed to apply the address within 100 to 300ns
	
	ori		r24, (1<<BIT_ASTB) //1
	out		PORT_ASTB, r24 //2

	//now we wait for the end of the cycle, and read the data
	
	//13 nops
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop //15
	
	in		r24, PINC
	
	ret
	
	



