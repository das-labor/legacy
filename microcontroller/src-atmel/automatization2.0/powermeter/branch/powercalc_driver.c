/* powermeter 
 ADC samples are loaded to powermetersamplebuffer[0] or powermetersamplebuffer[1] using DMA
 after that they will be converted and added to powerdraw[IN_PROGRESS]
 on every second powerdraw[] will be updated:
	 powerdraw[IN_PROGRESS] -> 1s -> powerdraw[LAST_SECOND] -> 60s -> powerdraw[LAST_MINUTE] -> ... */
#include "tc_timer.h"
#include "dma_driver.h"
#include "adc_driver.h"
#include "avr_compiler.h"

#define IN_PROGRESS 0
#define LAST_SECOND 1
#define LAST_MINUTE 2
#define LAST_HOUR 3
#define LAST_DAY 4
#define LAST_MONTH 5
#define LAST_YEAR 6
#define GLOBAL 7 		//counter that wont reset
#define CHANNEL_A 8		//counter reset on read
#define CHANNEL_B 9		//counter reset on read

#define POWERMETER_SAMPLEBUFF 512
#if POWERMETER_SAMPLEBUFF * 18 > 15500
	#error to less RAM, decrease POWERMETER_SAMPLEBUFF
#endif

typedef struct
{
	uint32_t Ueff;	
	uint32_t Ieff;
	uint32_t P;
	uint32_t S;
	
} powermeter_data_t;

typedef struct
{
	powermeter_data_t c1;
	powermeter_data_t c2;
	powermeter_data_t c3;
	
} powermeter_channel_t;

typedef struct
{
	uint16_t u[POWERMETER_SAMPLEBUFF * 3];

	uint16_t i1[POWERMETER_SAMPLEBUFF * 3];
	uint16_t i2[POWERMETER_SAMPLEBUFF * 3];
} samplebuffer_t;

typedef struct
{
	uint16_t ADCSamplesPerSecond;		//samples per second
	uint16_t ADCSampleBufferSize;
	/*
	powermeter_sample_t powermeter_sample_period[2];
	uint8_t powermeter_sample_period_buf;
	powermeter_sample_t powermeter_sample_second;
	*/
	powermeter_channel_t powerdraw;

	uint8_t startCalculations;		//if 1 main should start calculating
	uint8_t samplebuffer_page;		//0 or 1 depending on current page
	samplebuffer_t samplebuffer[2];
} powermeter_t;


volatile powermeter_t powermeter;


//this is called every time a DMA transfers completes

ISR(DMA_CH0_vect)
{
	
	PORTC.OUTSET = LED_RED;		//enable RED Led on every complete transfer
	powermeter.startCalculations = 1;
	
	//flip page
	if(powermeter.samplebuffer_page)
		powermeter.samplebuffer_page=0;
	else
		powermeter.samplebuffer_page=1;
	
	DMA0_init(powermeter.samplebuffer[powermeter.samplebuffer_page].u,powermeter.ADCSampleBufferSize);		//set DMA transfer destination
	DMA1_init(powermeter.samplebuffer[powermeter.samplebuffer_page].i1,powermeter.ADCSampleBufferSize);		//set DMA transfer destination
	
}



int powermeter_SetSampleratePerPeriod(uint16_t samples)
{
	if( samples > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!samples)
		return -1;
	//configure Timer1 to generate 50 * samples events * 3	(3 channels, 50Hz)
	powermeter.ADCSamplesPerSecond = samples * 50 * 3;
	powermeter.ADCSampleBufferSize = samples * 3 * sizeof(uint16_t);
	return 0;
}

int powermeter_Start()
{
	if( powermeter.ADCSamplesPerSecond > POWERMETER_SAMPLEBUFF)
		return -1;
	if(!powermeter.ADCSamplesPerSecond)
		return -1;

	powermeter.samplebuffer_page = 0;
	powermeter.startCalculations = 0;	
	TC1_init(powermeter.ADCSamplesPerSecond);
	ADC_Disable(&ADCA);
	ADC_Disable(&ADCB);
	adc_init();
	
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_HI_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init(powermeter.samplebuffer[powermeter.samplebuffer_page].u,powermeter.ADCSampleBufferSize);	//set DMA transfer destination

	
	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_HI_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init(powermeter.samplebuffer[powermeter.samplebuffer_page].i1,powermeter.ADCSampleBufferSize);	//set DMA transfer destination
	
	return 0;
}


void powermeter_Stop()
{
	TC1_Reset(&TCC1);	//stop trigger for ADC
	DMA_DisableChannel(&DMA.CH0);	//stop DMA transfers
	DMA_ResetChannel(&DMA.CH0);		//stop DMA transfers
	ADC_Disable(&ADCA);			//disable ADC
	ADC_Disable(&ADCB);			//disable ADC
}

inline void powermeter_Calculate()
{
	if(!powermeter.startCalculations)
		return;
	powermeter.startCalculations = 0;
	
	uint16_t * up = &powermeter.samplebuffer[powermeter.samplebuffer_page].u;
	uint16_t * ip = &powermeter.samplebuffer[powermeter.samplebuffer_page].i1;
	char buf[9];
	for(i = 0;i < powermeter.ADCSamplesPerSecond * sizeof(uint16_t) * 3- (3 * sizeof(uint16_t));i+=(3*sizeof(uint16_t)))
	{
		
		powermeter.powerdraw.c1.Ueff += *((uint16_t*)(up + i));
		powermeter.powerdraw.c2.Ueff += *((uint16_t*)(up + i + sizeof(uint16_t)));
		powermeter.powerdraw.c3.Ueff += *((uint16_t*)(up + i + sizeof(uint16_t)*2));

		powermeter.powerdraw.c1.Ieff += *((uint16_t*)(ip + i));
		powermeter.powerdraw.c2.Ieff += *((uint16_t*)(ip + i + sizeof(uint16_t)));
		powermeter.powerdraw.c3.Ieff += *((uint16_t*)(ip + i + sizeof(uint16_t)*2));	

	}
}