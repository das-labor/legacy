/* powermeter 
 ADC samples are loaded to powermetersamplebuffer[0] or powermetersamplebuffer[1] using DMA
 after that they will be converted and added to powerdraw[IN_PROGRESS]
 on every second powerdraw[] will be updated:
	 powerdraw[IN_PROGRESS] -> 1s -> powerdraw[LAST_SECOND] -> 60s -> powerdraw[LAST_MINUTE] -> ... */

#include "avr_compiler.h"
#include "config.h"

typedef struct
{
	int32_t  Ueff;	
	int32_t  Ieff;
	int32_t  P;
	int32_t  S;
	uint32_t E;
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
	int16_t u[ADCSAMPLESPERPERIOD * 3];
	int16_t i1[ADCSAMPLESPERPERIOD * 3];
	
#if 0	//TODO implement second current channel	
	int16_t i2[ADCSAMPLESPERPERIOD * 3];
#endif

} samplebuffer_t;

typedef struct
{
	uint8_t samplesPerSecondDone;
	powermeter_adc_offset_t ADCoffset;	//ADC offset (+-2)
	powermeter_channel_t powerdraw;
	powermeter_channel_t powerdrawPerSecond;
	powermeter_channel_t powerdrawLastSecond;
	uint8_t isrunning;				//0 or 1 if runnning
	samplebuffer_t samplebuffer;	//holding the RAW ADC data
	volatile uint32_t seconds_uptime;
} powermeter_t;


int powermeter_Start(void);

void powermeter_Stop(void);

void powermeter_docalculations();

int checkforcanupdate( void );

extern powermeter_t powermeter;

//helper functions
void ADC_init();
void TC1_init(volatile uint32_t eventsPerSecond);


