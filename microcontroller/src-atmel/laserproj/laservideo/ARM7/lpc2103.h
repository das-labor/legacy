// Processor register mappings for Philips LPC2103. Should also work fine
// for LPC2102 and LPC2101.
//
// Copyright (c) 2006, K9spud LLC.
// http://www.k9spud.com/arm/lpc2103/
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// As a special exception, if other files instantiate templates or
// use macros or inline functions from this file, or you compile this
// file and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered
// by the GNU General Public License. However the source code for this
// file must still be made available in accordance with section (3) of
// the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
//
// $Log: lpc2103.h,v $
// Revision 1.5  2006/05/04 05:01:05  edwards
// Added some more FIO access registers.
//
// Revision 1.4  2006/04/14 06:22:10  edwards
// Added ADC registers.
//
// Revision 1.3  2006/04/14 00:43:29  edwards
// Added WDT registers.
//
// Revision 1.2  2006/04/14 00:02:43  edwards
// Added RTC registers.
//
// Revision 1.1  2006/04/13 06:31:38  edwards
// Initial revision.
//

#ifndef __LPC2103_H
#define __LPC2103_H

#define REG_8           volatile unsigned char
#define REG16           volatile unsigned short
#define REG32           volatile unsigned long

#include "lpcWD.h"
#include "lpcTMR.h"
#include "lpcUART.h"
#include "lpcI2C.h"
#include "lpcSPI.h"
#include "lpcRTC.h"
#include "lpcGPIO.h"
#include "lpcPIN.h"
#include "lpcADC.h"
#include "lpcSCB.h"
#include "lpcVIC.h"

/* Vectored Interrupt Controller (VIC) */
#define VICIRQStatus	(*((volatile unsigned long *) 0xFFFFF000))
#define VICFIQStatus	(*((volatile unsigned long *) 0xFFFFF004))
#define VICRawIntr	(*((volatile unsigned long *) 0xFFFFF008))
#define VICIntSelect	(*((volatile unsigned long *) 0xFFFFF00C))
#define VICIntEnable	(*((volatile unsigned long *) 0xFFFFF010))
#define VICIntEnClr	(*((volatile unsigned long *) 0xFFFFF014))
#define VICIntEnClear (*((volatile unsigned long *) 0xFFFFF014))
#define VICSoftInt	(*((volatile unsigned long *) 0xFFFFF018))
#define VICSoftIntClr	(*((volatile unsigned long *) 0xFFFFF01C))
#define VICProtection	(*((volatile unsigned long *) 0xFFFFF020))
#define VICVectAddr	(*((volatile unsigned long *) 0xFFFFF030))
#define VICDefVectAddr	(*((volatile unsigned long *) 0xFFFFF034))
#define VICVectAddr0	(*((volatile unsigned long *) 0xFFFFF100))
#define VICVectAddr1	(*((volatile unsigned long *) 0xFFFFF104))
#define VICVectAddr2	(*((volatile unsigned long *) 0xFFFFF108))
#define VICVectAddr3	(*((volatile unsigned long *) 0xFFFFF10C))
#define VICVectAddr4	(*((volatile unsigned long *) 0xFFFFF110))
#define VICVectAddr5	(*((volatile unsigned long *) 0xFFFFF114))
#define VICVectAddr6	(*((volatile unsigned long *) 0xFFFFF118))
#define VICVectAddr7	(*((volatile unsigned long *) 0xFFFFF11C))
#define VICVectAddr8	(*((volatile unsigned long *) 0xFFFFF120))
#define VICVectAddr9	(*((volatile unsigned long *) 0xFFFFF124))
#define VICVectAddr10	(*((volatile unsigned long *) 0xFFFFF128))
#define VICVectAddr11	(*((volatile unsigned long *) 0xFFFFF12C))
#define VICVectAddr12	(*((volatile unsigned long *) 0xFFFFF130))
#define VICVectAddr13	(*((volatile unsigned long *) 0xFFFFF134))
#define VICVectAddr14	(*((volatile unsigned long *) 0xFFFFF138))
#define VICVectAddr15	(*((volatile unsigned long *) 0xFFFFF13C))
#define VICVectCntl0	(*((volatile unsigned long *) 0xFFFFF200))
#define VICVectCntl1	(*((volatile unsigned long *) 0xFFFFF204))
#define VICVectCntl2	(*((volatile unsigned long *) 0xFFFFF208))
#define VICVectCntl3	(*((volatile unsigned long *) 0xFFFFF20C))
#define VICVectCntl4	(*((volatile unsigned long *) 0xFFFFF210))
#define VICVectCntl5	(*((volatile unsigned long *) 0xFFFFF214))
#define VICVectCntl6	(*((volatile unsigned long *) 0xFFFFF218))
#define VICVectCntl7	(*((volatile unsigned long *) 0xFFFFF21C))
#define VICVectCntl8	(*((volatile unsigned long *) 0xFFFFF220))
#define VICVectCntl9	(*((volatile unsigned long *) 0xFFFFF224))
#define VICVectCntl10	(*((volatile unsigned long *) 0xFFFFF228))
#define VICVectCntl11	(*((volatile unsigned long *) 0xFFFFF22C))
#define VICVectCntl12	(*((volatile unsigned long *) 0xFFFFF230))
#define VICVectCntl13	(*((volatile unsigned long *) 0xFFFFF234))
#define VICVectCntl14	(*((volatile unsigned long *) 0xFFFFF238))
#define VICVectCntl15	(*((volatile unsigned long *) 0xFFFFF23C))

/* Pin Function Select */
#define PINSEL0		(*((volatile unsigned long *) 0xE002C000))
#define PINSEL1		(*((volatile unsigned long *) 0xE002C004))

/* Legacy General Purpose Input/Output (GPIO) */
#define IOPIN		(*((volatile unsigned long *) 0xE0028000))
#define IOSET		(*((volatile unsigned long *) 0xE0028004))
#define IODIR		(*((volatile unsigned long *) 0xE0028008))
#define IOCLR		(*((volatile unsigned long *) 0xE002800C))

/* Fast General Purpose Input/Output (GPIO) */
#define FIODIR		(*((volatile unsigned long *) 0x3FFFC000))
#define FIOMASK		(*((volatile unsigned long *) 0x3FFFC010))
#define FIOPIN		(*((volatile unsigned long *) 0x3FFFC014))
#define FIOSET		(*((volatile unsigned long *) 0x3FFFC018))
#define FIOCLR		(*((volatile unsigned long *) 0x3FFFC01C))

#define FIO0DIR0	(*((volatile unsigned char *) 0x3FFFC000))
#define FIO0DIR1	(*((volatile unsigned char *) 0x3FFFC001))
#define FIO0DIR2	(*((volatile unsigned char *) 0x3FFFC002))
#define FIO0DIR3	(*((volatile unsigned char *) 0x3FFFC003))
#define FIO0DIRL	(*((volatile unsigned short*) 0x3FFFC000))
#define FIO0DIRU	(*((volatile unsigned short*) 0x3FFFC002))

#define FIO0MASK0	(*((volatile unsigned char *) 0x3FFFC010))
#define FIO0MASK1	(*((volatile unsigned char *) 0x3FFFC011))
#define FIO0MASK2	(*((volatile unsigned char *) 0x3FFFC012))
#define FIO0MASK3	(*((volatile unsigned char *) 0x3FFFC013))
#define FIO0MASKL	(*((volatile unsigned short*) 0x3FFFC010))
#define FIO0MASKU	(*((volatile unsigned short*) 0x3FFFC012))

#define FIO0PIN0	(*((volatile unsigned char *) 0x3FFFC014))
#define FIO0PIN1	(*((volatile unsigned char *) 0x3FFFC015))
#define FIO0PIN2	(*((volatile unsigned char *) 0x3FFFC016))
#define FIO0PIN3	(*((volatile unsigned char *) 0x3FFFC017))
#define FIO0PINL	(*((volatile unsigned short*) 0x3FFFC014))
#define FIO0PINU	(*((volatile unsigned short*) 0x3FFFC016))

#define FIO0SET0	(*((volatile unsigned char *) 0x3FFFC018))
#define FIO0SET1	(*((volatile unsigned char *) 0x3FFFC019))
#define FIO0SET2	(*((volatile unsigned char *) 0x3FFFC01A))
#define FIO0SET3	(*((volatile unsigned char *) 0x3FFFC01B))
#define FIO0SETL	(*((volatile unsigned short*) 0x3FFFC018))
#define FIO0SETU	(*((volatile unsigned short*) 0x3FFFC01A))

#define FIO0CLR0	(*((volatile unsigned char *) 0x3FFFC01C))
#define FIO0CLR1	(*((volatile unsigned char *) 0x3FFFC01D))
#define FIO0CLR2	(*((volatile unsigned char *) 0x3FFFC01E))
#define FIO0CLR3	(*((volatile unsigned char *) 0x3FFFC01F))
#define FIO0CLRL	(*((volatile unsigned short*) 0x3FFFC01C))
#define FIO0CLRU	(*((volatile unsigned short*) 0x3FFFC01E))

///////////////////////////////////////////////////////////////////////////////
// Universal Asynchronous Receiver Transmitter 0 (UART0)
#define UART0           ((uartRegs_t *)0xE000C000)
#define U0_PINSEL       (0x00000005)    /* PINSEL0 Value for UART0 */
#define U0_PINMASK      (0x0000000F)    /* PINSEL0 Mask for UART0 */

/* Universal Asynchronous Receiver/Transmitter 0 (UART0) */
#define U0RBR		(*((volatile unsigned char *) 0xE000C000)) /* rx buffer reg. (R/O when DLAB=0) */
#define U0THR		(*((volatile unsigned char *) 0xE000C000)) /* tx holding reg. (W/O when DLAB=0) */
#define U0DLL		(*((volatile unsigned char *) 0xE000C000)) /* divisor latch LSB (R/W when DLAB=1) */
#define U0DLM		(*((volatile unsigned char *) 0xE000C004)) /* divisor latch MSB (R/W when DLAB=1) */
#define U0IER		(*((volatile unsigned char *) 0xE000C004)) /* interrupt enable reg. (R/W when DLAB=0) */
#define U0IIR		(*((volatile unsigned char *) 0xE000C008)) /* interrupt ID reg. */
#define U0FCR		(*((volatile unsigned char *) 0xE000C008)) /* FIFO control reg. */
#define U0LCR		(*((volatile unsigned char *) 0xE000C00C)) /* line control reg. */
#define U0LSR		(*((volatile unsigned char *) 0xE000C014)) /* line status reg. */
#define U0SCR		(*((volatile unsigned char *) 0xE000C01C)) /* scratch pad reg. */
#define U0ACR		(*((volatile unsigned long *) 0xE000C020)) /* auto-baud control reg. */
#define U0FDR		(*((volatile unsigned char *) 0xE000C028)) /* fractional divider reg. */
#define U0TER		(*((volatile unsigned char *) 0xE000C030)) /* tx enable reg. */

///////////////////////////////////////////////////////////////////////////////
// Universal Asynchronous Receiver Transmitter 1 (UART1)
#define UART1           ((uartRegs_t *)0xE0010000)
#define U1_PINSEL       (0x00050000)    /* PINSEL0 Value for UART1 */
#define U1_PINMASK      (0x000F0000)    /* PINSEL0 Mask for UART1 */

/* Universal Asynchronous Receiver/Transmitter 1 (UART1) */
#define U1RBR		(*((volatile unsigned char *) 0xE0010000)) /* rx buffer reg. (R/O when DLAB=0) */
#define U1THR		(*((volatile unsigned char *) 0xE0010000)) /* tx holding reg. (W/O when DLAB=0) */
#define U1DLL		(*((volatile unsigned char *) 0xE0010000)) /* divisor latch LSB (R/W when DLAB=1) */
#define U1DLM		(*((volatile unsigned char *) 0xE0010004)) /* divisor latch MSB (R/W when DLAB=1) */
#define U1IER		(*((volatile unsigned char *) 0xE0010004)) /* interrupt enable reg. (R/W when DLAB=0) */
#define U1IIR		(*((volatile unsigned char *) 0xE0010008)) /* interrupt ID reg. */
#define U1FCR		(*((volatile unsigned char *) 0xE0010008)) /* FIFO control reg. */
#define U1LCR		(*((volatile unsigned char *) 0xE001000C)) /* line control reg. */
#define U1MCR		(*((volatile unsigned char *) 0xE0010010)) /* modem control reg. */
#define U1LSR		(*((volatile unsigned char *) 0xE0010014)) /* line status reg. */
#define U1MSR		(*((volatile unsigned char *) 0xE0010018)) /* modem status reg. */
#define U1SCR		(*((volatile unsigned char *) 0xE001001C)) /* scratch pad reg. */
#define U1ACR		(*((volatile unsigned long *) 0xE0010020)) /* auto-baud control reg. */
#define U1FDR		(*((volatile unsigned char *) 0xE0010028)) /* fractional divider reg. */
#define U1TER		(*((volatile unsigned char *) 0xE0010030)) /* tx enable reg. */

/* I2C Interface 0 */
#define I2C0CONSET	(*((volatile unsigned char *) 0xE001C000)) /* control set reg. */
#define I2C0STAT	(*((volatile unsigned char *) 0xE001C004)) /* status reg. */
#define I2C0DAT		(*((volatile unsigned char *) 0xE001C008)) /* data reg. */
#define I2C0ADR		(*((volatile unsigned char *) 0xE001C00C)) /* slave address reg. */
#define I2C0SCLH	(*((volatile unsigned short*) 0xE001C010)) /* duty cycle reg. (MSW) */
#define I2C0SCLL	(*((volatile unsigned short*) 0xE001C014)) /* duty cycle reg. (LSW) */
#define I2C0CONCLR	(*((volatile unsigned char *) 0xE001C018)) /* control clear reg. W/O */

/* I2C Interface 1 */
#define I2C1CONSET	(*((volatile unsigned char *) 0xE005C000)) /* control set reg. */
#define I2C1STAT	(*((volatile unsigned char *) 0xE005C004)) /* status reg. */
#define I2C1DAT		(*((volatile unsigned char *) 0xE005C008)) /* data reg. */
#define I2C1ADR		(*((volatile unsigned char *) 0xE005C00C)) /* slave address reg. */
#define I2C1SCLH	(*((volatile unsigned short*) 0xE005C010)) /* duty cycle reg. (MSW) */
#define I2C1SCLL	(*((volatile unsigned short*) 0xE005C014)) /* duty cycle reg. (LSW) */
#define I2C1CONCLR	(*((volatile unsigned char *) 0xE005C018)) /* control clear reg. W/O */

/* Serial Peripheral Interface (SPI) */
#define S0SPCR		(*((volatile unsigned short*) 0xE0020000)) /* control reg. */
#define S0SPSR		(*((volatile unsigned char *) 0xE0020004)) /* status reg. */
#define S0SPDR		(*((volatile unsigned short*) 0xE0020008)) /* data reg. */
#define S0SPCCR		(*((volatile unsigned char *) 0xE002000C)) /* clock counter reg. */
#define S0SPINT		(*((volatile unsigned char *) 0xE002001C)) /* interrupt flag */

/* Synchronous Serial Port (SSP) */

#define SSPCR0		(*((volatile unsigned short*) 0xE0068000)) /* control reg. 0 */
#define SSPCR1		(*((volatile unsigned char *) 0xE0068004)) /* control reg. 1 */
#define SSPDR		(*((volatile unsigned short*) 0xE0068008)) /* data reg. */
#define SSPSR		(*((volatile unsigned char *) 0xE006800C)) /* status reg. */
#define SSPCPSR		(*((volatile unsigned char *) 0xE0068010)) /* clock prescale reg. */
#define SSPIMSC		(*((volatile unsigned char *) 0xE0068014)) /* interrupt mask set and clear reg. */
#define SSPRIS		(*((volatile unsigned char *) 0xE0068018)) /* raw interrupt status reg. */
#define SSPMIS		(*((volatile unsigned char *) 0xE006801C)) /* masked interrupt status reg. */
#define SSPICR		(*((volatile unsigned char *) 0xE0068020)) /* interrupt clear reg. */

/* Memory Accelerator Module (MAM) */
#define MAMCR		(*((volatile unsigned char *) 0xE01FC000)) /* MAM control reg. */
#define MAMTIM		(*((volatile unsigned char *) 0xE01FC004)) /* MAM timing control */

/* External Interrupts */
#define EXTINT		(*((volatile unsigned char *) 0xE01FC140)) /* external interrupt flag reg. */
#define INTWAKE		(*((volatile unsigned short*) 0xE01FC144)) /* interrupt wake-up reg. */
#define EXTMODE		(*((volatile unsigned char *) 0xE01FC148)) /* external interrupt mode reg. */
#define EXTPOLAR	(*((volatile unsigned char *) 0xE01FC14C)) /* external interrupt polarity reg. */

/* Memory Mapping Control */
#define MEMMAP		(*((volatile unsigned char *) 0xE01FC040)) /* memory mapping control */

/* Phase Locked Loop (PLL) */
#define PLLCON		(*((volatile unsigned char *) 0xE01FC080)) /* PLL control reg. (protected by PLLFEED) */
#define PLLCFG		(*((volatile unsigned char *) 0xE01FC084)) /* PLL configuration reg. (protected by PLLFEED) */
#define PLLSTAT		(*((volatile unsigned short*) 0xE01FC088)) /* PLL status reg. */
#define PLLFEED		(*((volatile unsigned char *) 0xE01FC08C)) /* PLL feed reg */

/* Power Control */
#define PCON		(*((volatile unsigned char *) 0xE01FC0C0)) /* power control reg. */
#define PCONP		(*((volatile unsigned long *) 0xE01FC0C4)) /* power control for peripherals */

/* APB Divider */
#define APBDIV		(*((volatile unsigned char *) 0xE01FC100)) /* APB divider control */

/* Reset */
#define RSIR		(*((volatile unsigned char *) 0xE01FC180)) /* reset source identification reg. */

/* Code Security/Debugging */
#define CSPR		(*((volatile unsigned char *) 0xE01FC184)) /* code security protection reg. (R/O) */

/* System Control and Status flags register */
#define SCS		(*((volatile unsigned long *) 0xE01FC1A0))

/* Timer 0 */
#define T0IR		(*((volatile unsigned char *) 0xE0004000)) /* interrupt reg. */
#define T0TCR		(*((volatile unsigned char *) 0xE0004004)) /* timer control reg. */
#define T0TC		(*((volatile unsigned long *) 0xE0004008)) /* timer counter */
#define T0PR		(*((volatile unsigned long *) 0xE000400C)) /* prescale reg. */
#define T0PC		(*((volatile unsigned long *) 0xE0004010)) /* prescale counter */
#define T0MCR		(*((volatile unsigned short*) 0xE0004014)) /* match control reg. */
#define T0MR0		(*((volatile unsigned long *) 0xE0004018)) /* match reg. 0 */
#define T0MR1		(*((volatile unsigned long *) 0xE000401C)) /* match reg. 1 */
#define T0MR2		(*((volatile unsigned long *) 0xE0004020)) /* match reg. 2 */
#define T0MR3		(*((volatile unsigned long *) 0xE0004024)) /* match reg. 3 */
#define T0CCR		(*((volatile unsigned short*) 0xE0004028)) /* capture control reg. */
#define T0CR0		(*((volatile unsigned long *) 0xE000402C)) /* capture reg. 0 */
#define T0CR1		(*((volatile unsigned long *) 0xE0004030)) /* capture reg. 1 */
#define T0CR2		(*((volatile unsigned long *) 0xE0004034)) /* capture reg. 2 */
#define T0EMR		(*((volatile unsigned short*) 0xE000403C)) /* external match reg. */
#define T0CTCR		(*((volatile unsigned char *) 0xE0004070)) /* count control reg. */
#define T0PWMCON	(*((volatile unsigned long *) 0xE0004074)) /* PWM control reg. */

/* Timer 1 */
#define T1IR		(*((volatile unsigned char *) 0xE0008000)) /* interrupt reg. */
#define T1TCR		(*((volatile unsigned char *) 0xE0008004)) /* timer control reg. */
#define T1TC		(*((volatile unsigned long *) 0xE0008008)) /* timer counter */
#define T1PR		(*((volatile unsigned long *) 0xE000800C)) /* prescale reg. */
#define T1PC		(*((volatile unsigned long *) 0xE0008010)) /* prescale counter */
#define T1MCR		(*((volatile unsigned short*) 0xE0008014)) /* match control reg. */
#define T1MR0		(*((volatile unsigned long *) 0xE0008018)) /* match reg. 0 */
#define T1MR1		(*((volatile unsigned long *) 0xE000801C)) /* match reg. 1 */
#define T1MR2		(*((volatile unsigned long *) 0xE0008020)) /* match reg. 2 */
#define T1MR3		(*((volatile unsigned long *) 0xE0008024)) /* match reg. 3 */
#define T1CCR		(*((volatile unsigned short*) 0xE0008028)) /* capture control reg. */
#define T1CR0		(*((volatile unsigned long *) 0xE000802C)) /* capture reg. 0 */
#define T1CR1		(*((volatile unsigned long *) 0xE0008030)) /* capture reg. 1 */
#define T1CR2		(*((volatile unsigned long *) 0xE0008034)) /* capture reg. 2 */
#define T1EMR		(*((volatile unsigned short*) 0xE000803C)) /* external match reg. */
#define T1CTCR		(*((volatile unsigned char *) 0xE0008070)) /* count control reg. */
#define T1PWMCON	(*((volatile unsigned long *) 0xE0008074)) /* PWM control reg. */

/* Timer 2 */
#define T2IR		(*((volatile unsigned char *) 0xE0070000)) /* interrupt reg. */
#define T2TCR		(*((volatile unsigned char *) 0xE0070004)) /* timer control reg. */
#define T2TC		(*((volatile unsigned short*) 0xE0070008)) /* timer counter */
#define T2PR		(*((volatile unsigned short*) 0xE007000C)) /* prescale reg. */
#define T2PC		(*((volatile unsigned short*) 0xE0070010)) /* prescale counter */
#define T2MCR		(*((volatile unsigned short*) 0xE0070014)) /* match control reg. */
#define T2MR0		(*((volatile unsigned short*) 0xE0070018)) /* match reg. 0 */
#define T2MR1		(*((volatile unsigned short*) 0xE007001C)) /* match reg. 1 */
#define T2MR2		(*((volatile unsigned short*) 0xE0070020)) /* match reg. 2 */
#define T2MR3		(*((volatile unsigned short*) 0xE0070024)) /* match reg. 3 */
#define T2CCR		(*((volatile unsigned short*) 0xE0070028)) /* capture control reg. */
#define T2CR0		(*((volatile unsigned short*) 0xE007002C)) /* capture reg. 0 */
#define T2CR1		(*((volatile unsigned short*) 0xE0070030)) /* capture reg. 1 */
#define T2CR2		(*((volatile unsigned short*) 0xE0070034)) /* capture reg. 2 */
#define T2EMR		(*((volatile unsigned short*) 0xE007003C)) /* external match reg. */
#define T2CTCR		(*((volatile unsigned char *) 0xE0070070)) /* count control reg. */
#define T2PWMCON	(*((volatile unsigned long *) 0xE0070074)) /* PWM control reg. */

/* Timer 3 */
#define T3IR		(*((volatile unsigned char *) 0xE0074000)) /* interrupt reg. */
#define T3TCR		(*((volatile unsigned char *) 0xE0074004)) /* timer control reg. */
#define T3TC		(*((volatile unsigned short*) 0xE0074008)) /* timer counter */
#define T3PR		(*((volatile unsigned short*) 0xE007400C)) /* prescale reg. */
#define T3PC		(*((volatile unsigned short*) 0xE0074010)) /* prescale counter */
#define T3MCR		(*((volatile unsigned short*) 0xE0074014)) /* match control reg. */
#define T3MR0		(*((volatile unsigned short*) 0xE0074018)) /* match reg. 0 */
#define T3MR1		(*((volatile unsigned short*) 0xE007401C)) /* match reg. 1 */
#define T3MR2		(*((volatile unsigned short*) 0xE0074020)) /* match reg. 2 */
#define T3MR3		(*((volatile unsigned short*) 0xE0074024)) /* match reg. 3 */
#define T3CCR		(*((volatile unsigned short*) 0xE0074028)) /* capture control reg. */
#define T3CR0		(*((volatile unsigned short*) 0xE007402C)) /* capture reg. 0 */
#define T3CR1		(*((volatile unsigned short*) 0xE0074030)) /* capture reg. 1 */
#define T3CR2		(*((volatile unsigned short*) 0xE0074034)) /* capture reg. 2 */
#define T3EMR		(*((volatile unsigned short*) 0xE007403C)) /* external match reg. */
#define T3CTCR		(*((volatile unsigned char *) 0xE0074070)) /* count control reg. */
#define T3PWMCON	(*((volatile unsigned long *) 0xE0074074)) /* PWM control reg. */

/* Real-Time Clock (RTC) Misc. */
#define ILR		(*((volatile unsigned char *) 0xE0024000)) /* interrupt location */
#define CTC		(*((volatile unsigned short*) 0xE0024004)) /* clock tick counter */
#define CCR		(*((volatile unsigned char *) 0xE0024008)) /* clock control reg. */
#define CIIR		(*((volatile unsigned char *) 0xE002400C)) /* counter increment interrupt */
#define AMR		(*((volatile unsigned char *) 0xE0024010)) /* alarm mask reg. */
#define CTIME0		(*((volatile unsigned long *) 0xE0024014)) /* consolidated time reg. 0 */
#define CTIME1		(*((volatile unsigned long *) 0xE0024018))
#define CTIME2		(*((volatile unsigned long *) 0xE002401C))

/* RTC Time Counter */
#define SEC		(*((volatile unsigned char *) 0xE0024020))
#define MIN		(*((volatile unsigned char *) 0xE0024024))
#define HOUR		(*((volatile unsigned char *) 0xE0024028))
#define DOM		(*((volatile unsigned char *) 0xE002402C))
#define DOW		(*((volatile unsigned char *) 0xE0024030))
#define DOY		(*((volatile unsigned short*) 0xE0024034))
#define MONTH		(*((volatile unsigned char *) 0xE0024038))
#define YEAR		(*((volatile unsigned short*) 0xE002403C))

/* RTC Alarm */
#define ALSEC		(*((volatile unsigned char *) 0xE0024060))
#define ALMIN		(*((volatile unsigned char *) 0xE0024064))
#define ALHOUR		(*((volatile unsigned char *) 0xE0024068))
#define ALDOM		(*((volatile unsigned char *) 0xE002406C))
#define ALDOW		(*((volatile unsigned char *) 0xE0024070))
#define ALDOY		(*((volatile unsigned short*) 0xE0024074))
#define ALMON		(*((volatile unsigned char *) 0xE0024078))
#define ALYEAR		(*((volatile unsigned short*) 0xE002407C))

/* RTC Reference Clock Divider */
#define PREINT		(*((volatile unsigned short*) 0xE0024080))
#define PREFRAC		(*((volatile unsigned short*) 0xE0024084))

/* Watchdog Timer (WDT) */
#define WDMOD		(*((volatile unsigned char *) 0xE0000000)) /* mode reg. */
#define WDTC		(*((volatile unsigned long *) 0xE0000004)) /* time-out interval */
#define WDFEED		(*((volatile unsigned char *) 0xE0000008))
#define WDTV		(*((volatile unsigned long *) 0xE000000C)) /* timer value */

/* Analog to Digital Converter (ADC) */
#define ADCR		(*((volatile unsigned long *) 0xE0034000)) /* A/D Control Reg. */
#define ADGDR		(*((volatile unsigned long *) 0xE0034004)) 
#define ADSTAT		(*((volatile unsigned long *) 0xE0034030)) 
#define ADINTEN		(*((volatile unsigned long *) 0xE003400C)) 
#define ADDR0		(*((volatile unsigned long *) 0xE0034010)) 
#define ADDR1		(*((volatile unsigned long *) 0xE0034014)) 
#define ADDR2		(*((volatile unsigned long *) 0xE0034018)) 
#define ADDR3		(*((volatile unsigned long *) 0xE003401C)) 
#define ADDR4		(*((volatile unsigned long *) 0xE0034020)) 
#define ADDR5		(*((volatile unsigned long *) 0xE0034024))
#define ADDR6		(*((volatile unsigned long *) 0xE0034028))
#define ADDR7		(*((volatile unsigned long *) 0xE003402C))

///////////////////////////////////////////////////////////////////////////////
// System Contol Block
#define SCB             ((scbRegs_t *)0xE01FC000)

// VPB Divider Register
#define VPBDIV          SCB->vpbdiv


#endif // __LPC2103_H
