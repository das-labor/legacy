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
 
#ifndef _RFM12_EXTRA_H
#define _RFM12_EXTRA_H

/************************
 * amplitude modulation receive mode
*/

#if RFM12_RECEIVE_CW
	#define RFRXBUF_SIZE 55
	#define STATE_EMPTY 0
	#define STATE_RECEIVING 1
	#define STATE_FULL 2

	typedef struct
	{
		volatile	uint8_t p;
		volatile	uint8_t state;
		uint8_t 	buf[RFRXBUF_SIZE];
	} rfrxbuf_t;

	extern rfrxbuf_t cw_rxbuf;
	 
	void adc_init();
#endif /* RFM12_RECEIVE_CW */


/************************
 * amplitude modulated raw tx mode
*/
 
#if RFM12_RAW_TX
	/*
	 * @description Enable the transmitter immediately.
	 */
	inline void rfm12_tx_on (void)
	{
		/* set enable transmission bit now. */
		rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ET);
	}

	/*
	 * @description Set default settings (usually transmitter off, receiver on)
	 */
	inline void rfm12_tx_off (void)
	{
		/* turn off everything. */
		rfm12_data(RFM12_CMD_PWRMGT);
	}
#endif /* RFM12_RAW_TX  */


/************************
 * rfm12 wakeup timer mode
*/

#if RFM12_USE_WAKEUP_TIMER
	//this function sets the wakeup timer register
	//(see datasheet for values)
	void rfm12_set_wakeup_timer(uint16_t val);
#endif /* RFM12_USE_WAKEUP_TIMER */


/************************
 * rfm12 low battery detector mode
*/

#if RFM12_LOW_BATT_DETECTOR
	#define RFM12_BATT_OKAY 0
	#define RFM12_BATT_LOW 1

	//this function sets the low battery detector and microcontroller clock divider register
	//(see datasheet for values)
	void rfm12_set_batt_detector(uint16_t val);
	
	//return the battery status
	uint8_t rfm12_get_batt_status();
#endif /* RFM12_LOW_BATT_DETECTOR */

#endif /* _RFM12_EXTRA_H */
