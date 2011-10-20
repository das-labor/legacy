/* powermeter 
 ADC samples are loaded to powermetersamplebuffer[0] or powermetersamplebuffer[1] using DMA
 after that they will be converted and added to powerdraw[IN_PROGRESS]
 on every second powerdraw[] will be updated:
	 powerdraw[IN_PROGRESS] -> 1s -> powerdraw[LAST_SECOND] -> 60s -> powerdraw[LAST_MINUTE] -> ... */
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



powermeter_t powermeter;

#define DMARESET 1		//muss 1 sein, 0 == bööööse
#define USEDMAMEMSET 0
#define USEDMAMEMCPY 0

//this is called every time a DMA transfers completes
ISR(DMA_CH0_vect)
{
	#if DMARESET
		DMA_ResetChannel(&DMA.CH0);
		DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);       //enable transfer interrupt, disable err interrupt
	#else
		DMA_DisableChannel(&DMA.CH0);
	#endif

#if DMARESET
	//set new page
	if(powermeter.samplebuffer_page){
		DMA0_init(&powermeter.samplebuffer[0].u[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
	}else{
		DMA0_init(&powermeter.samplebuffer[1].u[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
	}
#else
	if(powermeter.samplebuffer_page){
		DMA.CH0.DESTADDR0 = ((uint16_t)&powermeter.samplebuffer[0].u[0]) & 0xFF;
		DMA.CH0.DESTADDR1 = (((uint16_t)&powermeter.samplebuffer[0].u[0])>>8) & 0xFF;
		DMA.CH0.DESTADDR2 = 0;
	}else{
		DMA.CH0.DESTADDR0 = ((uint16_t)&powermeter.samplebuffer[1].u[0]) & 0xFF;
		DMA.CH0.DESTADDR1 = (((uint16_t)&powermeter.samplebuffer[1].u[0])>>8) & 0xFF;
		DMA.CH0.DESTADDR2 = 0;
	}
	DMA.CH0.REPCNT = powermeter.ADCSamplesPerPeriod;
	DMA.CH0.CTRLA |= DMA_CH_REPEAT_bm;

	DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
#endif
	powermeter.startCalculations++;		//increment, start calculating if  == 2
}

//this is called every time a DMA transfers completes
ISR(DMA_CH1_vect)
{
	#if DMARESET
		DMA_ResetChannel(&DMA.CH1);
		DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);       //enable transfer interrupt, disable err interrupt
	#else
		DMA_DisableChannel(&DMA.CH1);
	#endif

#if DMARESET
	//set new page
	if(powermeter.samplebuffer_page){
		DMA1_init((void*)&powermeter.samplebuffer[0].i1[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
	}else{
		DMA1_init((void*)&powermeter.samplebuffer[1].i1[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
	}
#else
	if(powermeter.samplebuffer_page){
		DMA.CH1.DESTADDR0 = ((uint16_t)&powermeter.samplebuffer[0].i1[0]) & 0xFF;
		DMA.CH1.DESTADDR1 = (((uint16_t)&powermeter.samplebuffer[0].i1[0])>>8) & 0xFF;
		DMA.CH1.DESTADDR2 = 0;
	}else{
		DMA.CH1.DESTADDR0 = ((uint16_t)&powermeter.samplebuffer[1].i1[0]) & 0xFF;
		DMA.CH1.DESTADDR1 = (((uint16_t)&powermeter.samplebuffer[1].i1[0])>>8) & 0xFF;
		DMA.CH1.DESTADDR2 = 0;
	}
	DMA.CH1.REPCNT = powermeter.ADCSamplesPerPeriod;
	DMA.CH1.CTRLA |= DMA_CH_REPEAT_bm;
	DMA.CH1.CTRLA |= DMA_CH_ENABLE_bm;
#endif
	powermeter.startCalculations++;		//increment, start calculating if  == 2
}


int powermeter_SetSampleratePerPeriod(uint16_t samples)
{
	if(samples > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!samples)
		return -1;
	//configure Timer1 to generate 50//60 * samples events 
	powermeter.ADCSamplesPerPeriod = samples;
	powermeter.ADCSamplesPerSecond = samples * NET_FREQ;
	powermeter.ADCSampleBufferSize = samples * 3;
	return 0;
}

int powermeter_Start()
{
	powermeter_clearpowerdraw();
	powermeter_clearpowerdrawPerSecond();

	if(powermeter.ADCSampleBufferSize > (POWERMETER_SAMPLEBUFF * 3))
		return -1;
	if(!powermeter.ADCSampleBufferSize)
		return -1;

	powermeter.samplebuffer_page = 0;
	powermeter.startCalculations = 0;

	powermeter.isrunning = 1;

	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	ADC_init();

	DMA_Enable();
	DMA_SetPriority(DMA_PRIMODE_RR0123_gc); //dma mode round robin
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init(&powermeter.samplebuffer[powermeter.samplebuffer_page].u[0],sizeof(int16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination

	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init(&powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0],sizeof(int16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination


	RTC_Init();	//enable 1sec interrupt
	LED_issampling();	//set blue LED
	TC1_init(powermeter.ADCSamplesPerSecond);		//set timer1 to trigger ADCSamplesPerSecond times per second
	return 0;
}


void powermeter_Stop()
{
	TC1_Reset(&TCC1);	//stop trigger for ADC
	DMA_DisableChannel(&DMA.CH0);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH0);		//stop DMA transfers
	DMA_DisableChannel(&DMA.CH1);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH1);		//stop DMA transf
	ADC_Disable(&ADCA);			//disable ADC
	ADC_Disable(&ADCB);			//disable ADC
	LED_isrunning();		//set green LED
	powermeter.isrunning = 0;
}
#define DEBUGMODE_CAN 0
void powermeter_docalculations()
{
	//check if calculations has to be done
	if(powermeter.startCalculations > 1)
	{
		LED__cyan();	//set LED color cyan

		int32_t u;
		int32_t i;
		register int16_t * up;		//points to start of array containing the sampled voltages: u1, u2, u3, u1, u2, u3, ....
		register int16_t * ip;		//points to start of array containing the sampled currents: i1, i2, i3, i1, i2 ,i3, ....
		up = &powermeter.samplebuffer[powermeter.samplebuffer_page].u[0];
		ip = &powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0];
		for(uint16_t x = 0;x < powermeter.ADCSamplesPerPeriod;x++)
		{
			//load u & i
			u = (int32_t)(*up)-powermeter.ADCoffset.offsetA;
			i = (int32_t)(*ip)-powermeter.ADCoffset.offsetB;
			up++;//increment pointer
			ip++;//increment pointer

			powermeter.powerdraw.c1.Ueff += u * u;	//calculate Ueff
			powermeter.powerdraw.c1.Ieff += i * i;	//calculate Ieff
			powermeter.powerdraw.c1.P += -u * i;	//calculate P

			//load u & i
			u = (int32_t)(*up)-powermeter.ADCoffset.offsetA;
			i = (int32_t)(*ip)-powermeter.ADCoffset.offsetB;

			up++;//increment pointer
			ip++;//increment pointer

			powermeter.powerdraw.c2.Ueff += u * u;
			powermeter.powerdraw.c2.Ieff += i * i;
			powermeter.powerdraw.c2.P += -u * i;
			//load u & i
			u = (int32_t)(*up)-powermeter.ADCoffset.offsetA;
			i = (int32_t)(*ip)-powermeter.ADCoffset.offsetB;

			up++;//increment pointer
			ip++;//increment pointer

			powermeter.powerdraw.c3.Ueff += u * u;
			powermeter.powerdraw.c3.Ieff += i * i;
			powermeter.powerdraw.c3.P += -u * i;
		}
		
		DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_OFF_gc,DMA_CH_ERRINTLVL_OFF_gc);
		DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_OFF_gc,DMA_CH_ERRINTLVL_OFF_gc);

		//flip page
		if(powermeter.samplebuffer_page)
			powermeter.samplebuffer_page=0;
		else
			powermeter.samplebuffer_page=1;

		if(powermeter.startCalculations > 2)
			setERROR(ERROR_SR_TOHIGH);
		powermeter.startCalculations = 0;

		DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);
		DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);

		//disable RTC Interrupt, it modifies powermeter.powerdrawPerSecond
		RTC_SetIntLevels( RTC_OVFINTLVL_OFF_gc, RTC_COMPINTLVL_OFF_gc );

#if USE_STATIC_ADCSamplesPerPeriod
#if ADCSAMPLESPERPERIOD == 256
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff >>8));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff >>8));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff >>8));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff >>8);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff >>8);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff >>8);
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += (((uint32_t)sqrt(powermeter.powerdraw.c1.Ueff) * (uint32_t)sqrt(powermeter.powerdraw.c1.Ieff ))>>8);
		powermeter.powerdrawPerSecond.c2.S += (((uint32_t)sqrt(powermeter.powerdraw.c2.Ueff) * (uint32_t)sqrt(powermeter.powerdraw.c2.Ieff ))>>8);
		powermeter.powerdrawPerSecond.c3.S += (((uint32_t)sqrt(powermeter.powerdraw.c3.Ueff) * (uint32_t)sqrt(powermeter.powerdraw.c3.Ieff ))>>8);
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P >>8);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P >>8);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P >>8);
#elif ADCSAMPLESPERPERIOD == 128
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += ((uint32_t)sqrt(powermeter.powerdraw.c1.Ueff>>7) * (uint32_t)sqrt(powermeter.powerdraw.c1.Ieff >>7));
		powermeter.powerdrawPerSecond.c2.S += ((uint32_t)sqrt(powermeter.powerdraw.c2.Ueff>>7) * (uint32_t)sqrt(powermeter.powerdraw.c2.Ieff >>7));
		powermeter.powerdrawPerSecond.c3.S += ((uint32_t)sqrt(powermeter.powerdraw.c3.Ueff>>7) * (uint32_t)sqrt(powermeter.powerdraw.c3.Ieff >>7));
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P >>7);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P >>7);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P >>7);
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff >>7));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff >>7));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff >>7));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff >>7);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff >>7);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff >>7);

#elif ADCSAMPLESPERPERIOD == 64
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += (sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )>>6);
		powermeter.powerdrawPerSecond.c2.S += (sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )>>6);
		powermeter.powerdrawPerSecond.c3.S += (sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )>>6);
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P >>6);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P >>6);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P >>6);
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff >>6));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff >>6));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff >>6));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff >>6);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff >>6);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff >>6);

#elif ADCSAMPLESPERPERIOD == 32
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += (sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )>>5);
		powermeter.powerdrawPerSecond.c2.S += (sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )>>5);
		powermeter.powerdrawPerSecond.c3.S += (sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )>>5);
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P >>5);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P >>5);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P >>5);
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff >>5));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff >>5));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff >>5));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff >>5);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff >>5);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff >>5);

#elif ADCSAMPLESPERPERIOD == 16
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += (sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )>>4);
		powermeter.powerdrawPerSecond.c2.S += (sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )>>4);
		powermeter.powerdrawPerSecond.c3.S += (sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )>>4);
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P >>4);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P >>4);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P >>4);
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff >>4));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff >>4));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff >>4));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff >>4);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff >>4);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff >>4);
#elif ADCSAMPLESPERPERIOD == 8
//calculate S
		powermeter.powerdrawPerSecond.c1.S += (sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )>>3);
		powermeter.powerdrawPerSecond.c2.S += (sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )>>3);
		powermeter.powerdrawPerSecond.c3.S += (sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )>>3);
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P >>3);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P >>3);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P >>3);
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff >>3));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff >>3));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff >>3));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff >>3);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff >>3);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff >>3);
#else
#	error "unknown value of ADCSAMPLESPERPERIOD"
#endif
#else
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += (sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )/ powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c2.S += (sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )/ powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c3.S += (sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )/ powermeter.ADCSamplesPerPeriod);
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P / powermeter.ADCSamplesPerPeriod);
		//calculate E
		powermeter.powerdrawPerSecond.c1.E += powermeter.powerdraw.c1.E;
		powermeter.powerdrawPerSecond.c2.E += powermeter.powerdraw.c2.E;
		powermeter.powerdrawPerSecond.c3.E += powermeter.powerdraw.c3.E;
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff / powermeter.ADCSamplesPerPeriod));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff / powermeter.ADCSamplesPerPeriod));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff / powermeter.ADCSamplesPerPeriod));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff / powermeter.ADCSamplesPerPeriod);
#endif
		powermeter.samplesPerSecondDone++;
		//enable RTC Interrupt
		RTC_SetIntLevels( RTC_OVFINTLVL_LO_gc, RTC_COMPINTLVL_OFF_gc );
		//clear powermeter.powerdraw

		powermeter_clearpowerdraw();

		LED_on();	//return to normal LED color
	}
}

void powermeter_copypowerdraw()
{
//#if USEDMAMEMCPY
//	DMA_memcpy(&powermeter.powerdrawLastSecond,&powermeter.powerdrawPerSecond,sizeof(powermeter_channel_t));
//#else
	if(powermeter.samplesPerSecondDone)
	{
		powermeter.powerdrawLastSecond.c1.S = powermeter.powerdrawPerSecond.c1.S/powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c2.S = powermeter.powerdrawPerSecond.c2.S/powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c3.S = powermeter.powerdrawPerSecond.c3.S/powermeter.samplesPerSecondDone;

		powermeter.powerdrawLastSecond.c1.P = powermeter.powerdrawPerSecond.c1.P/powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c2.P = powermeter.powerdrawPerSecond.c2.P/powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c3.P = powermeter.powerdrawPerSecond.c3.P/powermeter.samplesPerSecondDone;

		powermeter.powerdrawLastSecond.c1.Ueff = powermeter.powerdrawPerSecond.c1.Ueff /powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c2.Ueff = powermeter.powerdrawPerSecond.c2.Ueff /powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c3.Ueff = powermeter.powerdrawPerSecond.c3.Ueff /powermeter.samplesPerSecondDone;

		powermeter.powerdrawLastSecond.c1.Ieff = powermeter.powerdrawPerSecond.c1.Ieff /powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c2.Ieff = powermeter.powerdrawPerSecond.c2.Ieff /powermeter.samplesPerSecondDone;
		powermeter.powerdrawLastSecond.c3.Ieff = powermeter.powerdrawPerSecond.c3.Ieff /powermeter.samplesPerSecondDone;
	}
//#endif
}

void powermeter_clearpowerdrawPerSecond()
{
#if USEDMAMEMSET
	DMA_memset(&powermeter.powerdrawPerSecond, 0x00, sizeof(powermeter_channel_t));	
#else
	//clear S
	powermeter.powerdrawPerSecond.c1.S = 0;
	powermeter.powerdrawPerSecond.c2.S = 0;
	powermeter.powerdrawPerSecond.c3.S = 0;
	//clear P
	powermeter.powerdrawPerSecond.c1.P = 0;
	powermeter.powerdrawPerSecond.c2.P = 0;
	powermeter.powerdrawPerSecond.c3.P = 0;
    // DO NOT reset E!
	//powermeter.powerdrawPerSecond.c1.E = 0;
	//powermeter.powerdrawPerSecond.c2.E = 0;
	//powermeter.powerdrawPerSecond.c3.E = 0;
	//calculate Ueff
	powermeter.powerdrawPerSecond.c1.Ueff = 0;
	powermeter.powerdrawPerSecond.c2.Ueff = 0;
	powermeter.powerdrawPerSecond.c3.Ueff = 0;
	//calculate Ieff
	powermeter.powerdrawPerSecond.c1.Ieff = 0;
	powermeter.powerdrawPerSecond.c2.Ieff = 0;
	powermeter.powerdrawPerSecond.c3.Ieff = 0;
#endif
	powermeter.samplesPerSecondDone=0;
}

void powermeter_clearpowerdraw()
{
#if USEDMAMEMSET
	DMA_memset(&powermeter.powerdraw, 0x00, sizeof(powermeter_channel_t));
#else
	//clear S
	powermeter.powerdraw.c1.S = 0;
	powermeter.powerdraw.c2.S = 0;
	powermeter.powerdraw.c3.S = 0;
	//clear P
	powermeter.powerdraw.c1.P = 0;
	powermeter.powerdraw.c2.P = 0;
	powermeter.powerdraw.c3.P = 0;
	//clear E
	powermeter.powerdraw.c1.E = 0;
	powermeter.powerdraw.c2.E = 0;
	powermeter.powerdraw.c3.E = 0;
	//calculate Ueff
	powermeter.powerdraw.c1.Ueff = 0;
	powermeter.powerdraw.c2.Ueff = 0;
	powermeter.powerdraw.c3.Ueff = 0;
	//calculate Ieff
	powermeter.powerdraw.c1.Ieff = 0;
	powermeter.powerdraw.c2.Ieff = 0;
	powermeter.powerdraw.c3.Ieff = 0;
#endif
}

void powermeter_clearpowerdrawLastSecond()
{
#if USEDMAMEMSET
	DMA_memset(&powermeter.powerdrawLastSecond, 0x00, sizeof(powermeter_channel_t));
#else
	//clear S
	powermeter.powerdrawLastSecond.c1.S = 0;
	powermeter.powerdrawLastSecond.c2.S = 0;
	powermeter.powerdrawLastSecond.c3.S = 0;
	//clear P
	powermeter.powerdrawLastSecond.c1.P = 0;
	powermeter.powerdrawLastSecond.c2.P = 0;
	powermeter.powerdrawLastSecond.c3.P = 0;
	//calculate Ueff
	powermeter.powerdrawLastSecond.c1.Ueff = 0;
	powermeter.powerdrawLastSecond.c2.Ueff = 0;
	powermeter.powerdrawLastSecond.c3.Ueff = 0;
	//calculate Ieff
	powermeter.powerdrawLastSecond.c1.Ieff = 0;
	powermeter.powerdrawLastSecond.c2.Ieff = 0;
	powermeter.powerdrawLastSecond.c3.Ieff = 0;
#endif
}

void TC1_init(volatile uint32_t eventsPerSecond)
{

	if(eventsPerSecond > 0xF42400)		//greater than 16000000Hz ?
		return;
	TC1_Reset(&TCC1);
	TC1_SetOverflowIntLevel(&TCC1,TC_OVFINTLVL_MED_gc);
	uint16_t TC1_TOP;
	if((F_CPU/(1*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 1) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);

		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV1_gc );
		return;
	}

	if((F_CPU/(2*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 2) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV2_gc );
		return;
	}

	if((F_CPU/(4*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 4) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV4_gc );
		return;
	}

	if((F_CPU/(8*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 8) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV8_gc );
		return;
	}
	if((F_CPU/(64*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 64) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV64_gc );
		return;
	}
		if(F_CPU/(256*eventsPerSecond)<0xFFFF){
		TC1_TOP = ((F_CPU / 256) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV256_gc );
		return;
	}
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
	RTC_SetIntLevels( RTC_OVFINTLVL_LO_gc, RTC_COMPINTLVL_OFF_gc );

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

volatile uint8_t can_send_packet;
void RTC_seconds_int(void)
{
	powermeter_copypowerdraw();	//TODO: move to main()
	powermeter_clearpowerdrawPerSecond();
	//can transmit
	can_send_packet=1;
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



void DMA0_init(void * destAddr, uint8_t blockSize, uint8_t count)
{
	DMA_SetupBlock( &DMA.CH0,					//channel 0
			&ADCA.CH0RES,				//source-addr
			DMA_CH_SRCRELOAD_BLOCK_gc,	//srcDirection reload after each block
			DMA_CH_SRCDIR_INC_gc,		//srcDirection increment after each byte
			destAddr,						//set destAddr
			DMA_CH_DESTRELOAD_NONE_gc,		//reload destAddr after transaction,DMA_CH_DESTRELOAD_TRANSACTION_gc
			DMA_CH_DESTDIR_INC_gc, 		//destDirection increment destination memory addr
			blockSize,							//blockSize in bytes >= burstlen
			DMA_CH_BURSTLEN_2BYTE_gc,	//burstMode 2byte per burst
			count,							//repeat count times
			true );						//repeat

	DMA_SetTriggerSource(&DMA.CH0, DMA_CH_TRIGSRC_ADCA_CH4_gc);	//Trigger on ADCA_CH0 - 3
	DMA_EnableSingleShot(&DMA.CH0);
	DMA_EnableChannel(&DMA.CH0);
}

void DMA1_init(void * destAddr, uint8_t blockSize, uint8_t count)
{
	DMA_SetupBlock( &DMA.CH1,					//channel 1
			&ADCB.CH0RES,				//source-addr
			DMA_CH_SRCRELOAD_BLOCK_gc,	//srcDirection reload after each block
			DMA_CH_SRCDIR_INC_gc,		//srcDirection increment after each byte
			destAddr,						//set destAddr
			DMA_CH_DESTRELOAD_NONE_gc,		//reload destAddr after transaction
			DMA_CH_DESTDIR_INC_gc,		//destDirection increment destination memory addr
			blockSize,							//blockSize in bytes >= burstlen
			DMA_CH_BURSTLEN_2BYTE_gc,	//burstMode 2byte per burst
			count,							//repeat count times
			true );						//repeat

	DMA_SetTriggerSource(&DMA.CH1, DMA_CH_TRIGSRC_ADCB_CH4_gc);	//Trigger on ADCB_CH2
	DMA_EnableSingleShot(&DMA.CH1);
	DMA_EnableChannel(&DMA.CH1);
}

void DMA_memset(void * ptr, uint8_t val, uint16_t size)
{
	DMA_waitfor_memset_finish

	DMA_SetupBlock( &DMA.CH2,					//channel 0
			&val,				//source-addr
			DMA_CH_SRCRELOAD_NONE_gc,	//srcDirection reload after each block
			DMA_CH_SRCDIR_FIXED_gc,		//srcDirection increment after each byte
			ptr,						//set destAddr
			DMA_CH_DESTRELOAD_NONE_gc,		//reload destAddr after transaction,DMA_CH_DESTRELOAD_TRANSACTION_gc
			DMA_CH_DESTDIR_INC_gc,		//destDirection increment destination memory addr
			size,							//blockSize in bytes >= burstlen
			DMA_CH_BURSTLEN_1BYTE_gc,	//burstMode 2byte per burst
			0,							//repeat count times
			false );						//repeat

	DMA_EnableChannel(&DMA.CH2);
	DMA_StartTransfer(&DMA.CH2);
}

void DMA_memcpy(void * ptr_dest, void * ptr_src, uint16_t size)
{
	DMA_waitfor_memcpy_finish

	DMA_SetupBlock( &DMA.CH3,					//channel 0
			ptr_src,				//source-addr
			DMA_CH_SRCRELOAD_NONE_gc,	//srcDirection reload after each block
			DMA_CH_SRCDIR_INC_gc,		//srcDirection increment after each byte
			ptr_dest,						//set destAddr
			DMA_CH_DESTRELOAD_NONE_gc,		//reload destAddr after transaction,DMA_CH_DESTRELOAD_TRANSACTION_gc
			DMA_CH_DESTDIR_INC_gc,		//destDirection increment destination memory addr
			size,							//blockSize in bytes >= burstlen
			DMA_CH_BURSTLEN_1BYTE_gc,	//burstMode 2byte per burst
			0,							//repeat count times
			false );						//repeat

	DMA_EnableChannel(&DMA.CH3);
	DMA_StartTransfer(&DMA.CH3);
}



void ADC_init()
{
	/* Move stored calibration values to ADC A. */
	ADC_CalibrationValues_Load(&ADCA);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate. */
	//ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);	// 1 MSPs
	ADC_Prescaler_Config(&ADCA,  ADC_PRESCALER_DIV128_gc);	// 250ksps
	

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
	ADC_Events_Config(&ADCA, ADC_EVSEL_7_gc,  ADC_EVACT_SWEEP_gc);

	/* Setup sweep of three virtual channels. */
	ADC_SweepChannels_Config(&ADCA, ADC_SWEEP_012_gc);

	//configure DMASEL, this enables DMA_CH_TRIGSRC_ADCA_CH4_gc
	ADCA.CTRLA |= ADC_DMASEL_CH012_gc;

	/*  ADC_Ch_Interrupts_Config(&ADCA.CH0,ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
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


ISR(TCC1_OVF_vect)
{
	powermeter.timercc1clks++;
}


