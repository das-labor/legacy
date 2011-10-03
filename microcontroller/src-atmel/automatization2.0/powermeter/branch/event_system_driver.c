/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  XMEGA Event system driver source file.
 *
 *      This file contains the function implementations the XMEGA Event System
 *      driver.
 *
 *      The driver is not intended for size and/or speed critical code. The
 *      driver is intended for rapid prototyping and documentation purposes for
 *      getting started with the XMEGA Event system.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 *      Several functions use the following construct:
 *          "some_register = ... | (some_parameter ? SOME_BIT_bm : 0) | ..."
 *      Although the use of the ternary operator ( if ? then : else ) is
 *      discouraged, in some occasions the operator makes it possible to
 *      write pretty clean and neat code. In this driver, the construct is
 *      used to set or not set a configuration bit based on a boolean input
 *      parameter, such as the "some_parameter" in the example above.
 *
 * \par Application note:
 *      AVR1001: Getting Started With the XMEGA Event System
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

#include "event_system_driver.h"


/*! \brief This function sets the event source for an event channel.
 *
 *  \param eventChannel     The event channel number, range 0-7.
 *  \param eventSource      The event source to use as input to the MUX.
 *
 *  \retval true  if a valid channel was selected.
 *  \retval false if a non-valid channel was selected.
 */
bool EVSYS_SetEventSource( uint8_t eventChannel, EVSYS_CHMUX_t eventSource )
{
	volatile uint8_t * chMux;

	/*  Check if channel is valid and set the pointer offset for the selected
	 *  channel and assign the eventSource value.
	 */
	if (eventChannel < 8) {
		chMux = &EVSYS.CH0MUX + eventChannel;
		*chMux = eventSource;

		return true;
	} else {
		return false;
	}
}

/*! \brief This function sets the parameters for an event channel.
 *
 *  \note The quadrature decoder is only available on channel 0, 2 and 4.
 *
 *  \param eventChannel       The event channel number, either 0, 2 or 4.
 *  \param QDIRM              Quadrature decoder index recognition mode.
 *  \param QDIndexEnable      Enable quadrature decoder index.
 *  \param QDEnable           Enable Quadrature decoder.
 *  \param filterCoefficient  Filter coefficient for the digital input filter.
 *
 *  \retval true  if a valid channel was selected.
 *  \retval false if a non-valid channel was selected.
 */
bool EVSYS_SetEventChannelParameters( uint8_t eventChannel,
                                      EVSYS_QDIRM_t QDIRM,
                                      bool QDIndexEnable,
                                      bool QDEnable,
                                      EVSYS_DIGFILT_t filterCoefficient )
{

	/*  Check if channel is valid and set the pointer offset for the selected
	 *  channel and assign the configuration value.
	 */
	if ( ( eventChannel == 0 ) ||
	     ( eventChannel == 2 ) ||
	     ( eventChannel == 4 ) ) {

		volatile uint8_t * chCtrl;
		chCtrl = &EVSYS.CH0CTRL + eventChannel;
		*chCtrl = ( uint8_t ) QDIRM |
		          filterCoefficient |
		          ( QDIndexEnable ? EVSYS_QDIEN_bm : 0 ) |
		          ( QDEnable ? EVSYS_QDEN_bm : 0 );

		return true;
	} else {
		return false;
	}
}

/*! \brief This function sets the filter parameters for an event channel.
 *
 *  \param eventChannel       The event channel number, range 0-7.
 *  \param filterCoefficient  Filter coefficient for the digital input filter.
 *
 *  \retval true  if a valid channel was selected.
 *  \retval false if a non-valid channel was selected.
 */
bool EVSYS_SetEventChannelFilter( uint8_t eventChannel,
                                  EVSYS_DIGFILT_t filterCoefficient )
{
	/*  Check if channel is valid and set the pointer offset for the selected
	 *  channel and assign the configuration value.
	 */
	if (eventChannel < 8) {

		volatile uint8_t * chCtrl;
		chCtrl = &EVSYS.CH0CTRL + eventChannel;
		*chCtrl = filterCoefficient;

		return true;
	} else {
		return false;
	}
}


/*! \brief This function sets the event data and strobe for a manual event trigger.
 *
 *  This function manually triggers events on the selected channels. The
 *  "Manually Generating Events" section in the Xmega manual have a detailed
 *  description of the events generated with the different combinations of the
 *  bit settings.
 *
 *  \param dataMask   Bit mask for data events on the channel n, where the bit
 *                    position n correspond to the channel n.
 *  \param strobeMask Bit mask for strobe on the channel n, where the bit
 *                    position n correspond to the channel n.
 */
void EVSYS_ManualTrigger( uint8_t dataMask, uint8_t strobeMask )
{
	/* The datamask register must be set before the strobe register. */
	EVSYS.DATA = dataMask;
	EVSYS.STROBE = strobeMask;
}
