/* powermeter 
 ADC samples are loaded to powermetersamplebuffer[0] or powermetersamplebuffer[1] using DMA
 after that they will be converted and added to powerdraw[IN_PROGRESS]
 on every second powerdraw[] will be updated:
	 powerdraw[IN_PROGRESS] -> 1s -> powerdraw[LAST_SECOND] -> 60s -> powerdraw[LAST_MINUTE] -> ... */

#include "avr_compiler.h"
#include "config.h"

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


#if POWERMETER_SAMPLEBUFF * 18 > 15500
	#error to less RAM, decrease POWERMETER_SAMPLEBUFF
#endif

typedef struct
{
	long Ueff;	
	long Ieff;
	long P;
	long S;
} powermeter_data_t;

typedef struct
{
	powermeter_data_t c1;
	powermeter_data_t c2;
	powermeter_data_t c3;
	
} powermeter_channel_t;

typedef struct
{
	int u[POWERMETER_SAMPLEBUFF * 3];
	int i1[POWERMETER_SAMPLEBUFF * 3];
	int i2[POWERMETER_SAMPLEBUFF * 3];
} samplebuffer_t;

typedef struct
{
	uint16_t ADCSamplesPerSecond;		//samples per second
	uint16_t ADCSampleBufferSize;
	uint16_t ADCSampesPerPeriod;

	powermeter_channel_t *powerdraw;
	powermeter_channel_t *powerdrawPerSecond;
	uint8_t clearpowerdrawPerSecond;	//0 if locked, 1 if unlocked
	uint8_t isrunning;	//0 or 1 if runnning
	uint8_t startCalculations;		//if 1 main should start calculating
	uint8_t samplebuffer_page;		//0 or 1 depending on current page
	samplebuffer_t samplebuffer[2];	//this are two pages, each having three channel-buffers, holding the RAW ADC data
} powermeter_t;


int powermeter_SetSampleratePerPeriod(uint16_t samples);

void powermeter_Init();

int powermeter_Start(void);

void powermeter_Stop(void);

void powermeter_docalculations();

int powermeter_createDATAPACKET(void* can_packet);

void powermeter_clearBuf();

void memsetv(void *p,uint8_t value, uint16_t size);
