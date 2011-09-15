/* powermeter 
 ADC samples are loaded to powermetersamplebuffer[0] or powermetersamplebuffer[1] using DMA
 after that they will be converted and added to powerdraw[IN_PROGRESS]
 on every second powerdraw[] will be updated:
	 powerdraw[IN_PROGRESS] -> 1s -> powerdraw[LAST_SECOND] -> 60s -> powerdraw[LAST_MINUTE] -> ... */

#include "avr_compiler.h"
#include "config.h"

typedef struct
{
	int32_t Ueff;	
	int32_t Ieff;
	int32_t P;
	int32_t S;
} powermeter_data_t;

typedef struct
{
	int8_t offsetA;
	int8_t offsetB;
} powermeter_adc_offset_t;

typedef struct
{
	powermeter_data_t c1;
	powermeter_data_t c2;
	powermeter_data_t c3;
	
} powermeter_channel_t;

typedef struct
{
	int16_t u[POWERMETER_SAMPLEBUFF * 3];
	int16_t i1[POWERMETER_SAMPLEBUFF * 3];
	int16_t i2[POWERMETER_SAMPLEBUFF * 3];
} samplebuffer_t;

typedef struct
{
	uint16_t ADCSamplesPerSecond;		//samples per second
	uint16_t ADCSampleBufferSize;		//buffersize in Bytes
	uint16_t ADCSampesPerPeriod;		//samples per period
	powermeter_adc_offset_t ADCoffset;	//ADC offset (+-2)
	powermeter_channel_t powerdraw;
	powermeter_channel_t powerdrawPerSecond;

	uint8_t isrunning;				//0 or 1 if runnning
	volatile uint8_t startCalculations;		//if equal 2 main should start calculating
	volatile uint8_t samplebuffer_page;		//0 or 1 depending on current page
	samplebuffer_t samplebuffer[2];	//this are two pages, each having three channel-buffers, holding the RAW ADC data
} powermeter_t;


int powermeter_SetSampleratePerPeriod(uint16_t samples);

int powermeter_Start(void);

void powermeter_Stop(void);

void powermeter_docalculations();

void powermeter_clearpowerdraw();

void powermeter_clearpowerdrawPerSecond();
#if 0
void memsetv(void *p,uint8_t value, uint16_t size);
#endif
extern powermeter_t powermeter;
