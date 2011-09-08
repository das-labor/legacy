/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      XMEGA RTC driver source file.
 *
 *      This file contains the function implementations the XMEGA RTC driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the XMEGA RTC module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 * \par Application note:
 *      AVR1314: Using the XMEGA Real Time Counter
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com
 *
 * $Revision: 1569 $
 * $Date: 2008-04-22 13:03:43 +0200 (ti, 22 apr 2008) $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#include "config.h"
#include "rtc_driver.h"
#include "led_driver.h"

/*! \brief This function initializes the RTC with period, initial count,
 *         compare value and clock prescaler.
 *
 *  All the synchronized registers are written at the same time to save time.
 *  The program code need to check if the RTC is busy before calling this
 *  function. The RTC_Busy() function can be used to do this.
 *
 *  \param period         RTC period. Topvalue = Period - 1.
 *  \param count          Initial RTC count.
 *  \param compareValue   Compare value.
 *  \param prescaler      Clock prescaler setting.
 */
void RTC_Initialize( uint16_t period,
                     uint16_t count,
                     uint16_t compareValue,
                     RTC_PRESCALER_t prescaler )
{
	RTC.PER = period - 1;
	RTC.CNT = count;
	RTC.COMP = compareValue;
	RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | prescaler;
}


/*! \brief This function sets the RTC overflow interrupt level.
 *
 *  \param intLevel The overflow interrupt level.
 */
void RTC_SetOverflowIntLevel( RTC_OVFINTLVL_t intLevel )
{
	RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_OVFINTLVL_gm ) | intLevel;
}


/*! \brief This function sets the RTC compare interrupt level.
 *
 *  \param intLevel The compare interrupt level.
 */
void RTC_SetCompareIntLevel( RTC_COMPINTLVL_t intLevel )
{
	RTC.INTCTRL = ( RTC.INTCTRL & ~RTC_COMPINTLVL_gm ) | intLevel;
}


/*! \brief This function sets both compare and overflow interrupt levels in
 *         one go.
 *
 *  \param ovfIntLevel  The overflow interrupt level.
 *  \param compIntLevel The compare interrupt level.
 */
void RTC_SetIntLevels( RTC_OVFINTLVL_t ovfIntLevel,
                       RTC_COMPINTLVL_t compIntLevel )
{
	RTC.INTCTRL = ( RTC.INTCTRL &
	              ~( RTC_COMPINTLVL_gm | RTC_OVFINTLVL_gm ) ) |
	              ovfIntLevel |
	              compIntLevel;
}


/*! \brief This function sets a timeout alarm.
 *
 *  This function sets a timeout alarm by adding the timeout to the current
 *  count value. If the resulting alarm value is larger than the RTC period
 *  value, it will wrap around. An RTC compare interrupt will be triggered¨
 *  after the specified timeout.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \note The timeout must be less than the timer period. The timeout should not
 *        be set too low, as the timeout may be missed, depending on CPU speed.
 *
 *  \param alarmTimeout Timeout time in RTC clock cycles (scaled).
 */
void RTC_SetAlarm( uint16_t alarmTimeout )
{
	/* Calculate compare time. */
	uint16_t compareValue = RTC.CNT + alarmTimeout;

	/* Wrap on period. */
	if (compareValue > RTC.PER){
		compareValue -= RTC.PER;
	}

	/* Add the timeout value to get the absolute time of the alarm. */
	RTC.COMP = compareValue;
}


/*! \brief This function sets a new RTC clock prescaler value.
 *
 *  \note The program code need to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param prescaler Clock prescaler setting.
 */
void RTC_SetPrescaler( RTC_PRESCALER_t prescaler )
{
	RTC.CTRL = ( RTC.CTRL & ~RTC_PRESCALER_gm ) | prescaler;
}

/* User defined RTC Init */
void RTC_Init(void)
{
	/* Security Signature to modify clock */
	//CCP = CCP_IOREG_gc;
	
	/* Turn on internal 32kHz. */
	//OSC.CTRL |= OSC_RC32KEN_bm;

	do {
		/* Wait for the 32kHz oscillator to stabilize. */
	} while ( ( OSC.STATUS & OSC_RC32KRDY_bm ) == 0);


	/* Set internal 32kHz oscillator as clock source for RTC. */
	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;

	do {
		/* Wait until RTC is not busy. */
	} while ( RTC_Busy() );


	
	/* Configure RTC period to 1 second. */
	RTC_Initialize( RTC_CYCLES_1S, 0, 0, RTC_PRESCALER_DIV1_gc );

	/* Enable overflow interrupt. */
	//RTC_SetIntLevels( RTC_OVFINTLVL_LO_gc, RTC_COMPINTLVL_OFF_gc );



}

typedef struct
{
	uint8_t second;		//0-59
	uint8_t minute;		//0-59
	uint8_t hour;		//0-23
	uint8_t day;		//0-31
	uint8_t month;		//0-12
	uint16_t year;		//2011-??
	
} time_t;

static time_t time;
ISR(RTC_OVF_vect)
{

	time.second++;
	if(time.second == 60){
		time.second=0;
		time.minute++;
		if(time.minute == 60){
			time.minute=0;
			time.hour++;
			if(time.hour == 24){
				time.hour=0;
				time.day++;
				if(time.day == 365){
					time.day=0;
					time.year++;
					RTC_year_int();	
				}
				RTC_days_int();	
			}
			RTC_hours_int();	
		}
		RTC_minutes_int();	
	}
	RTC_seconds_int();
	
}

uint8_t RTC_getSeconds(void)
{
	return time.second;
}

uint8_t RTC_getMinutes(void)
{
	return time.minute;
}

uint8_t RTC_getHours(void)
{
	return time.hour;
}

uint8_t RTC_getDays(void)
{
	return time.day;
}

uint8_t RTC_getYears(void)
{
	return time.year;
}

void RTC_seconds_int(void)
{
	

}

void RTC_minutes_int(void)
{

}

void RTC_hours_int(void)
{

}

void RTC_days_int(void)
{

}

void RTC_year_int(void)
{

}
