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

#include "can/can.h"
#include "can/lap.h"
#include "string.h"

#include "can_handler.h"
#include "config.h"



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
	#else
		DMA_DisableChannel(&DMA.CH0);
	#endif
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);       //enable transfer interrupt, disable err interrupt

	//set new page
	if(powermeter.samplebuffer_page){
		DMA0_init(&powermeter.samplebuffer[0].u[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
	}else{
		DMA0_init(&powermeter.samplebuffer[1].u[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
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
		DMA_ResetChannel(&DMA.CH1);
	#else
		 DMA_DisableChannel(&DMA.CH1);
	#endif

	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);       //enable transfer interrupt, disable err interrupt
	//set new page
	if(powermeter.samplebuffer_page){
		DMA1_init((void*)&powermeter.samplebuffer[0].i1[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
	}else{
		DMA1_init((void*)&powermeter.samplebuffer[1].i1[0],sizeof(uint16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
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
		powermeter.ADCSamplesPerPeriod = samples;
		powermeter.ADCSamplesPerSecond = samples * NET_FREQ;// * 3;
		powermeter.ADCSampleBufferSize = samples * 3 * sizeof(int16_t);
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
	ADC_init();

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
	DMA_Enable();
	DMA_SetPriority(DMA_PRIMODE_RR0123_gc); //dma mode round robin
	DMA_DisableChannel(&DMA.CH0);
	DMA_ResetChannel(&DMA.CH0);
	DMA_SetIntLevel(&DMA.CH0,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA0_init(&powermeter.samplebuffer[powermeter.samplebuffer_page].u[0],sizeof(int16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination
#if DEBUGMODE
	sendUSARTC1_putstr("DMA0 Init complete\n\r");
#endif

	DMA_DisableChannel(&DMA.CH1);
	DMA_ResetChannel(&DMA.CH1);
	DMA_SetIntLevel(&DMA.CH1,DMA_CH_TRNINTLVL_LO_gc,DMA_CH_ERRINTLVL_OFF_gc);	//enable transfer interrupt, disable err interrupt
	DMA1_init(&powermeter.samplebuffer[powermeter.samplebuffer_page].i1[0],sizeof(int16_t)*3,powermeter.ADCSamplesPerPeriod);	//set DMA transfer destination

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
#define DEBUGMODE_CAN 0
void powermeter_docalculations()
{
	//check if calculations has to be done
	if(powermeter.startCalculations > 1)
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
#if DEBUGMODE_CAN
		static can_message msg = {0, 0, 0x05, 0x05, 4, {}};
        	can_message * txmsg;
#endif
		for(uint16_t x = 0;x < powermeter.ADCSamplesPerPeriod;x++)
		{
#if DEBUGMODE_CAN
		msg.addr_src = myaddr;
       		msg.dlc = 3;
        	msg.data[0] = (uint8_t)(((*up)&0xff00)>>8) ;   //counter (0=start message)
        	msg.data[1] = (uint8_t)((*up)&0xff);  //TODO
        	msg.data[2] = 0;

        	txmsg = can_buffer_get();

        	memcpy(txmsg, &msg, sizeof(can_message));
        	can_transmit(txmsg);
		msg.addr_src = myaddr;
        	msg.dlc = 3;
        	msg.data[0] = (uint8_t)(((*ip)&0xff00)>>8) ;   //counter (0=start message)
        	msg.data[1] = (uint8_t)((*ip)&0xff);  //TODO
        	msg.data[2] = 1;

        	txmsg = can_buffer_get();

        	memcpy(txmsg, &msg, sizeof(can_message));
        	can_transmit(txmsg);

#endif
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
#if DEBUGMODE_CAN
        msg.addr_src = myaddr;
        msg.dlc = 3;
        msg.data[0] = (uint8_t)(((*up)&0xff00)>>8) ;   //counter (0=start message)
        msg.data[1] = (uint8_t)((*up)&0xff);  //TODO
        msg.data[2] = 0;

        txmsg = can_buffer_get();

        memcpy(txmsg, &msg, sizeof(can_message));
        can_transmit(txmsg);
        msg.addr_src = myaddr;
        msg.dlc = 3;
        msg.data[0] = (uint8_t)(((*ip)&0xff00)>>8) ;   //counter (0=start message)
        msg.data[1] = (uint8_t)((*ip)&0xff);  //TODO
        msg.data[2] = 1;

        txmsg = can_buffer_get();

        memcpy(txmsg, &msg, sizeof(can_message));
        can_transmit(txmsg);

#endif
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
#if DEBUGMODE_CAN
        msg.addr_src = myaddr;
        msg.dlc = 3;
        msg.data[0] = (uint8_t)(((*up)&0xff00)>>8) ;   //counter (0=start message)
        msg.data[1] = (uint8_t)((*up)&0xff);  //TODO
        msg.data[2] = 0;

        txmsg = can_buffer_get();

        memcpy(txmsg, &msg, sizeof(can_message));
        can_transmit(txmsg);
        msg.addr_src = myaddr;
        msg.dlc = 3;
        msg.data[0] = (uint8_t)(((*ip)&0xff00)>>8) ;   //counter (0=start message)
        msg.data[1] = (uint8_t)((*ip)&0xff);  //TODO
        msg.data[2] = 1;

        txmsg = can_buffer_get();

        memcpy(txmsg, &msg, sizeof(can_message));
        can_transmit(txmsg);

#endif
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
		//done with calculations, free the page as soon as possible
		//flip page
		if(powermeter.samplebuffer_page)
			powermeter.samplebuffer_page=0;
		else
			powermeter.samplebuffer_page=1;

		if(powermeter.startCalculations > 2)
			setERROR(ERROR_SR_TOHIGH);
	
		powermeter.startCalculations = 0;

#if !DEBUGMODE

		//disable RTC Interrupt, it modifies powermeter.powerdrawPerSecond
		RTC_SetIntLevels( RTC_OVFINTLVL_OFF_gc, RTC_COMPINTLVL_OFF_gc );

		//calculate S
		powermeter.powerdrawPerSecond.c1.S += sqrt(powermeter.powerdraw.c1.Ueff * powermeter.powerdraw.c1.Ieff )/ powermeter.ADCSamplesPerPeriod;
		powermeter.powerdrawPerSecond.c2.S += sqrt(powermeter.powerdraw.c2.Ueff * powermeter.powerdraw.c2.Ieff )/ powermeter.ADCSamplesPerPeriod;
		powermeter.powerdrawPerSecond.c3.S += sqrt(powermeter.powerdraw.c3.Ueff * powermeter.powerdraw.c3.Ieff )/ powermeter.ADCSamplesPerPeriod;
		//calculate P
		powermeter.powerdrawPerSecond.c1.P += (powermeter.powerdraw.c1.P / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c2.P += (powermeter.powerdraw.c2.P / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c3.P += (powermeter.powerdraw.c3.P / powermeter.ADCSamplesPerPeriod);
		//calculate Ueff
		powermeter.powerdrawPerSecond.c1.Ueff += (sqrt(powermeter.powerdraw.c1.Ueff / powermeter.ADCSamplesPerPeriod));
		powermeter.powerdrawPerSecond.c2.Ueff += (sqrt(powermeter.powerdraw.c2.Ueff / powermeter.ADCSamplesPerPeriod));
		powermeter.powerdrawPerSecond.c3.Ueff += (sqrt(powermeter.powerdraw.c3.Ueff / powermeter.ADCSamplesPerPeriod));
		//calculate Ieff
		powermeter.powerdrawPerSecond.c1.Ieff += sqrt(powermeter.powerdraw.c1.Ieff / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c2.Ieff += sqrt(powermeter.powerdraw.c2.Ieff / powermeter.ADCSamplesPerPeriod);
		powermeter.powerdrawPerSecond.c3.Ieff += sqrt(powermeter.powerdraw.c3.Ieff / powermeter.ADCSamplesPerPeriod);

		powermeter.samplesPerSecondDone++;

		//enable RTC Interrupt
		RTC_SetIntLevels( RTC_OVFINTLVL_LO_gc, RTC_COMPINTLVL_OFF_gc );
		
		//clear powermeter.powerdraw
		powermeter_clearpowerdraw();

#endif

#if DEBUGMODE
		sendUSARTC1_putstr("endCalculations\n\r");
#endif
	LED_on();	//return to normal LED color

	}

	
}

void powermeter_copypowerdraw()
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

	powermeter.samplesPerSecondDone = 0;
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

void powermeter_clearpowerdrawLastSecond()
{
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






void TC1_init(volatile uint32_t eventsPerSecond){

	if(eventsPerSecond > 0xF42400)		//greater than 16000000Hz ?
#if DEBUGMODE
		sendUSARTC1_putstr("TC1 aborted\n\r");
#else
		return;
#endif
	TC1_Reset(&TCC1);
#if DEBUGMODE
		sendUSARTC1_putstr("TCC1 =");
		char buf[9];
#endif
	 TC1_SetOverflowIntLevel(&TCC1,TC_OVFINTLVL_LO_gc);
	uint16_t TC1_TOP;
	if((F_CPU/(1*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 1) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);

#if DEBUGMODE
		utoa(TCC1.PER,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr(" DIV1\n\r");
#endif
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV1_gc );
		return;
	}

	if((F_CPU/(2*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 2) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
#if DEBUGMODE
		utoa(TCC1.PER,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr(" DIV2\n\r");
#endif
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV2_gc );
		return;
	}

	if((F_CPU/(4*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 4) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
#if DEBUGMODE
		utoa(TCC1.PER,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr(" DIV4\n\r");
#endif
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV4_gc );
		return;
	}

	if((F_CPU/(8*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 8) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
#if DEBUGMODE
		utoa(TCC1.PER,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr(" DIV8\n\r");
#endif
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV8_gc );
		return;
	}
	if((F_CPU/(64*eventsPerSecond))<0xFFFF){
		TC1_TOP = ((F_CPU / 64) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
#if DEBUGMODE
		utoa(TCC1.PER,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr(" DIV64\n\r");
#endif
		/* Start Timer/Counter. */
		TC1_ConfigClockSource( &TCC1, TC_CLKSEL_DIV64_gc );
		return;
	}
		if(F_CPU/(256*eventsPerSecond)<0xFFFF){
		TC1_TOP = ((F_CPU / 256) / eventsPerSecond);		//calculate nearest match
		/* Set period ( TOP value ). */
		TC_SetPeriod( &TCC1,TC1_TOP);
#if DEBUGMODE
		utoa(TCC1.PER,&buf[0],10);
		sendUSARTC1_putstr(&buf[0]);
		sendUSARTC1_putstr(" DIV256\n\r");
#endif
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

	time.second++;
	if(time.second == 60){
		time.second=0;
		time.minute++;
		if(time.minute == 60){
			time.minute=0;
			time.hour++;
			if(time.hour == 24){
				time.hour=0;
				time.day++;
				if(time.day == 365){
					time.day=0;
					time.year++;
					RTC_year_int();	
				}
				RTC_days_int();	
			}
			RTC_hours_int();	
		}
		RTC_minutes_int();	
	}
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



void DMA0_init(void * destAddr, uint8_t blockSize, uint8_t count )
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

void DMA1_init(void * destAddr, uint8_t blockSize, uint8_t count  )
{
	DMA_SetupBlock( &DMA.CH1,					//channel 1
                    	&ADCB.CH0RES,				//source-addr
                  	DMA_CH_SRCRELOAD_BLOCK_gc,	//srcDirection reload after each block
			DMA_CH_SRCDIR_INC_gc,		//srcDirection increment after each byte
                   	destAddr,						//set destAddr
                  	DMA_CH_DESTRELOAD_NONE_gc,		//reload destAddr after transaction
			DMA_CH_DESTDIR_INC_gc, 		//destDirection increment destination memory addr
                    	blockSize,							//blockSize in bytes >= burstlen
                    	DMA_CH_BURSTLEN_2BYTE_gc,	//burstMode 2byte per burst
                    	count,							//repeat count times
                    	true );						//repeat

   	DMA_SetTriggerSource(&DMA.CH1, DMA_CH_TRIGSRC_ADCB_CH4_gc);	//Trigger on ADCB_CH2
   	DMA_EnableSingleShot(&DMA.CH1);
	DMA_EnableChannel(&DMA.CH1);
}


void ADC_init(){
	/* Move stored calibration values to ADC A. */
	ADC_CalibrationValues_Load(&ADCA);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
  	ADC_ConvMode_and_Resolution_Config(&ADCA, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate. */
	ADC_Prescaler_Config(&ADCA, ADC_PRESCALER_DIV32_gc);	// 1 MSPs

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

        ADCA.CTRLA |= ADC_DMASEL_CH012_gc;

  	/*  ADC_Ch_Interrupts_Config(&ADCA.CH0,ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
		Interrupts aren't used atm
	*/
	
	/* Move stored calibration values to ADC B. */
	ADC_CalibrationValues_Load(&ADCB);

	/* Set up ADC A to have signed conversion mode and 12 bit resolution. */
  	ADC_ConvMode_and_Resolution_Config(&ADCB, ADC_ConvMode_Signed, ADC_RESOLUTION_12BIT_gc);

	/* Set sample rate. */
	ADC_Prescaler_Config(&ADCB, ADC_PRESCALER_DIV32_gc);	// 1 MSPs

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
	ADCB.CTRLA |= ADC_DMASEL_CH012_gc;
	/* Setup sweep of three virtual channels. */
	ADC_SweepChannels_Config(&ADCB, ADC_SWEEP_012_gc);

//	ADC_Ch_Interrupts_Config(&ADCA.CH2, ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);
// 	ADC_Ch_Interrupts_Config(&ADCB.CH2, ADC_CH_INTMODE_COMPLETE_gc, ADC_CH_INTLVL_LO_gc);


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


#if 0
ISR(ADCA_CH2_vect)
{
	powermeter.adcsamples++;
//	DMA_StartTransfer(&DMA.CH0);
}
#endif

ISR(TCC1_OVF_vect)
{
	powermeter.timercc1clks++;
}


