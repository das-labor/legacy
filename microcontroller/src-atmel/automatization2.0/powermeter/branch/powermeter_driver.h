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


