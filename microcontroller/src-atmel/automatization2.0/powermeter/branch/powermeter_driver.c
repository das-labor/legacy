/*
*
*    Powermeter driver for XMega
*    Copyright (C) 2013  Patrick Rudolph <siro@das-labor.org>
*
*    This program is free software; you can redistribute it and/or modify it under the terms 
*    of the GNU General Public License as published by the Free Software Foundation; either version 3 
*    of the License, or (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
*    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*    See the GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License along with this program; 
*    if not, see <http://www.gnu.org/licenses/>.
*
*    This program uses the Xmega ADC and DMA and Timer to measure the power, voltage and current on a 3phase powerline 
*/

#include <stdio.h>

#include "tc_driver.h"
#include "avr_compiler.h"
#include "ursartC1_driver.h"
#include "dma_driver.h"
#include "adc_driver.h"
#include "led_driver.h"
#include "rtc_driver.h"
#include "powermeter_driver.h"
#include "error_handler.h"

#include "config.h"

#include "math.h"

#include "can/can.h"
#include "can/lap.h"
#include "string.h"

#include "can_handler.h"
#include "config.h"

volatile char dma1_done=0;
volatile char dma2_done=0;

powermeter_t powermeter;

ISR(DMA_CH0_vect)
{
    if( DMA.CH0.CTRLB & 0x20 ) //test ERRIF flag 
	{
		DMA.CH0.CTRLB |= 0x20; //clear flag by hand
		setERROR(1);
	}
	
	if( DMA.CH0.CTRLB & 0x10 ) //test TRNIF flag 
	{
		DMA.CH0.CTRLB |= 0x10; //clear flag by hand
		dma1_done++;
	}
	// re-enable the channel
	DMA_EnableChannel(&DMA.CH0);
}

ISR(DMA_CH1_vect)
{
    if( DMA.CH1.CTRLB & 0x20 ) //test ERRIF flag 
	{
		DMA.CH1.CTRLB |= 0x20; //clear flag by hand
		setERROR(1);
	}
	
	if( DMA.CH1.CTRLB & 0x10 ) //test TRNIF flag 
	{
		DMA.CH1.CTRLB |= 0x10; //clear flag by hand
		dma2_done++;
	}
	// re-enable the channel
	DMA_EnableChannel(&DMA.CH1);
}


int powermeter_Start()
{
	memset(&powermeter.powerdraw,0,sizeof(powermeter.powerdraw));
	memset(&powermeter.powerdrawPerSecond,0,sizeof(powermeter.powerdrawPerSecond));

	powermeter.isrunning = 1;
	powermeter.samplesPerSecondDone = 0;
	
	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	ADC_init();

	DMA_Enable();
	DMA_SetPriority(DMA_PRIMODE_RR0123_gc); //dma mode round robin
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0, DMA_CH_TRNINTLVL_MED_gc, DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	
	DMA_SetupBlock( &DMA.CH0,				//channel 0
		(void const *) &ADCA.CH0RES,			//source-addr
		DMA_CH_SRCRELOAD_BLOCK_gc,			//srcDirection reload after each block
		DMA_CH_SRCDIR_INC_gc,				//srcDirection increment after each byte
		(void *) &powermeter.samplebuffer.u[0],	//set destAddr
		DMA_CH_DESTRELOAD_TRANSACTION_gc,		//reload destAddr after transaction
		DMA_CH_DESTDIR_INC_gc,				//destDirection increment destination memory addr
		sizeof(int16_t) * 3,				//blockSize in bytes >= burstlen
		DMA_CH_BURSTLEN_2BYTE_gc,			//burstMode 2byte per burst
		ADCSAMPLESPERPERIOD,				//repeat ADCSAMPLESPERPERIOD times
		true );						//repeat

	DMA_SetTriggerSource(&DMA.CH0, DMA_CH_TRIGSRC_ADCA_CH4_gc);	//Trigger on ADCA_CH0 - 3
	DMA_EnableSingleShot(&DMA.CH0);
	DMA_EnableChannel(&DMA.CH0);

	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1, DMA_CH_TRNINTLVL_MED_gc, DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA_SetupBlock( &DMA.CH1,					//channel 1
			(void const *) &ADCB.CH0RES,			//source-addr
			DMA_CH_SRCRELOAD_BLOCK_gc,			//srcDirection reload after each block
			DMA_CH_SRCDIR_INC_gc,				//srcDirection increment after each byte
			(void *) &powermeter.samplebuffer.i1[0],	//set destAddr
			DMA_CH_DESTRELOAD_TRANSACTION_gc,		//reload destAddr after transaction
			DMA_CH_DESTDIR_INC_gc,				//destDirection increment destination memory addr
			sizeof(int16_t) * 3,				//blockSize in bytes >= burstlen
			DMA_CH_BURSTLEN_2BYTE_gc,			//burstMode 2byte per burst
			ADCSAMPLESPERPERIOD,				//repeat ADCSAMPLESPERPERIOD times
			true );						//repeat

	DMA_SetTriggerSource(&DMA.CH1, DMA_CH_TRIGSRC_ADCB_CH4_gc);	//Trigger on ADCB_CH2
	DMA_EnableSingleShot(&DMA.CH1);
	DMA_EnableChannel(&DMA.CH1);


	LED_issampling();				//set blue LED
	TC1_init( ADCSAMPLESPERPERIOD * NET_FREQ );	//set timer1 to trigger ADCSAMPLESPERPERIOD * NET_FREQ times per second
	return 0;
}

void powermeter_Stop()
{
	TC1_Reset(&TCC1);		//stop trigger for ADC
	DMA_DisableChannel(&DMA.CH0);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH0);	//stop DMA transfers
	DMA_DisableChannel(&DMA.CH1);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH1);	//stop DMA transf
	ADC_Disable(&ADCA);		//disable ADC
	ADC_Disable(&ADCB);		//disable ADC
	LED_isrunning();		//set green LED
	powermeter.isrunning = 0;
}


static uint32_t isqrt32 (uint32_t n)
{  
    register uint32_t root, remainder, place;  
  
    root = 0;  
    remainder = n;  
    place = 0x40000000;
  
    while (place > remainder)  
        place = place >> 2;  
    while (place)  
    {  
        if (remainder >= root + place)  
        {  
            remainder = remainder - root - place;  
            root = root + (place << 1);  
        }  
        root = root >> 1;  
        place = place >> 2;  
    }  
    return root;  
}

void powermeter_docalculations( void )
/*
	powermeter.powerdraw 			power used in one period - updated ADCSAMPLESPERPERIOD times per period
	powermeter.powerdrawPerSecond		power used in one second - updated NET_FREQ times per second
	powermeter.powerdrawLastSecond		power used in last second and energie counter - updated once a second
*/
{
	//check if calculations has to be done
	if (dma1_done && dma2_done)
	{
		register int32_t u;
		register int32_t i;
		register int16_t *up;		//points to start of array containing the sampled voltages: u1, u2, u3, u1, u2, u3, ....
		register int16_t *ip;		//points to start of array containing the sampled currents: i1, i2, i3, i1, i2 ,i3, ....
		
		up = &powermeter.samplebuffer.u[0];
		ip = &powermeter.samplebuffer.i1[0];
		
		LED__cyan();	//set LED color cyan
		dma1_done --;
		dma2_done --;
		
		for(uint8_t x = 0; x < ADCSAMPLESPERPERIOD ; x++)
		{
			//load u & i
			u = (int32_t) (*up) -powermeter.ADCoffset.offsetA;
			i = (int32_t) (*ip) -powermeter.ADCoffset.offsetB;
			up++;//increment pointer
			ip++;//increment pointer

			powermeter.powerdraw.c1.Ueff += u * u;	//calculate Ueff
			powermeter.powerdraw.c1.Ieff += i * i;	//calculate Ieff
			powermeter.powerdraw.c1.P += -u * i;	//calculate P

			//load u & i
			u = (int32_t) (*up) -powermeter.ADCoffset.offsetA;
			i = (int32_t) (*ip) -powermeter.ADCoffset.offsetB;

			up++;//increment pointer
			ip++;//increment pointer

			powermeter.powerdraw.c2.Ueff += u * u;
			powermeter.powerdraw.c2.Ieff += i * i;
			powermeter.powerdraw.c2.P += -u * i;
			//load u & i
			u = (int32_t) (*up) -powermeter.ADCoffset.offsetA;
			i = (int32_t) (*ip) -powermeter.ADCoffset.offsetB;

			up++;//increment pointer
			ip++;//increment pointer

			powermeter.powerdraw.c3.Ueff += u * u;
			powermeter.powerdraw.c3.Ieff += i * i;
			powermeter.powerdraw.c3.P += -u * i;
		}

		// ADCSAMPLESSHIFT is defined in config.h
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += (isqrt32(powermeter.powerdraw.c1.Ueff >> ADCSAMPLESSHIFT) * isqrt32(powermeter.powerdraw.c1.Ieff >> ADCSAMPLESSHIFT));
		powermeter.powerdrawPerSecond.c2.S += (isqrt32(powermeter.powerdraw.c2.Ueff >> ADCSAMPLESSHIFT) * isqrt32(powermeter.powerdraw.c2.Ieff >> ADCSAMPLESSHIFT));
		powermeter.powerdrawPerSecond.c3.S += (isqrt32(powermeter.powerdraw.c3.Ueff >> ADCSAMPLESSHIFT) * isqrt32(powermeter.powerdraw.c3.Ieff >> ADCSAMPLESSHIFT));
		
		//powermeter.powerdrawPerSecond.c1.S += isqrt32(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff)>> (ADCSAMPLESSHIFT*2));
		//powermeter.powerdrawPerSecond.c2.S += isqrt32(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff)>> (ADCSAMPLESSHIFT*2));
		//powermeter.powerdrawPerSecond.c3.S += isqrt32(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff)>> (ADCSAMPLESSHIFT*2));		
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += powermeter.powerdraw.c1.P >> ADCSAMPLESSHIFT;
		powermeter.powerdrawPerSecond.c2.P += powermeter.powerdraw.c2.P >> ADCSAMPLESSHIFT;
		powermeter.powerdrawPerSecond.c3.P += powermeter.powerdraw.c3.P >> ADCSAMPLESSHIFT;
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += isqrt32(powermeter.powerdraw.c1.Ueff >> ADCSAMPLESSHIFT);
		powermeter.powerdrawPerSecond.c2.Ueff += isqrt32(powermeter.powerdraw.c2.Ueff >> ADCSAMPLESSHIFT);
		powermeter.powerdrawPerSecond.c3.Ueff += isqrt32(powermeter.powerdraw.c3.Ueff >> ADCSAMPLESSHIFT);
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += isqrt32(powermeter.powerdraw.c1.Ieff >> ADCSAMPLESSHIFT);
		powermeter.powerdrawPerSecond.c2.Ieff += isqrt32(powermeter.powerdraw.c2.Ieff >> ADCSAMPLESSHIFT);
		powermeter.powerdrawPerSecond.c3.Ieff += isqrt32(powermeter.powerdraw.c3.Ieff >> ADCSAMPLESSHIFT);

		powermeter.samplesPerSecondDone++;

		//clear powermeter.powerdraw
		memset(&powermeter.powerdraw, 0x00, sizeof(powermeter_channel_t));
		
		//return to normal LED color
		LED_on();
	}
	
}

int checkforcanupdate( void )
/* 
	test for NET_FREQ samples done
	returns 1 on powermeter.powerdrawLastSecond updated
	returns 0 nothing to do
*/
{
	if ( powermeter.samplesPerSecondDone >= NET_FREQ )
	{
		/* hardcore values, so the compiler can optimise it */
		powermeter.powerdrawLastSecond.c1.S = powermeter.powerdrawPerSecond.c1.S / NET_FREQ;
		powermeter.powerdrawLastSecond.c2.S = powermeter.powerdrawPerSecond.c2.S / NET_FREQ;
		powermeter.powerdrawLastSecond.c3.S = powermeter.powerdrawPerSecond.c3.S / NET_FREQ;

		powermeter.powerdrawLastSecond.c1.P = powermeter.powerdrawPerSecond.c1.P / NET_FREQ;
		powermeter.powerdrawLastSecond.c2.P = powermeter.powerdrawPerSecond.c2.P / NET_FREQ;
		powermeter.powerdrawLastSecond.c3.P = powermeter.powerdrawPerSecond.c3.P / NET_FREQ;

		powermeter.powerdrawLastSecond.c1.Ueff = powermeter.powerdrawPerSecond.c1.Ueff / NET_FREQ;
		powermeter.powerdrawLastSecond.c2.Ueff = powermeter.powerdrawPerSecond.c2.Ueff / NET_FREQ;
		powermeter.powerdrawLastSecond.c3.Ueff = powermeter.powerdrawPerSecond.c3.Ueff / NET_FREQ;

		powermeter.powerdrawLastSecond.c1.Ieff = powermeter.powerdrawPerSecond.c1.Ieff / NET_FREQ;
		powermeter.powerdrawLastSecond.c2.Ieff = powermeter.powerdrawPerSecond.c2.Ieff / NET_FREQ;
		powermeter.powerdrawLastSecond.c3.Ieff = powermeter.powerdrawPerSecond.c3.Ieff / NET_FREQ;

		powermeter.powerdrawLastSecond.c1.E += powermeter.powerdrawPerSecond.c1.P;
		powermeter.powerdrawLastSecond.c2.E += powermeter.powerdrawPerSecond.c2.P;
		powermeter.powerdrawLastSecond.c3.E += powermeter.powerdrawPerSecond.c3.P;
		
		memset(&powermeter.powerdrawPerSecond, 0x00, sizeof(powermeter_channel_t));
		
		setERROR(0);
		powermeter.samplesPerSecondDone = 0;
		return 1;
	}
	return 0;
}

void TC1_init( volatile uint32_t eventsPerSecond )
/* init Timer 1, tries to find best value for given argument */
{

	if (eventsPerSecond > 0xF42400)		//greater than 16000000Hz ?
		return;
	TC1_Reset(&TCC1);

	uint16_t TC1_TOP;
	if ((F_CPU / (1 * eventsPerSecond)) < 0xFFFF) {
		TC1_TOP = ((F_CPU / 1) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod(&TCC1, TC1_TOP);

		/* Start Timer/Counter. */
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV1_gc);
		return;
	}

	if ((F_CPU / (2 * eventsPerSecond)) < 0xFFFF) {
		TC1_TOP = ((F_CPU / 2) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod(&TCC1, TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV2_gc);
		return;
	}

	if ((F_CPU / (4 * eventsPerSecond)) < 0xFFFF) {
		TC1_TOP = ((F_CPU / 4) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod(&TCC1, TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV4_gc);
		return;
	}

	if ((F_CPU / (8 * eventsPerSecond)) < 0xFFFF) {
		TC1_TOP = ((F_CPU / 8) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod(&TCC1, TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV8_gc);
		return;
	}
	if ((F_CPU / (64 * eventsPerSecond)) < 0xFFFF) {
		TC1_TOP = ((F_CPU / 64) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod(&TCC1, TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV64_gc);
		return;
	}
		if (F_CPU / (256 * eventsPerSecond) < 0xFFFF) {
		TC1_TOP = ((F_CPU / 256) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod(&TCC1, TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource(&TCC1, TC_CLKSEL_DIV256_gc);
		return;
	}
}

void ADC_init( void )
/* 	
	init ADCA and ADCB, used 3 channels per ADC 
	start conversion on Timer1 Event
*/
{
	/* Move stored calibration values to ADC A. */
	ADC_CalibrationValues_Load(&ADCA);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate. */
	//ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);	// 1 MSPs
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV128_gc);	// 250ksps

	/* Set reference voltage on ADC A to external reference pin on PORTA .*/
	ADC_Reference_Config(&ADCA, ADC_REFSEL_AREFA_gc);

	/* Differential Input signed -> 0V ==-2048, Vref/2 ==0, Vref ==+2048 
	no Gain to improve messurement */
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH0, ADC_CH_INPUTMODE_DIFF_gc, ADC_DRIVER_CH_GAIN_NONE);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH1, ADC_CH_INPUTMODE_DIFF_gc, ADC_DRIVER_CH_GAIN_NONE);
	ADC_Ch_InputMode_and_Gain_Config(&ADCA.CH2, ADC_CH_INPUTMODE_DIFF_gc, ADC_DRIVER_CH_GAIN_NONE);

#if ADC_OFFSET_CAL
	/* Get offset value for ADC A. */
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN2_gc, ADC_CH_MUXNEG_PIN2_gc);

	ADC_Enable(&ADCA);
	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_32MHz(&ADCA);
	powermeter.ADCoffset.offsetA = (int8_t)ADC_Offset_Get_Signed(&ADCA, &ADCA.CH0, false);
	ADC_Disable(&ADCA);
#else
	powermeter.ADCoffset.offsetA=0;
#endif
	/*Set Positive and Negative Pins for each Channel */
	ADC_Ch_InputMux_Config(&ADCA.CH0, ADC_CH_MUXPOS_PIN4_gc, ADC_CH_MUXNEG_PIN0_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH1, ADC_CH_MUXPOS_PIN3_gc, ADC_CH_MUXNEG_PIN0_gc);
	ADC_Ch_InputMux_Config(&ADCA.CH2, ADC_CH_MUXPOS_PIN2_gc, ADC_CH_MUXNEG_PIN0_gc);

	/* Configure Event to listen & Action to start on event */
	ADC_Events_Config(&ADCA, ADC_EVSEL_7_gc, ADC_EVACT_SWEEP_gc);

	/* Setup sweep of three virtual channels. */
	ADC_SweepChannels_Config(&ADCA, ADC_SWEEP_012_gc);

	//configure DMASEL, this enables DMA_CH_TRIGSRC_ADCA_CH4_gc
	ADCA.CTRLA |= ADC_DMASEL_CH012_gc;

	/* ADC_Ch_Interrupts_Config(&ADCA.CH0,ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
	   Interrupts aren't used atm
	 */
	
	/* Move stored calibration values to ADC B. */
	ADC_CalibrationValues_Load(&ADCB);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
	ADC_ConvMode_and_Resolution_Config(&ADCB, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate. */
	//ADC_Prescaler_Config(&ADCB, ADC_PRESCALER_DIV32_gc);	// 1 MSPs
	ADC_Prescaler_Config(&ADCB,  ADC_PRESCALER_DIV128_gc);	// 250ksps

	/* Set reference voltage on ADC A to external reference pin on PORTA .*/
	ADC_Reference_Config(&ADCB, ADC_REFSEL_AREFA_gc);

	/* Differential Input signed -> 0V ==-2048, Vref/2 ==0, Vref ==+2048 
	no Gain to improve messurement */
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH0, ADC_CH_INPUTMODE_DIFF_gc, ADC_DRIVER_CH_GAIN_NONE);
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH1, ADC_CH_INPUTMODE_DIFF_gc, ADC_DRIVER_CH_GAIN_NONE);
	ADC_Ch_InputMode_and_Gain_Config(&ADCB.CH2, ADC_CH_INPUTMODE_DIFF_gc, ADC_DRIVER_CH_GAIN_NONE);

#if ADC_OFFSET_CAL
	/* Get offset value for ADC B. */
	ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_MUXPOS_PIN2_gc, ADC_CH_MUXNEG_PIN2_gc);

	ADC_Enable(&ADCB);
	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_32MHz(&ADCB);
	powermeter.ADCoffset.offsetB = (int8_t)ADC_Offset_Get_Signed(&ADCB, &ADCB.CH0, false);
	ADC_Disable(&ADCB);
#else
	powermeter.ADCoffset.offsetB=0;
#endif
	/*Set Positive and Negative Pins for each Channel */
	ADC_Ch_InputMux_Config(&ADCB.CH0, ADC_CH_MUXPOS_PIN5_gc, ADC_CH_MUXNEG_PIN0_gc);
	ADC_Ch_InputMux_Config(&ADCB.CH1, ADC_CH_MUXPOS_PIN3_gc, ADC_CH_MUXNEG_PIN0_gc);
	ADC_Ch_InputMux_Config(&ADCB.CH2, ADC_CH_MUXPOS_PIN6_gc, ADC_CH_MUXNEG_PIN0_gc);

	/* Configure Event to listen & Action to start on event */
	ADC_Events_Config(&ADCB, ADC_EVSEL_7_gc, ADC_EVACT_SWEEP_gc);

	//configure DMASEL, this enables DMA_CH_TRIGSRC_ADCB_CH4_gc
	ADCB.CTRLA |= ADC_DMASEL_CH012_gc;

	/* Setup sweep of three virtual channels. */
	ADC_SweepChannels_Config(&ADCB, ADC_SWEEP_012_gc);

//	ADC_Ch_Interrupts_Config(&ADCA.CH2, ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
//	ADC_Ch_Interrupts_Config(&ADCB.CH2, ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);

	/* Enable ADC A .*/
	ADC_Enable(&ADCA);

	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_32MHz(&ADCA);
	
	 /* Enable ADC B .*/
	ADC_Enable(&ADCB);
	
	/* Wait until common mode voltage is stable. Default clk is 2MHz and
	 * therefore below the maximum frequency to use this function. */
	ADC_Wait_32MHz(&ADCB);
}

