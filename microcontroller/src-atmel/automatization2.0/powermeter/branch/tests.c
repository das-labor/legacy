/* powermeter 
 ADC samples are loaded to powermetersamplebuffer[0] or powermetersamplebuffer[1] using DMA
 after that they will be converted and added to powerdraw[IN_PROGRESS]
 on every second powerdraw[] will be updated:
	 powerdraw[IN_PROGRESS] -> 1s -> powerdraw[LAST_SECOND] -> 60s -> powerdraw[LAST_MINUTE] -> ... */
#include "tc_driver.h"
#include "avr_compiler.h"
#include "ursartC1_driver.h"
#include "dma_driver.h"
#include "adc_driver.h"
#include "tests.h"
#include "config.h"

#define POWERMETER_SAMPLEBUFF 512
#if POWERMETER_SAMPLEBUFF * 18 > 15500
	#error to less RAM, decrease POWERMETER_SAMPLEBUFF
#endif

#define DMARESET 1		//muss 1 sein, 0 == bööööse
#define DMACANCEL !DMARESET
#define DEBUGMODE
//this is called every time a DMA transfers completes

ISR(DMA_CH0_vect)
{
#ifdef DEBUGMODE
		sendUSARTC1_putstr("D");
#endif
	PORTC.OUT |= LED_RED;		//enable RED Led on every complete transfer
	
#if DMARESET
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
#elif DMACANCEL
	 DMA_DisableChannel(&DMA.CH0);
#endif
	
	//set new page
	if(powermeter.samplebuffer_page){
		DMA0_init(powermeter.samplebuffer[0].u,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}else{
		DMA0_init(powermeter.samplebuffer[1].u,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}
	
	powermeter.startCalculations++;		//increment, start calculating if  == 2
	//powermeter_Stop();
}

ISR(DMA_CH1_vect)
{
#ifdef DEBUGMODE
		sendUSARTC1_putstr("D");
#endif
	PORTC.OUT |= LED_RED;		//enable RED Led on every complete transfer

#if DMARESET
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
#elif DMACANCEL
	 DMA_DisableChannel(&DMA.CH1);
#endif
	//set new page
	if(powermeter.samplebuffer_page){
		DMA1_init(powermeter.samplebuffer[0].i1,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}else{
		DMA1_init(powermeter.samplebuffer[1].i1,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}
	powermeter.startCalculations++;		//increment, start calculating if  == 2
}


int powermeter_SetSampleratePerPeriod(uint16_t samples)
{
#ifdef DEBUGMODE
		sendUSARTC1_putstr("powermeter_SetSampleratePerPeriod\n\r");
#endif
	if( samples > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!samples)
		return -1;
	//configure Timer1 to generate 50 * samples events * 3	(3 channels, 50Hz)
	powermeter.ADCSampesPerPeriod = samples;
	powermeter.ADCSamplesPerSecond = samples * 10 * 3;
	powermeter.ADCSampleBufferSize = samples * 3 * sizeof(uint16_t);
#ifdef DEBUGMODE
		sendUSARTC1_putstr("Buffer set\n\r");
#endif
	return 0;
}

int powermeter_Start()
{
#ifdef DEBUGMODE
		sendUSARTC1_putstr("powermeter_Start\n\r");
#endif
	if( powermeter.ADCSampleBufferSize > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!powermeter.ADCSampleBufferSize)
		return -1;

	powermeter.samplebuffer_page = 0;
	powermeter.startCalculations = 0;	

	Eventsystem_init();
	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	adc_init();
	uint16_t x;
	for(x=0;x<powermeter.ADCSampleBufferSize;x++)
	    powermeter.samplebuffer[powermeter.samplebuffer_page].i1[x]=0xAA;
#ifdef DEBUGMODE
		sendUSARTC1_putstr("all reg clear\n\r");
#endif
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init(powermeter.samplebuffer[powermeter.samplebuffer_page].u,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
#ifdef DEBUGMODE
		sendUSARTC1_putstr("DMA0 Init\n\r");
#endif
	
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init(powermeter.samplebuffer[powermeter.samplebuffer_page].i1,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
#ifdef DEBUGMODE
		sendUSARTC1_putstr("DMA1 init\n\r");
#endif
		
	TC1_init(powermeter.ADCSamplesPerSecond);
	return 0;
}


void powermeter_Stop()
{
#ifdef DEBUGMODE
		sendUSARTC1_putstr("powermeter_Stop\n\r");
#endif
	TC1_Reset(&TCC1);	//stop trigger for ADC
	DMA_DisableChannel(&DMA.CH0);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH0);		//stop DMA transfers
	DMA_DisableChannel(&DMA.CH1);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH1);		//stop DMA transf
	ADC_Disable(&ADCA);			//disable ADC
	ADC_Disable(&ADCB);			//disable ADC
}
