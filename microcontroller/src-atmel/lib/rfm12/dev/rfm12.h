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

/******************************************************
 *                                                    *
 *    NO  C O N F I G U R A T I O N  IN THIS FILE     *
 *                                                    *
 *      ( thou shalt not change lines below )         *
 *                                                    *
 ******************************************************/
 
#ifndef _RFM12_H
#define _RFM12_H

//states for the rx and tx buffers
#define STATUS_FREE 0
#define STATUS_OCCUPIED 1
#define STATUS_COMPLETE 2

// possible return values for rfm12_tx() and rfm12_start_tx()
#define RFM12_TX_SUCCESS 0x00
#define RFM12_TX_OVERFLOW 0x01
#define RFM12_TX_ERROR 0x02
#define RFM12_TX_OCCUPIED 0x03
#define RFM12_TX_ENQUEUED 0x80

/************************
 * function protoypes
*/

void rfm12_init();
void rfm12_data(uint16_t d);
void rfm12_tick();

//if receive mode is not disabled (default)
#if !(RFM12_TRANSMIT_ONLY)
	void rfm12_rx_clear();
#endif /* !(RFM12_TRANSMIT_ONLY) */

//FIXME: the tx function should return a status, do we need to do this also?
// uint8_t rfm12_tx_status();

#if (RFM12_NORETURNS)
void rfm12_start_tx(uint8_t type, uint8_t length);
void rfm12_tx(uint8_t len, uint8_t type, uint8_t *data);
#else
uint8_t rfm12_start_tx(uint8_t type, uint8_t length);
uint8_t rfm12_tx(uint8_t len, uint8_t type, uint8_t *data);
#endif


/************************
 * private control structs
 */

//the transmission buffer
typedef struct{
	//we transmit the bytes beginning here
	uint8_t sync[2]; 		//sync bytes for receiver to start filling fifo
	uint8_t len;			//length byte - number of bytes in buffer
	uint8_t type;			//type field for airlab
	uint8_t checksum;		//checksum over the former 2
	uint8_t buffer[RFM12_TX_BUFFER_SIZE];//buffer for the raw bytes to be transmitted
} rf_tx_buffer_t;


//if receive mode is not disabled (default)
#if !(RFM12_TRANSMIT_ONLY)
	//a receive buffer
	typedef struct{
		volatile uint8_t status;//is the buffer free or is there complete data in it?
		uint8_t len;			//length byte - number of bytes in buffer
		uint8_t type;			//type field for airlab
		uint8_t checksum;		//rx checksum
		uint8_t buffer[RFM12_RX_BUFFER_SIZE]; //receive buffer
	} rf_rx_buffer_t;
#endif /* !(RFM12_TRANSMIT_ONLY) */	
 

//NEW AND SHINY CONTROL STRUCT
typedef struct
{
	//this is the internal state machine
	volatile uint8_t rfm12_state;
	
	//tx buffer status
	//(free or occupied -> transmission enqueued)
	volatile uint8_t txstate;
	
	//number of bytes to transmit or receive
	//(size in buffer)
	uint8_t num_bytes;
	
	//counter for the byte we are transmitting or receiving at the moment
	uint8_t bytecount;

	//if receive mode is not disabled (default)
	#if !(RFM12_TRANSMIT_ONLY)	
		//points to the rf_buffer in rf_buffers that will be filled next
		rf_rx_buffer_t * rf_buffer_in;
		
		//points to the rf_buffer in rf_buffers that will be retruned for reading next
		rf_rx_buffer_t * rf_buffer_out;
		
		//the numbers of the currently used in / out receive buffers
		uint8_t buffer_in_num;
		uint8_t buffer_out_num;
	#endif /* !(RFM12_TRANSMIT_ONLY) */
	
	//wakeup timer feature
	#if RFM12_USE_WAKEUP_TIMER
		//power management shadow register
		uint16_t pwrmgt_shadow;
	#endif /* RFM12_USE_WAKEUP_TIMER */
	
	#if RFM12_LOW_BATT_DETECTOR
		uint8_t low_batt;
	#endif /* RFM12_LOW_BATT_DETECTOR */
} rfm12_control_t;


/************************
 * GLOBALS
 */

//Buffer and status for the message to be transmitted
extern rf_tx_buffer_t rf_tx_buffer;

//if receive mode is not disabled (default)
#if !(RFM12_TRANSMIT_ONLY)
	//buffers for storing incoming transmissions
	extern rf_rx_buffer_t rf_rx_buffers[2];
#endif /* !(RFM12_TRANSMIT_ONLY) */

//the control struct
extern rfm12_control_t ctrl;


/************************
 * INLINE FUNCTIONS
 */

//if receive mode is not disabled (default)
#if !(RFM12_TRANSMIT_ONLY)
	//inline function to return the rx buffer status byte
	//(returns STATUS_FREE or STATUS_COMPLETE)
	static inline uint8_t rfm12_rx_status()
	{
		return ctrl.rf_buffer_out->status;
	}

	//inline function to return the rx buffer length field
	static inline uint8_t rfm12_rx_len()
	{
		return ctrl.rf_buffer_out->len;
	}

	//inline function to return the rx buffer type field
	static inline uint8_t rfm12_rx_type()
	{
		return ctrl.rf_buffer_out->type;
	}

	//inline function to retreive current rf buffer contents
	static inline uint8_t *rfm12_rx_buffer()
	{
		return (uint8_t*) ctrl.rf_buffer_out->buffer;
	}
#endif /* !(RFM12_TRANSMIT_ONLY) */


/************************
 * include headers for all the optional stuff in here
 * this way the user only needs to include rfm12.h
*/
#include "include/rfm12_extra.h"
#include "include/rfm12_ctrl.h"

#endif /* _RFM12_H */