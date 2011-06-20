
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
#include "armVIC.h"
#include "uart.h"
#include "timer.h"
#include "sysTime.h"
#include "videoengine.h"
#include "iccom.h"
#include "menu.h"

#define MODE_VIDEOMODE 255
#define MODE_TESTMODE 1

extern volatile Videoengine_t ve;
//extern volatile unsigned long timeval;

void dovideo(void);

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

	init_RTC();
	init_timer0();
	init_timer1();
	init_timer2();
	init_timer3();
	
	uart0Init(UART_BAUD(HOST_BAUD0), UART_8N1, UART_FIFO_8); // setup the UART0
	
	uart1Init(UART_BAUD(HOST_BAUD1), UART_8N1, UART_FIFO_8); // setup the UART1

	enableIRQ(); 
	enableFIQ();
	
	menu_init_entry((void *)&mode,sizeof(mode),"mode",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_sync_wait_perc,sizeof(ve.h_sync_wait_perc),"h_sync_wait %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.v_sync_wait_perc,sizeof(ve.v_sync_wait_perc),"v_sync_wait %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_pixel_perc,sizeof(ve.h_pixel_perc),"h_viewfield %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.v_pixel_perc,sizeof(ve.v_pixel_perc),"v_viewfield %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.h_pixel_end_val,sizeof(ve.h_pixel_end_val),"h_pixel %",MENU_READANDWRITE);
	menu_init_entry((void *)&ve.v_pixel_end_val,sizeof(ve.v_pixel_end_val),"v_pixel %",MENU_READANDWRITE);

	menu_init_entry((void *)&testvar,sizeof(testvar),"testvar",MENU_READANDWRITE);
	menu_init_entry((void *)&OCR1A,sizeof(OCR1A),"Timer 1 V",MENU_READANDWRITE);
	menu_init_entry((void *)&OCR2,sizeof(OCR2),"Timer2 H",MENU_READANDWRITE);
	menu_init_entry((void *)&timeval,sizeof(timeval),"time    ",MENU_READONLY);

	menu_init_entry((void *)&ve.h_line_cnt_avr,sizeof(ve.h_line_cnt_avr),"h_line_cnt_avr",MENU_READONLY);
	menu_init_entry((void *)&ve.frame_cnt_avr,sizeof(ve.frame_cnt_avr),"frame_cnt_avr",MENU_READONLY);
	
	menu_init_entry((void *)&ve.h_line_time_avr ,sizeof(ve.h_line_time_avr ),"linetimeavr ",MENU_READONLY);
	menu_init_entry((void *)&ve.frame_time_avr,sizeof(ve.frame_time_avr),"frametimeavr",MENU_READONLY);

	menu_init_entry((void *)&ve.h_pixel_clk_pre,sizeof(ve.h_pixel_clk_pre),"h_pixelclkpre",MENU_READONLY);
	menu_init_entry((void *)&ve.v_pixel_clk_pre,sizeof(ve.v_pixel_clk_pre),"v_pixelclkpre",MENU_READONLY);

	menu_init_entry((void *)&ve.h_sync_wait,sizeof(ve.h_sync_wait),"hsyncwait",MENU_READONLY);
	menu_init_entry((void *)&ve.v_sync_wait,sizeof(ve.v_sync_wait),"vsyncwait",MENU_READONLY);

	
	icc_init((void *)&mode,sizeof(mode),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.h_sync_wait_perc,sizeof(ve.h_sync_wait_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.v_sync_wait_perc,sizeof(ve.v_sync_wait_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.h_pixel_perc,sizeof(ve.h_pixel_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.v_pixel_perc,sizeof(ve.v_pixel_perc),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.h_pixel_end_val,sizeof(ve.h_pixel_end_val),ICC_DIRECTION_BOTH);
	icc_init((void *)&ve.v_pixel_end_val,sizeof(ve.v_pixel_end_val),ICC_DIRECTION_BOTH);


	mode =5;
	ve.h_sync_wait_perc=55;
	ve.v_sync_wait_perc=70;
	ve.h_pixel_perc=70;
	ve.v_pixel_perc=70;
	ve.h_pixel_end_val=32;
	ve.v_pixel_end_val=32;
//	icc_init(&ve.h_line_cnt_avr);
//	icc_init(&ve.frame_cnt_avr);
//	icc_init(&testvar);
	wait(1);
	
	uart0Puts("Hallo I'm lpc2103\n");
	uart0Puts("laserprojector by siro 2011\n");	
	uart0Puts("waiting for init\n");

	menu_init();
	
	while((ve.v_pixel_end_val == 0) || (ve.h_pixel_end_val == 0) || (ve.h_pixel_perc == 0) || (ve.v_pixel_perc == 0) || (mode == 0))
	{
		icc_send(menu_update());
		if(uart1rxqueuelength())	//check if UART1 buffer is not empty
			if(icc_handle_uart())	//check if any value changed icc_init;
				ve_init();			//re-init videoengine	
	}
	
	ve_init();	//init videoengine
	
	uart0Puts("main loop\n");	
	
	while(1) {
		
		if(mode < MODE_VIDEOMODE)
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
				OUTPORT = 0xff;	//high
				while(T2TCR&TCR_ENABLE)
					asm("nop");
					OUTPORT = 0x00;	//low
				}
			}
			if(mode == 4)
			{
				if(T3TCR&TCR_ENABLE)	//check if Timer3 is enabled
					OUTPORT = 0xff;	//high
				else
					OUTPORT = 0x00;	//low
			}
			if(mode == 5)
			{
			
				if(T3TCR&TCR_ENABLE){	//check if Timer3 is enabled
				
					//disable Interrupt:
					while(T2TCR&TCR_ENABLE){
					
						//calc next position in RAM
						if (T2TC & 0x01)
							OUTPORT = 0;	//low
						else
							OUTPORT = 0xff;	//high
					
					}
					OUTPORT=0x0;
				}	
			}
			if(mode == 6)
			{
			
				if(T3TCR&TCR_ENABLE){	//check if Timer3 is enabled
				
					//disable Interrupt:
					while(T2TCR&TCR_ENABLE){
					
						//calc next position in RAM
						if (T3TC & 0x0001)
							OUTPORT = 0;	//low
						else
							OUTPORT = 0xff;	//high
					
					}
					OUTPORT=0x0;
				}	
			}
			if(mode == 7)
			{
			
				if((T3TCR&T2TCR&TCR_ENABLE)){	//check if Timer3&Timer2 is enabled
					//disable RTC
					VICVectCntl12 = 0;
					VICIntEnClear|= VIC_BIT(VIC_RTC);	//disable Interrupt

					//disable UART0
					VICIntEnClear|= VIC_BIT(VIC_UART0); //disable Interrupt
		  			VICVectCntl10 = 0;
				
					while(T2TCR&TCR_ENABLE){
						//calc next position in RAM
						if ((T2TC & 0x0001 & T3TC))
							OUTPORT = 0;	//low
						else
							OUTPORT = 0xff;	//high
					}
				
					//enable RTC
					VICVectCntl12= VIC_ENABLE|VIC_RTC;
					VICIntEnable |= VIC_BIT(VIC_RTC);
					//enable UART0
					
					VICIntEnable |= VIC_BIT(VIC_UART0);
		  			VICVectCntl10 = VIC_ENABLE|VIC_UART0;
				
					OUTPORT=0x0;
				}	
			}
		}
		else if(mode == MODE_VIDEOMODE)
		{
			if((T3TCR&TCR_ENABLE)&&(T2TCR&TCR_ENABLE)){	//check if Timer3 & Timer2 is enabled
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
				
				dovideo();	//video routine 
				
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
		
		menu_changed=menu_update();
		if(menu_changed != NULL)
			icc_send(menu_changed);
		
		if(uart1rxqueuelength())	//check if UART1 buffer is not empty
			icc_handle_uart();	//check if any value changed icc_init(&ve.h_pixel_perc);
		
	}
}

void dovideo(void)
{
	REG32 temph = 0;
	while(T2TCR&TCR_ENABLE){
		CLK_LOW;	//disable clock
		//calc next position in RAM
		temph = (T3TC*ve.h_pixel_end_val);
		OUTPORT = ve.videobuffer[T2TC+temph];	//move from RAM to PORT
		CLK_HIGH;	//enable clock
	}
}
