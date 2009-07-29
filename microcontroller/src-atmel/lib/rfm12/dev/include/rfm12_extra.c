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
 *    THIS FILE IS BEING INCLUDED DIRECTLY		*
 *		(for performance reasons)				*
 ******************************************************/

 
/************************
 * amplitude modulation receive mode
*/

#if RFM12_RECEIVE_CW
	rfrxbuf_t cw_rxbuf;

	ISR(ADC_vect, ISR_NOBLOCK)
	{
		static uint16_t adc_average;
		static uint8_t pulse_timer;
		
			   uint8_t    value;
		static uint8_t oldvalue;
		static uint8_t ignore;
		uint16_t adc;
		

		
		ADCSRA = (1<<ADEN) | (1<<ADFR) | (0<<ADIE) //start free running mode
				| (1<<ADPS2) | (1<<ADPS1);  //preescaler to clk/64
											//samplerate = 16MHz/(64*13) = 19231 Hz

		
		adc = ADC;
		
		adc_average -= adc_average/64;
		adc_average +=adc;
		
		value = (ADC > ((adc_average/64)+50) )?1:0;
		
		if(value)
		{
			PORTD |= (1<<PD7);
		}else
		{
			PORTD &= ~(1<<PD7);
		}


		if(TCNT0 > 0xE0){
			ignore = 0;
		}
		
		if(cw_rxbuf.state == STATE_EMPTY)
		{
			if(value && (!ignore) )
			{
				//pulse_timer = 0;
				TCNT0 = 0;
				cw_rxbuf.p   = 0;
				cw_rxbuf.state = STATE_RECEIVING;
			}
		}else if(cw_rxbuf.state == STATE_FULL)
		{
			if(value)
			{
				TCNT0 = 0;
				ignore = 1;
			}
			
		}else if(cw_rxbuf.state == STATE_RECEIVING)
		{
			if(value != oldvalue)
			{

				cw_rxbuf.buf[cw_rxbuf.p] = TCNT0;
				TCNT0 = 0;
				//pulse_timer = 0;
				if(cw_rxbuf.p != (RFRXBUF_SIZE-1) )
				{
					cw_rxbuf.p++;
				}
			}else if(TCNT0 > 0xe0)
			{
				//if( !value ){
				//PORTD |= (1<<PD6);
					cw_rxbuf.state = STATE_FULL;
				//}else{
				//	cw_rxbuf.state = STATE_EMPTY;
				//}
			}
		}

		oldvalue = value;

		ADCSRA = (1<<ADEN) | (1<<ADFR) | (1<<ADIE) //start free running mode
				| (1<<ADPS2) | (1<<ADPS1);  //preescaler to clk/64
											//samplerate = 16MHz/(64*13) = 19231 Hz


	}

	void adc_init()
	{
		ADMUX  = (1<<REFS0) | (1<<REFS1); //Internal 2.56V Reference, MUX0
		
		ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADFR) | (1<<ADIE) //start free running mode
				| (1<<ADPS2) | (1<<ADPS1);  //preescaler to clk/64
											//samplerate = 16MHz/(64*13) = 19231 Hz
		
	}
#endif /* RFM12_RECEIVE_CW */


/************************
 * amplitude modulated raw tx mode
*/

#if RFM12_RAW_TX
	/*
	 * @description en- or disable raw transmissions.
	 */
	void rfm12_rawmode (uint8_t in_setting)
	{
		rfm12_raw_tx = in_setting;

		if (in_setting)
		{
			rfm12_mode = MODE_RAW;
			RFM12_INT_OFF();
		} else
		{
			/* re-enable the receiver */
			rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
			RFM12_INT_ON();
			rfm12_mode = MODE_RX;
		}
	}
#endif /* RFM12_RAW_TX */


/************************
 * rfm12 wakeup timer mode
*/

#if RFM12_USE_WAKEUP_TIMER
	//this function sets the wakeup timer register
	//(see datasheet for values)
	void rfm12_set_wakeup_timer(uint16_t val)
	{	
		//set wakeup timer
		rfm12_data (RFM12_CMD_WAKEUP | (val & 0x1FFF));
	
		//reset wakeup timer
		rfm12_data(RFM12_CMD_PWRMGT | (PWRMGT_DEFAULT & ~RFM12_PWRMGT_EW));
		rfm12_data(RFM12_CMD_PWRMGT |  PWRMGT_DEFAULT);		
	}
#endif /* RFM12_USE_WAKEUP_TIMER */


/************************
 * rfm12 low battery detector mode
*/

#if RFM12_LOW_BATT_DETECTOR
	//this function sets the low battery detector and microcontroller clock divider register
	//(see datasheet for values)
	void rfm12_set_batt_detector(uint16_t val)
	{	
		//set the low battery detector and microcontroller clock divider register
		rfm12_data (RFM12_CMD_LBDMCD | (val & 0x01FF));
	}
	
	uint8_t rfm12_get_batt_status()
	{
		return ctrl.low_batt;
	}
#endif /* RFM12_LOW_BATT_DETECTOR */
