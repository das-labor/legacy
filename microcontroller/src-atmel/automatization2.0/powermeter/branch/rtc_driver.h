/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA RTC driver header file.
 *
 *      This file contains the function prototypes and enumerator definitions
 *      for various configuration parameters for the XMEGA RTC driver.
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
#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include "avr_compiler.h"

/* Definitions of macros. */

/*! \brief This macro checks the RTC busy flag.
 *
 *  \return Non-Zero if RTC is busy, zero otherwise.
 */
#define RTC_Busy()               ( RTC.STATUS & RTC_SYNCBUSY_bm )

/*! \brief This macro returns the status of the RTC overflow flag.
 *
 *  \return Non-Zero if overflow has occured, zero otherwise.
 */
#define RTC_GetOverflowFlag()    ( RTC.INTFLAGS & RTC_OVFIF_bm )

/*! \brief This macro returns the status of the RTC compare flag.
 *
 *  \return Non-Zero if an RTC compare has occured, zero otherwise.
 */
#define RTC_GetCompareFlag()     ( RTC.INTFLAGS & RTC_COMPIF_bm )

/*! \brief This macro sets a new RTC count value.
 *
 *  This is the count value of the RTC, which will count up to the RTC period.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param _rtccnt The new count value.
 */
#define RTC_SetCount( _rtccnt )  ( RTC.CNT = (_rtccnt) )

/*! \brief This macro returns the current RTC count value.
 *
 *  This is the count value of the RTC, which will count up to the RTC period.
 *
 *  \return The current RTC count value.
 */
#define RTC_GetCount()           ( RTC.CNT )

/*! \brief This macro sets a new RTC period.
 *
 *  This is the period value of the RTC, which is the top of the count sequence.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param _rtcper The new RTC period.
 */
#define RTC_SetPeriod( _rtcper ) ( RTC.PER = (_rtcper) )

/*! \brief This macro returns the current RTC period value.
 *
 *  This is the period value of the RTC, which is the top of the count sequence.
 *
 *  \return The current RTC period value.
 */
#define RTC_GetPeriod()          ( RTC.PER )

/*! \brief This macro sets a new RTC compare value.
 *
 *  This is the compare value of the RTC, which will be compared to the count
 *  sequence. The compare value must be less than the RTC period.
 *
 *  \note The program code needs to check if the RTC is busy before calling
 *        this function. The RTC_Busy() function can be used to do this.
 *
 *  \param _cmpVal   The new compare value.
 */
#define RTC_SetCompareValue( _cmpVal ) ( RTC.COMP = (_cmpVal) )

/*! \brief This macro returns the current RTC compare value.
 *
 *  This is the compare value of the RTC, which will be compared to the count
 *  sequence. The compare value must be less than the RTC period.
 *
 *  \return  The current compare value.
 */
#define RTC_GetCompareValue()    ( RTC.COMP )

#define RTC_CYCLES_1S     1024
/* Prototyping of functions. Documentation is found in source file. */

void RTC_Initialize( uint16_t period,
                     uint16_t count,
                     uint16_t compareValue,
                     RTC_PRESCALER_t prescaler);
void RTC_SetOverflowIntLevel( RTC_OVFINTLVL_t intLevel );
void RTC_SetCompareIntLevel( RTC_COMPINTLVL_t intLevel );
void RTC_SetIntLevels( RTC_OVFINTLVL_t ovfIntLevel,
                       RTC_COMPINTLVL_t compIntLevel );
void RTC_SetAlarm( uint16_t alarmTimeout );
void RTC_SetPrescaler( RTC_PRESCALER_t prescaler );

void RTC_Init(void);

uint8_t RTC_getSeconds(void);
uint8_t RTC_getMinutes(void);
uint8_t RTC_getHours(void);
uint8_t RTC_getDays(void);
uint8_t RTC_getYears(void);
void RTC_seconds_int(void);
void RTC_minutes_int(void);
void RTC_hours_int(void);
void RTC_days_int(void);
void RTC_year_int(void);
/*! This is the interrupt vector declaration. Copy it to your
 *  program code if you want to use it. Leave commented if it's not
 *  used to avoid overhead when compiling.
 *
   ISR(RTC_COMP_vect)
   {
   }

   ISR(RTC_OVF_vect)
   {
   }
 *
 */

#endif
