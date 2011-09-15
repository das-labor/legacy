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


powermeter_t powermeter;

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

//this is called every time a DMA transfers completes
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
		DMA_ResetChannel(&DMA.CH1);sendUSARTC1_putstr("\n\rADCSamplesPerSecond ");
		itoa(powermeter.ADCSamplesPerSecond,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr("\n\r");
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
	char buf[9];
		sendUSARTC1_putstr("powermeter_SetSampleratePerPeriod\n\r");
#endif
	if(samples > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!samples)
		return -1;
	//configure Timer1 to generate 50//60 * samples events * 3	(3 channels, 50Hz/60hz)
	powermeter.ADCSampesPerPeriod = samples;
	powermeter.ADCSamplesPerSecond = samples *3 ;//* NET_FREQ * 3;
	powermeter.ADCSampleBufferSize = samples * 3 * sizeof(uint16_t);
#if DEBUGMODE
		sendUSARTC1_putstr("ADCSamplesPerPeriod");

		itoa(samples,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr("\n\rADCSamplesPerSecond ");
		itoa(powermeter.ADCSamplesPerSecond,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr("\n\r");
		sendUSARTC1_putstr("\n\rADCSampleBufferSize ");
		itoa(powermeter.ADCSampleBufferSize,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr("\n\r");
#endif
	return 0;
}

int powermeter_Start()
{
#if DEBUGMODE
	sendUSARTC1_putstr("powermeter_Start:\n\r");
	char buf[9];
#endif
	
	powermeter_clearpowerdraw();
	powermeter_clearpowerdrawPerSecond();
	
	if(powermeter.ADCSampleBufferSize > (POWERMETER_SAMPLEBUFF * 3 * sizeof(uint16_t)))
#if DEBUGMODE
	sendUSARTC1_putstr("ADCSampleBufferSize to large\n\r");
#else
	return -1;
#endif
	if(!powermeter.ADCSampleBufferSize)
#if DEBUGMODE
	sendUSARTC1_putstr("ADCSampleBufferSize = 0\n\r");
#else
	return -1;
#endif
	
	powermeter.samplebuffer_page = 0;
	powermeter.startCalculations = 0;	
	
	powermeter.isrunning = 1;
	
#if DEBUGMODE
	sendUSARTC1_putstr("clear all reg complete\n\r");
#endif	
	
	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	adc_init((int8_t*)&powermeter.ADCoffset.offsetA,(int8_t*)&powermeter.ADCoffset.offsetB);

#if DEBUGMODE
	sendUSARTC1_putstr("offsetA\n\r");
	itoa(powermeter.ADCoffset.offsetA,&buf[0],10);
	sendUSARTC1_putstr(&buf[0]);
	sendUSARTC1_putstr(" \n");
	sendUSARTC1_putstr("offsetB\n\r");
	itoa(powermeter.ADCoffset.offsetB,&buf[0],10);
	sendUSARTC1_putstr(&buf[0]);
	sendUSARTC1_putstr(" \n");
#endif
		
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init((void*)&powermeter.samplebuffer[powermeter.samplebuffer_page].u[0],sizeof(int16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
	sendUSARTC1_putstr("DMA0 Init complete\n\r");
#endif
	
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init((void*)&powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0],sizeof(int16_t)*3,powermeter.ADCSampesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
	sendUSARTC1_putstr("DMA1 init complete\n\r");
#endif
		
	RTC_Init();	//enable 1sec interrupt
#if DEBUGMODE
	sendUSARTC1_putstr("RTC init complete\n\r");
#endif
	LED_issampling();	//set blue LED
#if DEBUGMODE
	sendUSARTC1_putstr("LED set to blue\n\r");
#endif
		TC1_init(powermeter.ADCSamplesPerSecond);		//set timer1 to trigger ADCSamplesPerSecond times per second
#if DEBUGMODE
	sendUSARTC1_putstr("TC1 init complete\n\r");
#endif	
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
	powermeter.isrunning = 0;
}

void powermeter_docalculations()
{
	//check if calculations has to be done
	if(powermeter.startCalculations == 2)
	{
		LED__cyan();	//set LED color cyan
#if DEBUGMODE
		sendUSARTC1_putstr("startCalculations\n\r");
		char buf[9];
#else
		int32_t u;
		int32_t i;
#endif		
		register int16_t * up;		//points to start of array containing the sampled voltages: u1, u2, u3, u1, u2, u3, ....
		register int16_t * ip;		//points to start of array containing the sampled currents: i1, i2, i3, i1, i2 ,i3, ....
		up = &powermeter.samplebuffer[powermeter.samplebuffer_page].u[0];
		ip = &powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0];
		
		for(uint16_t x = 0;x < powermeter.ADCSampesPerPeriod;x++)
		{
#if !DEBUGMODE
			//load u & i
			u = (int32_t)(*up)-powermeter.ADCoffset.offsetA;
			i = (int32_t)(*ip)-powermeter.ADCoffset.offsetB;
#endif
#if DEBUGMODE
			itoa(*up,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*ip,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
			up++;//up+=1;//sizeof(int16_t);	//increment pointer
			ip++;//ip+=1;//sizeof(int16_t);	//increment pointer
#if !DEBUGMODE			
			powermeter.powerdraw.c1.Ueff += u * u;	//calculate Ueff
			powermeter.powerdraw.c1.Ieff += i * i;	//calculate Ieff
			powermeter.powerdraw.c1.P += u * i;	//calculate P

			//load u & i
			u = (int32_t)(*up)-powermeter.ADCoffset.offsetA;
			i = (int32_t)(*ip)-powermeter.ADCoffset.offsetB;			
#endif
#if DEBUGMODE
			itoa(*up,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*ip,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
			up++;//up+=1;//sizeof(int16_t);	//increment pointer
			ip++;//ip+=1;//sizeof(int16_t);	//increment pointer
#if !DEBUGMODE			
			powermeter.powerdraw.c2.Ueff += u * u;
			powermeter.powerdraw.c2.Ieff += i * i;
			powermeter.powerdraw.c2.P += u * i;
			//load u & i
			u = (int32_t)(*up)-powermeter.ADCoffset.offsetA;
			i = (int32_t)(*ip)-powermeter.ADCoffset.offsetB;
#endif
#if DEBUGMODE
			itoa(*up,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
			itoa(*ip,&buf[0],10);
			sendUSARTC1_putstr(&buf[0]);
			sendUSARTC1_putstr(" \n");
#endif
			up++;//up+=1;//sizeof(int16_t);	//increment pointer
			ip++;//ip+=1;//sizeof(int16_t);	//increment pointer
#if !DEBUGMODE			
			powermeter.powerdraw.c3.Ueff += u * u;
			powermeter.powerdraw.c3.Ieff += i * i;
			powermeter.powerdraw.c3.P += u * i;
#endif
		}
#if !DEBUGMODE
		
		//calculate S
		powermeter.powerdrawPerSecond.c1.S += sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )/ powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c2.S += sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )/ powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c3.S += sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )/ powermeter.ADCSampleBufferSize;
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += powermeter.powerdraw.c1.P / powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c2.P += powermeter.powerdraw.c2.P / powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c3.P += powermeter.powerdraw.c3.P / powermeter.ADCSampleBufferSize;
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += sqrt(powermeter.powerdraw.c1.Ueff) / powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c2.Ueff += sqrt(powermeter.powerdraw.c2.Ueff) / powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c3.Ueff += sqrt(powermeter.powerdraw.c3.Ueff) / powermeter.ADCSampleBufferSize;
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff) / powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff) / powermeter.ADCSampleBufferSize;
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff) / powermeter.ADCSampleBufferSize;
		
		//clear powermeter.powerdraw
		powermeter_clearpowerdraw();

#endif
		
	//flip page
	if(powermeter.samplebuffer_page)
		powermeter.samplebuffer_page=0;
	else
		powermeter.samplebuffer_page=1;

	if(powermeter.startCalculations > 2)
		setERROR(ERROR_SR_TOHIGH);

#if DEBUGMODE
		sendUSARTC1_putstr("endCalculations\n\r");
#endif
	LED_on();	//return to normal LED color
	powermeter.startCalculations = 0;
	}
	
	if(powermeter.startCalculations > 2)
	{
		setERROR(ERROR_SR_TOHIGH);
		powermeter.startCalculations = 0;
	}
}

void powermeter_clearpowerdrawPerSecond()
{
	//clear S
	powermeter.powerdrawPerSecond.c1.S = 0;
	powermeter.powerdrawPerSecond.c2.S = 0;
	powermeter.powerdrawPerSecond.c3.S = 0;
	//clear P
	powermeter.powerdrawPerSecond.c1.P = 0;
	powermeter.powerdrawPerSecond.c2.P = 0;
	powermeter.powerdrawPerSecond.c3.P = 0;
	//calculate Ueff
	powermeter.powerdrawPerSecond.c1.Ueff = 0;
	powermeter.powerdrawPerSecond.c2.Ueff = 0;
	powermeter.powerdrawPerSecond.c3.Ueff = 0;
	//calculate Ieff
	powermeter.powerdrawPerSecond.c1.Ieff = 0;
	powermeter.powerdrawPerSecond.c2.Ieff = 0;
	powermeter.powerdrawPerSecond.c3.Ieff = 0;
}

void powermeter_clearpowerdraw()
{
	//clear S
	powermeter.powerdraw.c1.S = 0;
	powermeter.powerdraw.c2.S = 0;
	powermeter.powerdraw.c3.S = 0;
	//clear P
	powermeter.powerdraw.c1.P = 0;
	powermeter.powerdraw.c2.P = 0;
	powermeter.powerdraw.c3.P = 0;
	//calculate Ueff
	powermeter.powerdraw.c1.Ueff = 0;
	powermeter.powerdraw.c2.Ueff = 0;
	powermeter.powerdraw.c3.Ueff = 0;
	//calculate Ieff
	powermeter.powerdraw.c1.Ieff = 0;
	powermeter.powerdraw.c2.Ieff = 0;
	powermeter.powerdraw.c3.Ieff = 0;
}

#if 0
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
#endif