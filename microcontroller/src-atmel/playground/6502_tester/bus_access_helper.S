


#include <avr/io.h>

#define PIN_CLK (PINB-32)
#define BIT_CLK PB0

#define PORT_ASTB (PORTB-32)
#define BIT_ASTB  PB3
#define BIT_RW	  PB2

#define DDR_DATA (DDRC-32)

//uint8_t bus_read_helper(void)

	.global bus_read_helper
bus_read_helper:

lba:
	sbic 	PIN_CLK, BIT_CLK
	rjmp	lba
lb0:
	//first wait until clock is high, so we can get in sync
	sbis	PIN_CLK, BIT_CLK
	rjmp	lb0
lb1:
	//wait for high to low transition this should put us somewhere within 3 cycles of the pinchange
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
	//nop
	//nop
	//nop
	in		r24,PORT_ASTB
	ori		r24, ((1<<BIT_ASTB)|(1<<BIT_RW)) //1
	out		PORT_ASTB, r24 //2


	//here we are at the next transition
	//manual says we are supposed to apply the address within 100 to 300ns
	//but it looks good on the scope when we do it there ^
	
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
	//nop //13
	
	//this position looks allright for reading on the scope. maybe shorten to 12 nops - we'll see...
	in		r25,(PINC-32)
	in		r24,(PIND-32) //because of dirk's wiring bug

	cbi		PORT_ASTB, BIT_ASTB;
	
	ret
	
	
//uint8_t bus_write_helper(void)

	.global bus_write_helper
bus_write_helper:

lbba:
	sbic 	PIN_CLK, BIT_CLK
	rjmp	lbba
lbb0:
	//first wait until clock is high, so we can get in sync
	sbis	PIN_CLK, BIT_CLK
	rjmp	lbb0
lbb1:
	//wait for high to low transition this should put us somewhere within 3 cycles of the pinchange
	sbic 	PIN_CLK, BIT_CLK
	rjmp	lbb1

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
	rjmp	lbb2
	rjmp	lbb2

lbb2:
	//we are 3 or 4 clocks after the transition now. the bit in r24 is 1 if it's 3.
	bst		r24, BIT_CLK //4/5
	brtc	lbb3		// 5/7   jump if we don't need to burn cycle
	
	//5
	nop//6
	nop//7
lbb3:
	//there we go - now we're exactly 7 cycles after the transition
	
	//9 more nops to burn the rest of the cycles
	nop
	nop
	nop
	nop
	nop
	//nop
	//nop
	//nop
	//nop
	in		r24,PORT_ASTB
	ori		r24, ((1<<BIT_ASTB))
	andi	r24, (~(1<<BIT_RW))
	out		PORT_ASTB, r24


	//here we are at the next transition
	//manual says we are supposed to apply the address within 100 to 300ns
	//but it looks good on the scope when we do it there ^
	
	//now we wait for the middle of the cycle, and put out the data
	
	//7 nops
	nop
	nop
	nop
	nop
	nop
	//nop
	//nop
	ldi		r25, 0xff
	ldi		r26, 0x80
	out		DDR_DATA, r25
	out		(DDRD-32), r26
	
	//hold data until end of cycle
	
	//6 nops
	
	ldi		r25, 0
	nop
	nop
	nop
	nop
	nop
	ori		r24, (1<<BIT_RW)
		
	out		DDR_DATA,r25	//release the datalines
	out		(DDRD-32),r25
	
	out		PORT_ASTB, r24		//end the write
	cbi		PORT_ASTB, BIT_ASTB	//reset the strobe
	
	ret


//	sbi		(PORTA-32), 7
//	cbi		(PORTA-32), 7


