/* System Config */

/* 
	PLL
	
   - Main clock F_OSC=14,7MHz @ Olimex LPC-P2106) [limits: 10 MHz to 25 MHz]
   - System should run at max. Frequency (60MHz) [limit: max 60 MHz]
   - Choose multiplier M=4 
     so cclk = M * F_OSC= 4 * 14745000Hz = 58980000 Hz
   - MSEL-Bit in PLLCFG (bits 0-4) MSEL = M-1
   - F_CCO must be inbetween the limits 156 MHz to 320 MHz
     datasheet: F_CCO = F_OSC * M * 2 * P
   - choose devider P=2 => F_CCO = 14745000Hz * 4 * 2 * 2
     = 235920000 ~=236 MHz
   - PSEL0 (Bit5 in PLLCFG) = 1, PSEL1 (Bit6) = 0 (0b01)
*/


#define PSEL0 		5
#define PSEL1 		6

#define PLLE		0
#define PLLC		1
#define WDOG()

// PLL setup values are computed within the LPC include file
// It relies upon the following defines
#define FOSC				18432000  // Master Oscillator Freq.
#define PLL_MUL             (3)         // PLL Multiplier
#define CCLK                (FOSC * PLL_MUL) // CPU Clock Freq.
#define MSEL				(PLL_MUL-1)

// Pheripheral Bus Speed Divider
#define PBSD                2           // MUST BE 1, 2, or 4
#define PCLK                (CCLK / PBSD) // Pheripheal Bus Clock Freq.

// Do some value range testing
#if ((FOSC < 10000000) || (FOSC > 25000000))
#error Fosc out of range (10MHz-25MHz)
#error correct and recompile
#endif

#if ((CCLK < 10000000) || (CCLK > 60000000))
#error cclk out of range (10MHz-60MHz)
#error correct PLL_MUL and recompile
#endif

#if ((FCCO < 150000000) || (FCCO > 320000000))
#error Fcco out of range (156MHz-320MHz)
#error internal algorithm error
#endif

#if ((PBSD != 1) && (PBSD != 2) && (PBSD != 4))
#error Pheripheal Bus Speed Divider (PBSD) illegal value (1, 2, or 4)
#endif
#define PLOCK		10

#define PLL_FEED1	0xAA
#define PLL_FEED2	0x55


/*
	MAM(Memory Accelerator Module)
	- choosen: MAM fully enabled = MAM-Mode 2
	- System-Clock cclk=59MHz -> 3 CCLKs are proposed as fetch timing
*/
#define MAM_MODE 	2
#define MAM_FETCH   3
	
/*
	VPB (V... Pheriphal Bus)
	- choosen: VPB should run at half speed -> devider VPBDIV=2
	=> pclk = 35Mhz
*/
#define VPBDIV_VAL	2

/* 
	SCB
*/
#define MEMMAP_BOOT_LOADER_MODE   0       // Interrupt vectors are re-mapped to Boot Block.
#define MEMMAP_USER_FLASH_MODE    (1<<0)  // Interrupt vectors are not re-mapped and reside in Flash.
#define MEMMAP_USER_RAM_MODE      (1<<1)  // Interrupt vectors are re-mapped to Static RAM.

/* UART0 BAUDRATE */
#define HOST_BAUD0 921600
/* UART1 BAUDRATE */
#define HOST_BAUD1 115200
/* Video engine */
#define VIDEO_BUFFER_SIZE 4096  //set video buffer size
/*define OUTPORT*/
#define OUTPORT FIO0PIN

#define LEDPIN  14
#define LEDMASK (1<<LEDPIN);

#define WAIT33MS   1
#define WAIT100MS  3
#define WAIT1S     32

