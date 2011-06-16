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

//HSYNC
void __attribute__ ((interrupt("IRQ"))) tmr0_interrupt(void)
{
	if(T0IR&TIR_MR1I){ //Timer0 Match1 Interrupt	
		OUTPORT=0xff;
		T0MCR &= ~TMCR_MR1_I;	//Timer0 clear Interrupt on MR1
		T2TCR = 0;            // Timer2 Disable
		T0IR|=TIR_MR1I;		//reset Interrupt
	}
	
	if(T0IR&TIR_CR1I){ //Timer0 Capture0 Interrupt	
		ve.h_line_cnt++;
		ve.h_line_time +=T0CR1; //add to counter
		T0TC=0; //reset the counter
		OUTPORT = 0x0;	//laser off
		if((ve.h_sync_wait > 0) && (T3TCR == TCR_ENABLE)){	//only activate Timer2 if Timer3 is running
			T0MR0 = ve.h_sync_wait;     // set Compare-hit 
			T0MCR |= TMCR_MR0_I;	//Timer0 set Interrupt on MR0
		}

		if(ve.h_line_time_avr > 21){
			T0MR1 = ve.h_line_time_avr - 20;
			T0MCR |= TMCR_MR1_I;//Timer0 set Interrupt on MR1
		}
		T0IR|=TIR_CR1I;		//reset Interrupt
	}
	if((T0IR&TIR_MR0I) || (ve.h_sync_wait == 0)){ //Timer0 Match0 Interrupt or not h_sync_wait			
		T2MR0 = ve.h_pixel_end_val; // set Compare-hit 
		T2MCR = TMCR_MR0_S; //Stop on MR0
		T2IR=0xFF; //clear all interrupts
		T2PR=ve.h_pixel_clk_pre; //set the prescaler
		T2CTCR=0; //use PCLK as clock source
		T2TCR = TCR_ENABLE;            // Timer2 Enable
		
		T0IR|=TIR_MR0I;	//reset interrupt
		T0MCR&=~TMCR_MR0_I;	//Timer0 disable Interrupt on MR0
	}
	
	VICVectAddr=0; //prepare VIC for next Interrupt ???
}

//on cap0 interrupt
//->set MR0 interrupt
//on MR0 interrupt
//->set Timer3 on (VCLOCK)

//VSYNC
void __attribute__ ((interrupt("IRQ"))) tmr1_interrupt(void)
{
	if(T1IR&TIR_MR1I){ //Timer1 Match1 Interrupt	
		OUTPORT=0xff;
		T1MCR &= ~TMCR_MR1_I;	//Timer1 clear Interrupt on MR1
		T1IR|=TIR_MR1I;		//reset Interrupt
	}
	if(T1IR&TIR_CR0I){ //Timer1 Capture0 Interrupt			

		ve.frame_cnt++;		//increment frame counter
		ve.frame_time+=T1CR0; //increment frame time counter
		T1TC=0; //reset the counter
		
		if(ve.h_sync_wait > 0){
			T1MR0 = ve.v_sync_wait; //wait until enabling Timer3 (v_pixel_clk)
			T1MCR |= TMCR_MR0_I;	//Timer1 set Interrupt on MR0
		}

		if(ve.frame_time_avr > 21){
			T1MR1 = ve.frame_time_avr - 20;
			T1MCR |= TMCR_MR1_I;//Timer0 set Interrupt on MR1
		}
		
		//T3TCR=TCR_RESET;	//stop Timer3 (should be off anyway) TODO:??
		T1IR|=TIR_CR0I;	//reset Timer1 interrupt Capture0
	}
	if((T1IR&TIR_MR0I) || (ve.v_sync_wait == 0)){ //Timer1 Match0 Interrupt or ve.v_sync_wait == 0
		T3MR0 = ve.v_pixel_end_val; // set Timer3 Compare-hit to
		T3MCR = TMCR_MR0_S; //Stop Timer3 on MR0
		T3IR=0xFF; //clear all interrupts
		T3PR=ve.v_pixel_clk_pre; //set the prescaler
		T3CTCR=0; //use PCLK as clock source
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
	ve.frame_cnt_avr/=2;		//average fps

	ve.h_line_cnt_avr+=ve.h_line_cnt;	//linecounter
	ve.h_line_cnt_avr/=2;	//average lps

	if(ve.h_line_cnt_avr>0)	//same as 1s /h_line_cnt_avr =?
		ve.h_line_time_avr=ve.h_line_time/ve.h_line_cnt_avr;
	
	if(ve.frame_cnt_avr > 0)//same as 1s /frame_cnt_avr =?
		ve.frame_time_avr=ve.frame_time/ve.frame_cnt_avr;

	ve.h_line_cnt=0;	//h-lines per second
	ve.frame_cnt=0;		//frames per second
	ve.frame_time=0;	//frame duration
	ve.h_line_time=0;	//h-line time

	FIO0PINL ^= LEDMASK; 	// LED blink
	
	ILR|=1;	//reset interrupt 
	VICVectAddr=0; //prepare VIC for next Interrupt

}
