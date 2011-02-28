
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

//#include "lpc210x_gnuarm.h"
#include "lpc2103.h"
#include "config.h"
#include "VIClowlevel.h"
#include "uart.h"
#include "timer.h"
#include "sysTime.h"
#include "videoengine.h"


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

	//--- Enable FastIO pins ---
	//FIO0MASK0 =0;	//active low, enable all Pins
	

}

/* Setup Timer0 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
static void init_timer0 (void) {
    T0CCR = TCCR_CR0_F|TCCR_CR0_I; // Capture Control Register, CAP0.0 Falling Edge+ Int, CAP0.1 Falling Edge+ Int
	T0MCR = TMCR_MR0_I;	//Interrupt on MR0
	T0MR0 = 0xffffff;	//this will never happen
	T0TCR = TCR_ENABLE;            // Timer0 Enable
	
	VICVectAddr0 = (unsigned long)tmr0_interrupt;   // set interrupt vector in 0
	VICVectCntl0 = VIC_ENABLE | VIC_TIMER0; // use it for Timer 0 Match Interrupt:
	VICIntEnable = VIC_BIT(VIC_TIMER0);    // Enable Timer0 Interrupt

	//VICVectAddr1 = (unsigned long)tc0_mat;   // set interrupt vector in 1
	//VICVectCntl1 = VIC_ENABLE | VIC_TIMER0; // use it for Timer 0 Cap Interrupt:
	
}

/* Setup Timer1 Compare-Match Interrupt         */
/* no prescaler timer runs at cclk = FOSC*PLL_M */
static void init_timer1 (void) {
    T1CCR = TCCR_CR0_F|TCCR_CR0_I; // Capture Control Register, CAP0.0 Falling Edge+ Int, CAP0.1 Falling Edge+ Int
	T1MCR = TMCR_MR0_I;	//Interrupt on MR0
	T0MR1 = 0xffffff;	//this will never happen
	T1TCR = TCR_ENABLE;            // Timer1 Enable
	
	VICVectAddr1 = (unsigned long)tmr1_interrupt;   // set interrupt vector in 0
	VICVectCntl1 = VIC_ENABLE | VIC_TIMER1; // use it for Timer 0 Match Interrupt:
	VICIntEnable = VIC_BIT(VIC_TIMER1);    // Enable Timer0 Interrupt
}

/* Setup RTC */
static void init_RTC (void){

	//CTC=0x7C00; //set CTC to 31/32
	CIIR=0x01; //interrupt once a second
	AMR=0xFF; //disable Alarm clk
	CCR=RTC_CLK_SOURCE|RTC_CLK_ENABLE;  //enable rtc, use xtal as source

	VICIntSelect &= ~(1<<VIC_RTC);
	VICVectAddr12=(unsigned long)rtc_interrupt;
	VICVectCntl12=VIC_ENABLE|VIC_RTC;
	VICIntEnable |= VIC_BIT(VIC_RTC);
}

static void gpioInit(void)
{
	IOSET  = (1<<LEDPIN);	// set Bit = LED off (active low)
	IODIR |= (1<<LEDPIN);	// define LED-Pin as output
	//GPIO_IODIR &= ~(1<<SWPIN);	// define Switch-Pin as input
}

static void wait (unsigned long delay)  
{
  unsigned long i;

  i = timeval + delay;
  while ( i != timeval);        
}

int main(void) 
{
	gpioInit();  
	
	IOCLR = LEDMASK; 	// enable LED - "life sign"

	systemInit();			// PLL, MAM etc.
	
	//init_timer0();

	init_RTC();
		
	uart0Init(UART_BAUD(HOST_BAUD0), UART_8N1, UART_FIFO_8); // setup the UART0
	
	uart1Init(UART_BAUD(HOST_BAUD1), UART_8N1, UART_FIFO_8); // setup the UART1

	ve_init();
	
	enableIRQ(); 
	
	
	//wait(WAIT100MS);
	uart0Puts("Hallo I'm lpc2103\n");	
	
	wait(1);
	IOSET = LEDMASK; 	// disable LED
	wait(1);

	while(1) {
		//draw
#if 0
		if((T1TCR=1)&&(T2TCR=1)){	
			i=T2TC+(T3TC*ve.h_pixel_end_val); //calc next position in RAM
			FIO0PIN0 = ve.videobuffer[i];	//move from RAM to PORT
		}
#else
		wait(1);
#endif
	}
}
