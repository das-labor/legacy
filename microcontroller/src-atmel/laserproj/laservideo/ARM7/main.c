
/******************************************************************************/
/*  lpc_blink_switch_irq                                                      */
/*  LED Flasher - Timer Interrupt example for arm-elf-gcc                     */
/*  Lights LED on Startup and flashes LED when button is pressed.              */
/******************************************************************************/
/*  Inspired by a sample application from Keil Elektronik                     */
/*  A lot of information has been found in a sample from R O Software.        */
/******************************************************************************/
/*  Sample for WinARM by M.Thomas <eversmith@heizung-thomas.de>               */
/******************************************************************************/
/* Timer0 OCR ICVectAddr1*/
/* Timer0 MAT ICVectAddr2*/

/* UART0 ICVectAddr10*/
/* UART1 ICVectAddr11*/
/* Changelog: Demo for thumb-interwork 17. Feb. 2005 */

#define ___ARM_lpc2103__

//#include "lpc210x_gnuarm.h"
#include <stdlib.h>
#include "lpc2103.h"
#include "config.h"
#include "VIClowlevel.h"
#include "uart.h"
#include "timer.h"
#include "sysTime.h"
#include "videoengine.h"
#include "iccom.h"
#include "menu.h"

#define MODE_VIDEOMODE 255
#define MODE_TESTMODE 1

extern volatile Videoengine_t ve;
extern volatile unsigned long timeval;
static void systemInit(void)
{
	// --- enable and connect the PLL (Phase Locked Loop) ---
	// a. set multiplier and divider
	PLLCFG = MSEL | (1<<PSEL1) | (0<<PSEL0);
	// b. enable PLL
	PLLCON = (1<<PLLE);
	// c. feed sequence
	PLLFEED = PLL_FEED1;
	PLLFEED = PLL_FEED2;
	// d. wait for PLL lock (PLOCK bit is set if locked)
	while ( !( PLLSTAT & (1<<PLOCK) ) );
	// e. connect (and enable) PLL
	PLLCON = (1<<PLLE) | (1<<PLLC);
	// f. feed sequence
	PLLFEED = PLL_FEED1;
	PLLFEED = PLL_FEED2;
	
	// --- setup and enable the MAM (Memory Accelerator Module) ---
	// a. start change by turning of the MAM (redundant)
	MAMCR = 0;	
	// b. set MAM-Fetch cycle to 3 cclk as recommended for >40MHz
	MAMTIM = MAM_FETCH;
	// c. enable MAM 
	MAMCR = MAM_MODE;

	// --- set VPB speed ---
	VPBDIV = VPBDIV_VAL;

    // --- map INT-vector ---
	#if defined(RAM_RUN)
	  MEMMAP = MEMMAP_USER_RAM_MODE;
	#elif defined(ROM_RUN)
	  MEMMAP = MEMMAP_USER_FLASH_MODE;
	#else
	#error RUN_MODE not defined!
	#endif
		
	//--- clear Interrupts ---	
	VICIntEnClr = 0xFFFFFFFF;           // clear all interrupts
  	VICIntSelect = 0x00000000;            // clear all FIQ selections

}

/* Setup Timer0 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
static void init_timer0 (void) {
	PINSEL0 |=(1 << 9);	//set Pin0.4 as T0CR1 input
	PINSEL0 &=~(1<<8);
    T0CCR = TCCR_CR1_F|TCCR_CR1_I; // Capture Control Register, CAP0.1 Falling Edge+ Int
	T0MCR = 0;	//clear match interrupt
	T0MR0 = 0x000fffff;	//this will never happen
	T0IR=0xff; //clear all interrupts
	T0PR=0; //prescaler = 0
	T0TCR = TCR_ENABLE;            // Timer0 Enable

	VICVectAddr0 = (unsigned long)tmr0_interrupt;   // set interrupt vector in 0
	VICVectCntl0 = VIC_ENABLE | VIC_TIMER0; // use it for Timer 0 Match Interrupt:
	VICIntEnable = VIC_BIT(VIC_TIMER0);    // Enable Timer0 Interrupt
	
}

/* Setup Timer1 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
static void init_timer1 (void) {
	PINSEL0 |=(1 << 21);	//set Pin0.10 as T1CR0 input
	PINSEL0 &=~(1<<20);
	
    T1CCR = TCCR_CR0_F|TCCR_CR0_I; // Capture Control Register, CAP1.0 Falling Edge+ Int
	T1MCR =  0;	//clear match interrupt
	T1PR=0; //prescaler = 0
	T1MR0 = 0x000fffff;	//this will never happen
	T1IR= 0xff; //clear all interrutps
	T1TCR = TCR_ENABLE;            // Timer1 Enable
	
	VICVectAddr1 = (unsigned long)tmr1_interrupt;   // set interrupt vector in 1
	VICVectCntl1 = VIC_ENABLE | VIC_TIMER1; // use it for Timer 0 Match Interrupt:
	VICIntEnable = VIC_BIT(VIC_TIMER1);    // Enable Timer0 Interrupt
}

/* Setup RTC */
static void init_RTC (void){

	//CTC=0x7C00; //set CTC to 31/32
	CIIR=0x01; //interrupt once a second
	AMR=0xFF; //disable Alarm clk
	CCR=RTC_CLK_SOURCE|RTC_CLK_ENABLE;  //enable rtc, use xtal as source

	VICIntSelect &= ~(1<<VIC_RTC);		//set Interrupt is IRQ
	VICVectAddr12=(unsigned long)rtc_interrupt;
	VICVectCntl12=VIC_ENABLE|VIC_RTC;
	VICIntEnable |= VIC_BIT(VIC_RTC);	//enable RTC Interrupt
}

static void fpioInit(void)
{
	//--- Enable FastIO pins ---
	SCS=1; //enable fast IO
	FIO0DIRL |= LEDMASK;
	FIOMASK=0;	//all pins are fast IO
	FIO0DIRU = 0x01FF; //Pin 15-24 output
}

static void wait (unsigned long delay)  
{
  unsigned long i;

  i = timeval + delay;
  while ( i != timeval);        
}

int main(void)
{
	//needs to be filled from ICCom
	uint8_t mode=0;
	uint16_t testvar=1;
	uint8_t OCR2=0;		//for motor control
	uint16_t OCR1A=0;	//for motor control
	void* menu_changed=NULL;
	
	fpioInit();
	
	systemInit();			// PLL, MAM etc.

	CLK_HIGH;
	OUTPORT=0xff;
	CLK_LOW;

	init_RTC();
	init_timer0();
	init_timer1();
	
	uart0Init(UART_BAUD(HOST_BAUD0), UART_8N1, UART_FIFO_8); // setup the UART0
	
	uart1Init(UART_BAUD(HOST_BAUD1), UART_8N1, UART_FIFO_8); // setup the UART1

	enableIRQ(); 

	menu_init_entry((void *)&mode,sizeof(mode),"mode",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_sync_wait_perc,sizeof(ve.h_sync_wait_perc),"h_sync_wait %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.v_sync_wait_perc,sizeof(ve.v_sync_wait_perc),"v_sync_wait %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_pixel_perc,sizeof(ve.h_pixel_perc),"h_viewfield %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.v_pixel_perc,sizeof(ve.v_pixel_perc),"v_viewfield %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_pixel_end_val,sizeof(ve.h_pixel_end_val),"h_pixel %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.v_pixel_end_val,sizeof(ve.v_pixel_end_val),"v_pixel %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_line_cnt_avr,sizeof(ve.h_line_cnt_avr),"h lines ps",MENU_READONLY);
	menu_init_entry((void *)&ve.frame_cnt_avr,sizeof(ve.frame_cnt_avr),"frames ps",MENU_READONLY);
	menu_init_entry((void *)&testvar,sizeof(testvar),"testvar",MENU_READANDWRITE);
	menu_init_entry((void *)&OCR1A,sizeof(OCR1A),"Timer 1 V",MENU_READANDWRITE);
	menu_init_entry((void *)&OCR2,sizeof(OCR2),"Timer2 H",MENU_READANDWRITE);
	menu_init_entry((void *)&timeval,sizeof(timeval),"time    ",MENU_READONLY);
	menu_init_entry((void *)&ve.h_line_cnt_avr,sizeof(ve.h_line_cnt_avr),"h_line_cnt_avr",MENU_READONLY);
	menu_init_entry((void *)&ve.frame_cnt_avr,sizeof(ve.frame_cnt_avr),"frame_cnt_avr",MENU_READONLY);
	
	menu_init_entry((void *)&ve.h_pixel_clk_pre,sizeof(ve.h_line_cnt_avr),"h_pixelclkpre",MENU_READONLY);
	menu_init_entry((void *)&ve.v_pixel_clk_pre,sizeof(ve.frame_cnt_avr),"v_pixelclkpre",MENU_READONLY);

	
	icc_init((void *)&mode,sizeof(mode),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.h_sync_wait_perc,sizeof(ve.h_sync_wait_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.v_sync_wait_perc,sizeof(ve.v_sync_wait_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.h_pixel_perc,sizeof(ve.h_pixel_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.v_pixel_perc,sizeof(ve.v_pixel_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.h_pixel_end_val,sizeof(ve.h_pixel_end_val),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.v_pixel_end_val,sizeof(ve.v_pixel_end_val),ICC_DIRECTION_BOTH);


	mode =3;
	ve.h_sync_wait_perc=10;
	ve.v_sync_wait_perc=10;
	ve.h_pixel_perc=30;
	ve.v_pixel_perc=30;
	ve.h_pixel_end_val=64;
	ve.v_pixel_end_val=64;
//	icc_init(&ve.h_line_cnt_avr);
//	icc_init(&ve.frame_cnt_avr);
//	icc_init(&testvar);
	wait(1);
	
	uart0Puts("Hallo I'm lpc2103\n");
	uart0Puts("laserprojector by siro 2011\n");	
	uart0Puts("waiting for init\n");
	int c=0;
	menu_init();
	
	while((ve.v_pixel_end_val == 0) || (ve.h_pixel_end_val == 0) || (ve.h_pixel_perc == 0) || (ve.v_pixel_perc == 0) || (mode == 0))
	{
		icc_send(menu_update());
		if(uart1rxqueuelength())	//check if UART1 buffer is not empty
			if(icc_handle_uart())	//check if any value changed icc_init;
				ve_init();			//re-init videoengine	
	}
	ve_init();
	while(mode < MODE_VIDEOMODE)
	{
		if(mode == 1)
		{
			CLK_LOW;	//disable clock
			//calc next position in RAM
			OUTPORT = testvar;	//move from RAM to PORT
			CLK_HIGH;	//enable clock

		}
		if(mode == 2)
		{
			//calc next position in RAM
			for(uint16_t i=0;i<testvar;i++)
			{
				CLK_LOW;	//disable clock
				OUTPORT = 0xff;	//move from RAM to PORT
				CLK_HIGH;	//enable clock
				wait(1);
				CLK_LOW;	//disable clock
				OUTPORT = 0x00;	//move from RAM to PORT
				CLK_HIGH;	//enable clock
				wait(1);
			}
		}
		if(mode == 3)
		{
			if(T2TCR&TCR_ENABLE){
				OUTPORT = 0x0;	//high
			}
		}
		if(mode == 4)
		{
			if(T3TCR&TCR_ENABLE){	//check if Timer3 is enabled
			OUTPORT = 0x0;	//high
			}
			else
			{
			OUTPORT = 0xff;	//high

			}
		}
		if(mode == 5)
		{
			if(T3TCR&TCR_ENABLE){	//check if Timer3 is enabled
				//disable Interrupt:
				while(T2TCR&TCR_ENABLE){
					CLK_LOW;	//disable clock
					//calc next position in RAM
					if (T2TC & 0x01)
						OUTPORT = 0;	//low
					else
						OUTPORT = 0xff;	//high
					CLK_HIGH;	//enable clock
				}
				OUTPORT=0xff;
			}	
		}
		menu_changed=menu_update();
		if(menu_changed != NULL)
		{
			icc_send(menu_changed);
			ve_init();
		}
		if(uart1rxqueuelength())	//check if UART1 buffer is not empty
			if(icc_handle_uart())	//check if any value changed icc_init(&ve.h_pixel_perc);
			{
				ve_init();			//re-init videoengine
			}
	}
	ve_init();	//init videoengine
	
	uart0Puts("main loop\n");	
	
	while(1) {
		if(!mode)
		{
			//do nothing
			wait(1);
		}
		else if(mode == MODE_VIDEOMODE)
		{
			if(T3TCR&TCR_ENABLE){	//check if Timer3 is enabled
				//disable Interrupt:
				//disable RTC
				VICVectCntl12=0;
				VICIntEnClear|= VIC_BIT(VIC_RTC);	//disable Interrupt

				//disable UART0
				VICIntEnClear|= VIC_BIT(VIC_UART0); //disable Interrupt
	  			VICVectCntl10 = 0;

				//disable UART1
				VICIntEnClear|= VIC_BIT(VIC_UART1); //disable Interrupt
	  			VICVectCntl11 = 0;
				while(T2TCR&TCR_ENABLE){
					CLK_LOW;	//disable clock
					//calc next position in RAM
					OUTPORT = ve.videobuffer[T2TC+(T3TC*ve.h_pixel_end_val)];	//move from RAM to PORT
					CLK_HIGH;	//enable clock
				}
				//enable Interrupt:
				//enable RTC
				VICVectCntl12=VIC_ENABLE|VIC_RTC;
				VICIntEnable |= VIC_BIT(VIC_RTC);
				//enavle UART1
				 VICVectCntl11 = VIC_ENABLE | VIC_UART1;
  				VICIntEnable |= VIC_BIT(VIC_UART1);
				//enable UART0
				VICIntEnable |= VIC_BIT(VIC_UART0);
	  			VICVectCntl10 =VIC_ENABLE|VIC_UART0;
			
			}	

		}
		if(uart1rxqueuelength())	//check if UART1 buffer is not empty
			if(icc_handle_uart())	//check if any value changed icc_init(&ve.h_pixel_perc);
			{
				ve_init();			//re-init videoengine
			}
	}
}
