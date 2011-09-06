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
#include "led_driver.h"
#include "tests.h"
#include "config.h"
#include "error_handler.h"

#define POWERMETER_SAMPLEBUFF 512
#if POWERMETER_SAMPLEBUFF * 18 > 15500
	#error to less RAM, decrease POWERMETER_SAMPLEBUFF
#endif

#define DMARESET 1		//muss 1 sein, 0 == bööööse


//this is called every time a DMA transfers completes

ISR(DMA_CH0_vect)
{
#if DEBUGMODE
	sendUSARTC1_putstr("D");
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	if(powermeter.startCalculations)
		powermeter_Stop();
#else
	#if DMARESET
		DMA_ResetChannel(&DMA.CH0);
		DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	#else
		 DMA_DisableChannel(&DMA.CH0);
	#endif

	//set new page
	if(powermeter.samplebuffer_page){
		DMA0_init((void*)powermeter.samplebuffer[0].u,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}else{
		DMA0_init((void*)powermeter.samplebuffer[1].u,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}
#endif
	powermeter.startCalculations++;		//increment, start calculating if  == 2
}

ISR(DMA_CH1_vect)
{
#if DEBUGMODE
	sendUSARTC1_putstr("D");
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	if(powermeter.startCalculations)
		powermeter_Stop();
#else
	#if DMARESET
		DMA_ResetChannel(&DMA.CH1);
		DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	#else
		 DMA_DisableChannel(&DMA.CH1);
	#endif

	//set new page
	if(powermeter.samplebuffer_page){
		DMA1_init((void*)powermeter.samplebuffer[0].i1,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}else{
		DMA1_init((void*)powermeter.samplebuffer[1].i1,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
	}
#endif
	powermeter.startCalculations++;		//increment, start calculating if  == 2
}


int powermeter_SetSampleratePerPeriod(uint16_t samples)
{
#if DEBUGMODE
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
#if DEBUGMODE
		sendUSARTC1_putstr("Buffer set\n\r");
#endif
	return 0;
}

int powermeter_Start()
{
#if DEBUGMODE
		sendUSARTC1_putstr("powermeter_Start\n\r");
#endif
	if( powermeter.ADCSampleBufferSize > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!powermeter.ADCSampleBufferSize)
		return -1;

	powermeter.samplebuffer_page = 0;
	powermeter.startCalculations = 0;	

	
	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	adc_init();
	uint16_t x;
	for(x=0;x<powermeter.ADCSampleBufferSize;x++)
	    powermeter.samplebuffer[powermeter.samplebuffer_page].i1[x]=0xAA;
#if DEBUGMODE
		sendUSARTC1_putstr("all reg clear\n\r");
#endif
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init((void*)powermeter.samplebuffer[powermeter.samplebuffer_page].u,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
		sendUSARTC1_putstr("DMA0 Init\n\r");
#endif
	
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init((void*)powermeter.samplebuffer[powermeter.samplebuffer_page].i1,sizeof(uint16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
		sendUSARTC1_putstr("DMA1 init\n\r");
#endif
		
	TC1_init(powermeter.ADCSamplesPerSecond);		//set timer1 to trigger ADCSamplesPerSecond times per second
	LED_issampling();	//set blue LED
	return 0;
}


void powermeter_Stop()
{
#if DEBUGMODE
		sendUSARTC1_putstr("powermeter_Stop\n\r");
#endif
	TC1_Reset(&TCC1);	//stop trigger for ADC
	DMA_DisableChannel(&DMA.CH0);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH0);		//stop DMA transfers
	DMA_DisableChannel(&DMA.CH1);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH1);		//stop DMA transf
	ADC_Disable(&ADCA);			//disable ADC
	ADC_Disable(&ADCB);			//disable ADC
	LED_isrunning();		//set green LED
}

void powermeter_docalculations()
{
	//check if calculations has to be done
	if(powermeter.startCalculations == 2)
	{
		powermeter.startCalculations = 0;
		 void * up;
		 void * ip;
		 uint16_t i;
#if DEBUGMODE
		char buf[9];
#endif
		up = (void*)&powermeter.samplebuffer[powermeter.samplebuffer_page].u[0];
		ip =  (void*)&powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0];
		
		for(i = 0;i < powermeter.ADCSampleBufferSize;i+=(3*sizeof(uint16_t)))
		{

		/*	powermeter.powerdraw.c1.Ueff += *((uint16_t*)(up + i));
			powermeter.powerdraw.c2.Ueff += *((uint16_t*)(up + i + sizeof(uint16_t)));
			powermeter.powerdraw.c3.Ueff += *((uint16_t*)(up + i + sizeof(uint16_t)*2));

			powermeter.powerdraw.c1.Ieff += *((uint16_t*)(ip + i));
			powermeter.powerdraw.c2.Ieff += *((uint16_t*)(ip + i + sizeof(uint16_t)));
			powermeter.powerdraw.c3.Ieff += *((uint16_t*)(ip + i + sizeof(uint16_t)*2));	*/
#if DEBUGMODE
			itoa(*((uint16_t*)(up + i)),buf[0],10);
			sendUSARTC1_putstr(buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa( *((uint16_t*)(up + i + sizeof(uint16_t))),buf[0],10);
			sendUSARTC1_putstr(buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*((uint16_t*)(up + i + sizeof(uint16_t)*2)),buf[0],10);
			sendUSARTC1_putstr(buf[0]);
			sendUSARTC1_putstr(" \n");

			itoa(*((uint16_t*)(ip + i )),buf[0],10);
			sendUSARTC1_putstr(buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*((uint16_t*)(ip + i + sizeof(uint16_t))),buf[0],10);
			sendUSARTC1_putstr(buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*((uint16_t*)(ip + i + sizeof(uint16_t)*2)),buf[0],10);
			sendUSARTC1_putstr(buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
		}

	
	//flip page
	if(powermeter.samplebuffer_page)
		powermeter.samplebuffer_page=0;
	else
		powermeter.samplebuffer_page=1;
	}
	if(powermeter.startCalculations > 2)
	{
		//setERROR(ERROR_SR_TOHIGH);
		powermeter.startCalculations = 0;
	}
}