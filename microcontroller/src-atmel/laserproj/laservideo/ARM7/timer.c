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
#include "ICcom.h"

#define TxIR_MR0_FLAG        (1<<0)

volatile unsigned long timeval;

/* Timer0 Compare-Match Interrupt Handler (ISR) */
//HSYNC
void __attribute__ ((interrupt("IRQ"))) tmr0_interrupt(void) 
{
	if(T0IR&TIR_CR0I){ //Timer0 Capture0 Interrupt	
		ve.h_line_time +=T0CR0; //add to counter
		ve.h_line_time+=T0CR0;

		if(ve.h_sync_wait > 0){
			T0MR0 = ve.h_sync_wait;     // set Compare-hit 
		}
		
		T0IR|=TIR_CR0I;		//reset Interrupt
	}
	if(T0IR&TIR_MR0I){ //Timer0 Match0 Interrupt			
		T2MR0 = ve.h_pixel_end_val; // set Compare-hit 
		T2MCR = TMCR_MR0_S; //Stop on MR0
		T2IR=0xFF; //clear all interrupts
		T2PR=ve.h_pixel_clk_pre; //set the prescaler
		T2CTCR=0; //use PCLK as clock source
		T2TCR = TCR_ENABLE;            // Timer2 Enable
		
		T1IR&=~TIR_MR0I;	//clear interrupt
	}
	VICVectAddr=0; //prepare VIC for next Interrupt ???
	
}


//VSYNC
void __attribute__ ((interrupt("IRQ"))) tmr1_interrupt(void)
{
	if(T1IR&TIR_CR0I){ //Timer1 Capture0 Interrupt			
		ve.frame_cnt++;		//increment frame counter
		ve.frame_time+=T1CR0; //increment frame time counter

		if(ve.h_sync_wait > 0){
			T1MR0 = ve.v_sync_wait; //wait until enabling Timer3 (v_pixel_clk)
		}
		
		T3TCR=TCR_RESET;	//stop Timer3 (should be off anyway)
		T1IR&=~TIR_CR0I;	//clear interrupt
	}
	if(T1IR&TIR_MR0I){ //Timer1 Match0 Interrupt
		T3MR0 = ve.v_pixel_end_val; // set Compare-hit 
		T3MCR = TMCR_MR0_S; //Stop on MR0
		T3IR=0xFF; //clear all interrupts
		T3PR=ve.v_pixel_clk_pre; //set the prescaler
		T3CTCR=0; //use PCLK as clock source
		T3TCR=TCR_ENABLE;	//start Timer3

		T1IR&=~TIR_MR0I;	//clear interrupt
	}
	VICVectAddr=0; //prepare VIC for next Interrupt ???
}

void __attribute__ ((interrupt("IRQ"))) rtc_interrupt(void)
{
	IOSET ^= LEDMASK; 	// toggle LED
	uart0Puts("P\n");	
	timeval++;

	ve.frame_cnt_avr+=ve.frame_cnt;
	ve.frame_cnt_avr/=2;

	ve.h_line_cnt_avr+=ve.h_line_cnt;
	ve.h_line_cnt_avr/=2;

	if(ve.frame_cnt_avr >0)
		ve.h_line_time_avr=ve.h_line_time/ve.h_line_cnt_avr;
	
	if(ve.frame_cnt_avr > 0)
		ve.frame_time_avr=ve.frame_time/ve.frame_cnt_avr;

	ve.h_line_cnt=0;
	ve.frame_cnt=0;
	ve.frame_time=0;
	ve.h_line_time=0;

	handlePacket();

	//CTC=0x7C00; //set CTC to 31/32
	ILR|=1;	//reset interrupt 
	VICVectAddr=0; //prepare VIC for next Interrupt

}
