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
#include "rtc_driver.h"
#include "powermeter_driver.h"
#include "error_handler.h"

#include "config.h"

#include "math.h"


volatile powermeter_t *powermeter;

#define DMARESET 1		//muss 1 sein, 0 == bööööse


//this is called every time a DMA transfers completes
ISR(DMA_CH0_vect)
{
#if DEBUGMODE
	sendUSARTC1_putstr("D");
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	if(powermeter->startCalculations)
		powermeter_Stop();
#else
	#if DMARESET
		DMA_ResetChannel(&DMA.CH0);
		DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	#else
		DMA_DisableChannel(&DMA.CH0);
	#endif

	//set new page
	if(powermeter->samplebuffer_page){
		DMA0_init((void*)powermeter->samplebuffer[0].u,sizeof(uint16_t)*3,powermeter->ADCSampesPerPeriod);	//set DMA transfer destination
	}else{
		DMA0_init((void*)powermeter->samplebuffer[1].u,sizeof(uint16_t)*3,powermeter->ADCSampesPerPeriod);	//set DMA transfer destination
	}
#endif
	powermeter->startCalculations++;		//increment, start calculating if  == 2
}

//this is called every time a DMA transfers completes
ISR(DMA_CH1_vect)
{
#if DEBUGMODE
	sendUSARTC1_putstr("D");
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	if(powermeter->startCalculations)
		powermeter_Stop();
#else
	#if DMARESET
		DMA_ResetChannel(&DMA.CH1);
		DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	#else
		 DMA_DisableChannel(&DMA.CH1);
	#endif

	//set new page
	if(powermeter->samplebuffer_page){
		DMA1_init((void*)powermeter->samplebuffer[0].i1,sizeof(uint16_t)*3,powermeter->ADCSampesPerPeriod);	//set DMA transfer destination
	}else{
		DMA1_init((void*)powermeter->samplebuffer[1].i1,sizeof(uint16_t)*3,powermeter->ADCSampesPerPeriod);	//set DMA transfer destination
	}
#endif
	powermeter->startCalculations++;		//increment, start calculating if  == 2
}


int powermeter_SetSampleratePerPeriod(uint16_t samples)
{
#if DEBUGMODE
	char buf[9];
		sendUSARTC1_putstr("powermeter_SetSampleratePerPeriod\n\r");
#endif
	if(!powermeter)
		return -1;
	if( samples > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!samples)
		return -1;
	//configure Timer1 to generate 50 * samples events * 3	(3 channels, 50Hz)
	powermeter->ADCSampesPerPeriod = samples;
	powermeter->ADCSamplesPerSecond = samples * 50 * 3;
	powermeter->ADCSampleBufferSize = samples * 3 * sizeof(uint16_t);
#if DEBUGMODE
		sendUSARTC1_putstr("ADCSampleRate:\n\rADCSamplesPerPeriod");

		itoa(samples,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr("\n\rADCSamplesPerSecond ");
		itoa(powermeter->ADCSamplesPerSecond,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr("\n\r");

#endif
	return 0;
}

void powermeter_Init()
{
	if(!powermeter)
		powermeter=calloc(sizeof(powermeter_t),0);	
	
	if(!powermeter->powerdraw)
		powermeter->powerdraw=calloc(sizeof(powermeter_channel_t),0);
	
	if(!powermeter->powerdrawPerSecond)
		powermeter->powerdrawPerSecond=calloc(sizeof(powermeter_channel_t),0);
}

int powermeter_Start()
{
#if DEBUGMODE
		sendUSARTC1_putstr("powermeter_Start:\n\r");
#endif
	if(!powermeter)
		return -1;
	if(powermeter->ADCSampleBufferSize > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!powermeter->ADCSampleBufferSize)
		return -1;
	if(!powermeter->powerdraw)
		return -1;
	if(!powermeter->powerdrawPerSecond)
		return -1;
	
	powermeter->samplebuffer_page = 0;
	powermeter->startCalculations = 0;	
	powermeter->isrunning = 1;
	
#if DEBUGMODE
		sendUSARTC1_putstr("clear all reg\n\r");
#endif	
	
	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	adc_init();

	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init((void*)powermeter->samplebuffer[powermeter->samplebuffer_page].u,sizeof(uint16_t)*3,powermeter->ADCSampesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
		sendUSARTC1_putstr("DMA0 Init\n\r");
#endif
	
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init((void*)powermeter->samplebuffer[powermeter->samplebuffer_page].i1,sizeof(uint16_t)*3,powermeter->ADCSampesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
		sendUSARTC1_putstr("DMA1 init\n\r");
#endif
		
	TC1_init(powermeter->ADCSamplesPerSecond);		//set timer1 to trigger ADCSamplesPerSecond times per second
	RTC_Init();	//enable 1sec interrupt
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
	powermeter->isrunning = 0;
	
	if(powermeter->powerdraw)
		free(powermeter->powerdraw);
	
	if(powermeter->powerdrawPerSecond)
		free(powermeter->powerdrawPerSecond);
}

void powermeter_docalculations()
{
	//check if calculations has to be done
	if(powermeter->startCalculations == 2)
	{
		LED__cyan();	//set LED color cyan
#if DEBUGMODE
		sendUSARTC1_putstr("startCalculations\n\r");
		char buf[9];
#else
		long u;
		long i;
#endif		
		register int * up;		//points to start of array containing the sampled voltages: u1, u2, u3, u1, u2, u3, ....
		register int * ip;		//points to start of array containing the sampled currents: i1, i2, i3, i1, i2 ,i3, ....
		up = (int*)&powermeter->samplebuffer[powermeter->samplebuffer_page].u[0];
		ip = (int*)&powermeter->samplebuffer[powermeter->samplebuffer_page].i1[0];
		
		uint16_t x;
		for(x = 0;x < powermeter->ADCSampesPerPeriod;x++)
		{
#if !DEBUGMODE
			//load u & i
			u = (long)(*up);
			i = (long)(*ip);
#endif
#if DEBUGMODE
			itoa(*up,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*ip,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
			up+=sizeof(int);	//increment pointer
			ip+=sizeof(int);	//increment pointer
#if !DEBUGMODE			
			powermeter->powerdraw->c1.Ueff += u * u;	//calculate Ueff
			powermeter->powerdraw->c1.Ieff += i * i;	//calculate Ieff
			powermeter->powerdraw->c1.P += u * i;	//calculate P

			//load u & i
			u = (long)(*up);
			i = (long)(*ip);			
#endif
#if DEBUGMODE
			itoa(*up,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*ip,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
			up+=sizeof(int);	//increment pointer
			ip+=sizeof(int);	//increment pointer
#if !DEBUGMODE			
			powermeter->powerdraw->c2.Ueff += u * u;
			powermeter->powerdraw->c2.Ieff += i * i;
			powermeter->powerdraw->c2.P += u * i;
			//load u & i
			u = (long)(*up);
			i = (long)(*ip);
#endif
#if DEBUGMODE
			itoa(*up,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*ip,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
			up+=sizeof(int);	//increment pointer
			ip+=sizeof(int);	//increment pointer
#if !DEBUGMODE			
			powermeter->powerdraw->c3.Ueff += u * u;
			powermeter->powerdraw->c3.Ieff += i * i;
			powermeter->powerdraw->c3.P += u * i;
#endif
		}
#if !DEBUGMODE
		
		//powermeter->powerdraw->c1.S += sqrt(powermeter->powerdraw->c1.Ueff * powermeter->powerdraw->c1.Ieff )/ powermeter->ADCSampleBufferSize;
		//powermeter->powerdraw->c2.S += sqrt(powermeter->powerdraw->c2.Ueff * powermeter->powerdraw->c2.Ieff )/ powermeter->ADCSampleBufferSize;
		//powermeter->powerdraw->c3.S += sqrt(powermeter->powerdraw->c3.Ueff * powermeter->powerdraw->c3.Ieff )/ powermeter->ADCSampleBufferSize;
		//powermeter->powerdraw->c1.P /= powermeter->ADCSampleBufferSize;
		//powermeter->powerdraw->c2.P /= powermeter->ADCSampleBufferSize;
		//powermeter->powerdraw->c3.P /= powermeter->ADCSampleBufferSize;

		//check if we have to clear powermeter->powerdrawPerSecond
		if(powermeter->clearpowerdrawPerSecond)
		{
			//memset(powermeter->powerdrawPerSecond,0x00,sizeof(powermeter_channel_t));
			powermeter->clearpowerdrawPerSecond=0;
		}
		//calculate S
		powermeter->powerdrawPerSecond->c1.S += sqrt(powermeter->powerdraw->c1.Ueff * powermeter->powerdraw->c1.Ieff )/ powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c2.S += sqrt(powermeter->powerdraw->c2.Ueff * powermeter->powerdraw->c2.Ieff )/ powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c3.S += sqrt(powermeter->powerdraw->c3.Ueff * powermeter->powerdraw->c3.Ieff )/ powermeter->ADCSampleBufferSize;
		//calculate P
		powermeter->powerdrawPerSecond->c1.P += powermeter->powerdraw->c1.P / powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c2.P += powermeter->powerdraw->c2.P / powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c3.P += powermeter->powerdraw->c3.P / powermeter->ADCSampleBufferSize;
		//calculate Ueff
		powermeter->powerdrawPerSecond->c1.Ueff += sqrt(powermeter->powerdraw->c1.Ueff) / powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c2.Ueff += sqrt(powermeter->powerdraw->c2.Ueff) / powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c3.Ueff += sqrt(powermeter->powerdraw->c3.Ueff) / powermeter->ADCSampleBufferSize;
		//calculate Ieff
		powermeter->powerdrawPerSecond->c1.Ieff += sqrt(powermeter->powerdraw->c1.Ieff) / powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c2.Ieff += sqrt(powermeter->powerdraw->c2.Ieff) / powermeter->ADCSampleBufferSize;
		powermeter->powerdrawPerSecond->c3.Ieff += sqrt(powermeter->powerdraw->c3.Ieff) / powermeter->ADCSampleBufferSize;
		
		//clear powermeter->powerdraw
		memsetv(powermeter->powerdraw,0,sizeof(powermeter_channel_t));


#endif
		
	//flip page
	if(powermeter->samplebuffer_page)
		powermeter->samplebuffer_page=0;
	else
		powermeter->samplebuffer_page=1;

	if(powermeter->startCalculations > 2)
		setERROR(ERROR_SR_TOHIGH);

#if DEBUGMODE
		sendUSARTC1_putstr("endCalculations\n\r");
#endif
	LED_on();	//return to normal LED color
	powermeter->startCalculations = 0;
	}
	
	if(powermeter->startCalculations > 2)
	{
		setERROR(ERROR_SR_TOHIGH);
		powermeter->startCalculations = 0;
	}
}

//TODO
int powermeter_createDATAPACKET(void* can_packet)
{
	return 0;
}

void powermeter_clearBuf()
{
	//mark powermeter->powerdrawPerSecond to be cleared
	powermeter->clearpowerdrawPerSecond = 1;
}

void memsetv(void *p,uint8_t value, uint16_t size)
{
	if(!p || !size)
		return;
	uint16_t cnt;
	for(cnt=0;cnt<size;cnt++){
		*((uint8_t*)p)=value;
		p++;
	}
}