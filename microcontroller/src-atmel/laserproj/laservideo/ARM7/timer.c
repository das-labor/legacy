/******************************************************************************/
/*  TIMER.C: Time Functions for 10Hz Clock Tick using Timer0                  */
/******************************************************************************/
/*  Inspired by a sample application from Keil Elektronik                     */
/*  A lot of information has been found in a sample from R O Software.        */
/******************************************************************************/
/*  Sample for WinARM by M.Thomas <eversmith@heizung-thomas.de>               */
/******************************************************************************/

#include "lpc2103.h"
#include "config.h"
#include "timer.h"
#include "videoengine.h"
#include "uart.h"
#include "menu.h"
#include "printf_P.h"

volatile unsigned long timeval;
extern volatile Videoengine_t ve;

/* Timer0 Compare-Match Interrupt Handler (ISR) */

//on cap0 interrupt
//->set MR0 interrupt
//on MR0 interrupt
//->set Timer2 on (HCLOCK)

/* Setup Timer0 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
void init_timer0 (void) {
	PINSEL0 |=(1 << 9);	//set Pin0.4 as T0CR1 input
	PINSEL0 &=~(1<<8);
    T0CCR = TCCR_CR1_R|TCCR_CR1_I; // Capture Control Register, CAP0.1 Falling Edge+ Int
	T0MCR = 0;	//clear match interrupt
	T0MR0 = 0x000fffff;	//this will never happen
	T0IR=0xff; //clear all interrupts
	T0PR=0; //prescaler = 0
	T0TCR = TCR_ENABLE;            // Timer0 Enable

	//VICIntSelect |= VIC_BIT(VIC_TIMER0);	//set as FIQ, note: only ONE FIQ is allowed !
	VICVectAddr0 = (unsigned long)tmr0_interrupt;   // set interrupt vector in 0
	VICVectCntl0 = VIC_ENABLE | VIC_TIMER0; // use it for Timer 0 Match Interrupt:
	VICIntEnable = VIC_BIT(VIC_TIMER0);    // Enable Timer0 Interrupt
}

/* Setup Timer1 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
void init_timer1 (void) {
	PINSEL0 |=(1 << 21);	//set Pin0.10 as T1CR0 input
	PINSEL0 &=~(1<<20);
	
    T1CCR = TCCR_CR0_R|TCCR_CR0_I; // Capture Control Register, CAP1.0 Falling Edge+ Int
	T1MCR =  0;	//clear match interrupt
	T1PR=0; //prescaler = 0
	T1MR0 = 0x000fffff;	//this will never happen
	T1IR= 0xff; //clear all interrutps
	T1TCR = TCR_ENABLE;            // Timer1 Enable
	
	VICVectAddr1 = (unsigned long)tmr1_interrupt;   // set interrupt vector in 1
	VICVectCntl1 = VIC_ENABLE | VIC_TIMER1; // use it for Timer 0 Match Interrupt:
	VICIntEnable = VIC_BIT(VIC_TIMER1);    // Enable Timer0 Interrupt
}

/* HCLOCK Pixel */
void init_timer2(void)
{
	T2MCR = TMCR_MR0_S; //Stop on MR0
	T2IR=0xFF; //clear all interrupts
	T2CTCR=0; //use PCLK as clock source
	T2TC=0;		//reset timer2 counter
	T2TCR =0;   // Timer2 disable
}

/* VCLOCK Pixel */
void init_timer3(void)
{
	T3MCR = TMCR_MR0_S; //Stop Timer3 on MR0
	T3IR=0xFF; //clear all interrupts
	T3CTCR=0; //use PCLK as clock source
	T3TC=0;	//reset timer3 counter 
	T3TCR=0;	//disable Timer3
}

/* Setup RTC */
void init_RTC (void){
	CIIR=0x01; //interrupt once a second
	AMR=0xFF; //disable Alarm clk
	CCR=RTC_CLK_SOURCE|RTC_CLK_ENABLE;  //enable rtc, use xtal as source

	VICIntSelect &= ~VIC_BIT(VIC_RTC);		//set Interrupt is IRQ
	VICVectAddr12=(unsigned long)rtc_interrupt;
	VICVectCntl12=VIC_ENABLE|VIC_RTC;
	VICIntEnable |= VIC_BIT(VIC_RTC);	//enable RTC Interrupt
}



//HSYNC
FASTRUN void  __attribute__ ((interrupt("IRQ"))) tmr0_interrupt(void)
{
	
	if(T0IR&TIR_CR1I){ //Timer0 Capture0 Interrupt	
		ve.h_line_cnt++;
		ve.h_line_time +=T0CR1; //add to counter
		T0TC-=T0CR1; //reset the counter

		//if((ve.h_sync_wait > 0) && (T3TCR & TCR_ENABLE)){	//only activate Timer2 if Timer3 is running
		if(T3TCR & TCR_ENABLE){	
			T0MR0 = ve.h_sync_wait;     // set Compare-hit 
			T0MCR |= TMCR_MR0_I;	//Timer0 set Interrupt on MR0
		}
		
		T0IR|=TIR_CR1I;		//reset Interrupt
	}
	if((T0IR&TIR_MR0I) || (ve.h_sync_wait == 0)){ //Timer0 Match0 Interrupt or not h_sync_wait			
		T2TC=0;		//reset timer2 counter
		T2TCR = TCR_ENABLE;            // Timer2 Enable
		
		T0IR|=TIR_MR0I;	//reset interrupt
		T0MCR&=~TMCR_MR0_I;	//Timer0 disable Interrupt on MR0
	}
	
	VICVectAddr=0; //prepare VIC for next Interrupt ???
}


//VSYNC
void __attribute__ ((interrupt("IRQ"))) tmr1_interrupt(void)
{

	if(T1IR&TIR_CR0I){ //Timer1 Capture0 Interrupt			

		ve.frame_cnt++;		//increment frame counter
		ve.frame_time+=T1CR0; //increment frame time counter
		T1TC-=T1CR0; //reset the counter
		
		T1MR0 = ve.v_sync_wait; //wait until enabling Timer3 (v_pixel_clk)
		T1MCR |= TMCR_MR0_I;	//Timer1 set Interrupt on MR0
		
		//T3TCR=TCR_RESET;	//stop Timer3 (should be off anyway) TODO:??
		T1IR|=TIR_CR0I;	//reset Timer1 interrupt Capture0
	}
	if(T1IR&TIR_MR0I){ //Timer1 Match0 Interrupt or ve.v_sync_wait == 0
		T3TC=0;  //reset timer3 counter 
		T3TCR=TCR_ENABLE;	//start Timer3

		T1IR|=TIR_MR0I;	//reset interrupt
		T1MCR &=~TMCR_MR0_I;	//Timer1 clear Interrupt on MR0
	}
	VICVectAddr=0; //prepare VIC for next Interrupt ???
}

void __attribute__ ((interrupt("IRQ"))) rtc_interrupt(void)
{
	timeval++;
	
	ve.frame_cnt_avr+=ve.frame_cnt;	//framecounter
	ve.frame_cnt_avr=(ve.frame_cnt_avr>>1);		//average fps

	ve.h_line_cnt_avr+=ve.h_line_cnt;	//linecounter
	ve.h_line_cnt_avr=(ve.h_line_cnt_avr>>1);	//average lps

	if(ve.h_line_cnt_avr>0)	//same as 1s /h_line_cnt_avr =?
		ve.h_line_time_avr=ve.h_line_time/ve.h_line_cnt_avr;
	
	if(ve.frame_cnt_avr > 0)//same as 1s /frame_cnt_avr =?
		ve.frame_time_avr=ve.frame_time/ve.frame_cnt_avr;

	ve.h_line_cnt=0;	//h-lines per second
	ve.frame_cnt=0;		//frames per second
	ve.frame_time=0;	//frame duration
	ve.h_line_time=0;	//h-line time

	/* update all ve registers */
	ve_init();
	
	/* update timer2 register */
	T2MR0 = ve.h_pixel_end_val; // set Compare-hit 
	T2PR=ve.h_pixel_clk_pre; //set the prescaler
	
	/* update timer3 register */
	T3MR0 = ve.v_pixel_end_val; // set Timer3 Compare-hit to
	T3PR=ve.v_pixel_clk_pre; //set the prescaler

	
	FIO0PINL ^= LEDMASK; 	// LED blink
	
	ILR|=1;	//reset interrupt 
	VICVectAddr=0; //prepare VIC for next Interrupt
}
