/**** RFM 12 library for Atmel AVR Microcontrollers *******
 *
 * This software is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * @author Peter Fuhrmann, Hans-Gert Dahmen, Soeren Heisrath
 */
 
#ifndef _RFM12_CORE_H
#define _RFM12_CORE_H

/************************
* VARIOUS RFM RELATED DEFINES FOR INTERNAL USE	
*(defines which shall be visible to the user are located in rfm12.h)
*/

//default preamble (altrernating 1s and 0s)
#define PREAMBLE 0xAA

//default synchronization pattern
#define SYNC_MSB 0x2D
#define SYNC_LSB 0xD4

//these are the states for the receive/transmit state machine
#define STATE_RX_IDLE 0
#define STATE_RX_ACTIVE 1
#define STATE_TX 2
//FIXME: raw mode...
#define MODE_RAW 2

//packet header length in bytes
#define PACKET_OVERHEAD 3


/************************
* LIBRARY DEFAULT SETTINGS	
 */
 
//if notreturns is not defined, we won't use this feature
#ifndef RFM12_NORETURNS
	#define RFM12_NORETURNS 0
#endif

//if transmit only is not defined, we won't use this feature
#ifndef RFM12_TRANSMIT_ONLY
	#define RFM12_TRANSMIT_ONLY 0
#endif

//if transmit only is on, we need to turn of collision detection
#if RFM12_TRANSMIT_ONLY
	//disable collision detection, as we won't be able to receive data
	#ifdef RFM12_NOCOLLISIONDETECTION
		#undef RFM12_NOCOLLISIONDETECTION
	#endif
	#define RFM12_NOCOLLISIONDETECTION 1
#endif

//if nocollisiondetection is not defined, we won't use this feature
#ifndef RFM12_NOCOLLISIONDETECTION
	#define RFM12_NOCOLLISIONDETECTION 0
#endif

//if livectrl is not defined, we won't use this feature
#ifndef RFM12_LIVECTRL
	#define RFM12_LIVECTRL 0
#endif

//if low battery detector is not defined, we won't use this feature
#ifndef RFM12_LOW_BATT_DETECTOR
	#define RFM12_LOW_BATT_DETECTOR 0
	#define PWRMGMT_LOW_BATT 0
#endif

//if the low battery detector feature is used, we will set some extra pwrmgmt options
#if RFM12_LOW_BATT_DETECTOR
	//define PWRMGMT_LOW_BATT  with low batt detector
	//it will be used later
	#define PWRMGMT_LOW_BATT (RFM12_PWRMGT_EB)
	
	//check if the default power management setting has the LB bit set
	//and warn the user if it's not
	#ifdef PWRMGT_DEFAULT
		#if !((PWRMGT_DEFAULT) & RFM12_PWRMGT_EB)
			#warning "You are using the RFM12 low battery detector, but PWRMGT_DEFAULT has the low battery detector bit unset."
		#endif
	#endif
#endif /* RFM12_LOW_BATT_DETECTOR */

//if wakeuptimer is not defined, we won't use this feature
#ifndef RFM12_USE_WAKEUP_TIMER
	#define RFM12_USE_WAKEUP_TIMER 0
	#define PWRMGMT_WKUP 0
#endif

//if wakeuptimer is on, we will set the default power management to use the wakeup timer
#if RFM12_USE_WAKEUP_TIMER
	//define PWRMGMT_LOW_BATT  with low batt detector
	//it will be used later
	#define PWRMGMT_WKUP (RFM12_PWRMGT_EW)
	
	//check if the default power management setting has the EW bit set
	//and warn the user if it's not
	#ifdef PWRMGT_DEFAULT
		#if !((PWRMGT_DEFAULT) & RFM12_PWRMGT_EW)
			#warning "You are using the RFM12 wakeup timer, but PWRMGT_DEFAULT has the wakeup timer bit unset."
		#endif
	#endif
#endif

//if raw tx is not defined, we won't use this feature
#ifndef RFM12_RAW_TX
	#define RFM12_RAW_TX 0
#endif

//if receive cw is not defined, we won't use this feature
#ifndef RFM12_RECEIVE_CW
	#define RFM12_RECEIVE_CW 0
#endif

//if software spi is not defined, we won't use this feature
#ifndef RFM12_SPI_SOFTWARE
	#define RFM12_SPI_SOFTWARE 0
#endif

//if uart debug is not defined, we won't use this feature
#ifndef RFM12_UART_DEBUG
	#define RFM12_UART_DEBUG 0
#endif
 
//default value for powermanagement register
#ifndef PWRMGT_DEFAULT
	#define PWRMGT_DEFAULT (RFM12_PWRMGT_DC | PWRMGMT_WKUP | PWRMGMT_LOW_BATT)
#endif

//default channel free time, if not defined elsewhere
#ifndef CHANNEL_FREE_TIME
	#define CHANNEL_FREE_TIME 200
#endif

/*
 * backward compatibility for the spi stuff
 * these values weren't set in older revisions of this library
 * so they're now assumed to be on the same pin/port.
 * otherwise these defines serve to configure the software SPI ports
 */
#ifndef DDR_MOSI
	#define DDR_MOSI DDR_SPI
	#define PORT_MOSI PORT_SPI
#endif

#ifndef DDR_MISO
	#define DDR_MISO DDR_SPI
	#define PIN_MISO PIN_SPI
#endif

#ifndef DDR_SCK
	#define DDR_SCK DDR_SPI
	#define PORT_SCK PORT_SPI
#endif

#ifndef DDR_SPI_SS
	#define DDR_SPI_SS DDR_SPI
	#define PORT_SPI_SS PORT_SPI
#endif


/************************
* HELPER MACROS
*/

//macros to turn the int on and off
#define RFM12_INT_ON() RFM12_INT_MSK |= (1<<RFM12_INT_BIT)
#define RFM12_INT_OFF() RFM12_INT_MSK &= ~(1<<RFM12_INT_BIT)

/*
 * the following macros help to manage the rfm12 fifo
 * default fiforeset is as follows:
 * 2 Byte Sync Pattern, disable sensitive reset, fifo filled interrupt at 8 bits
 */
//default fiforeset register value to accept data
#define ACCEPT_DATA (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
#define ACCEPT_DATA_INLINE (RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
//default fiforeset register value to clear fifo
#define CLEAR_FIFO (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4))
#define CLEAR_FIFO_INLINE (RFM12_FIFORESET_DR | (8<<4))

//this macro helps to encapsulate the return values, when noreturn is set to on
#if (RFM12_NORETURNS)
	#define TXRETURN(a)
#else
	#define TXRETURN(a) (a)
#endif

#endif /* _RFM12_CORE_H */