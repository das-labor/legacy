/* -*- Mode: C; tab-width: 2 -*- */
#include <avr/io.h>

#include <avrx-io.h>
#include <avrx-signal.h>
//#include <util/delay.h>
#include "avrx.h"               // AvrX System calls/data structures
//#include "serialio.h"           // From AvrX...

#include "config.h"
#include "xcan.h"
#include "xlap.h"
#include "switch.h"
#include "led.h"
#include "PowerCommander.h"
#include "twi_master/twi_master.h"
#include "queue.h"

uint8_t direction_lounge=DIM_DOWN;
uint8_t state_lounge=F_SW_OFF;
uint8_t direction_vortrag=DIM_DOWN;
uint8_t state_vortrag=F_SW_OFF;

AVRX_SIGINT(SIG_OVERFLOW0)
{
	IntProlog();                // Save interrupted context, switch stacks
	TCNT0 = TCNT0_INIT;			// Reload the timer counter
	AvrXTimerHandler();         // Process Timer queue
	if( rb[RB_LOUNGE].clickstate[rb[RB_LOUNGE].clickpos] < TASTER_MIN ) {
		rb[RB_LOUNGE].clickstate_free[rb[RB_LOUNGE].clickpos]++;
	}
	if( rb[RB_VORTRAG].clickstate[rb[RB_LOUNGE].clickpos] < TASTER_MIN ) {
		rb[RB_VORTRAG].clickstate_free[rb[RB_LOUNGE].clickpos]++;
	}
	Epilog();                   // Restore context of next running task
};


AVRX_SIGINT(TASTER_VORTRAG)
{
	static t_i2cMessage_out i2c_outdata_tv;
	uint8_t i=0;
	cli();

	if( ( rb[RB_VORTRAG].clickstate_free[rb[RB_VORTRAG].clickpos] > TASTER_MIN ) &&
			( rb[RB_VORTRAG].clickstate[rb[RB_VORTRAG].clickpos] == 0) ) rb[RB_VORTRAG].clickpos++;

	if( rb[RB_VORTRAG].clickpos > RINGBUFFERSIZE) rb[RB_VORTRAG].clickpos=0;
	// der taster faengt bei 1 an, der free bei 0
	if( rb[RB_VORTRAG].clickpos != 0)	
		{
			rb[RB_VORTRAG].clickstate[rb[RB_VORTRAG].clickpos]++;

			if ( ( rb[RB_VORTRAG].clickstate_free[rb[RB_VORTRAG].clickpos - 1] > TASTER_MIN ) && 
					 ( rb[RB_VORTRAG].clickstate[rb[RB_VORTRAG].clickpos] < SHORT_TASTER ) && 
					 ( rb[RB_VORTRAG].clickstate_free[rb[RB_VORTRAG].clickpos] > TASTER_MIN ) )
				{
					if ( state_vortrag == F_SW_ON ){
						i2c_outdata_tv.outdata[0]=C_VIRT;
						i2c_outdata_tv.outdata[1]=VIRT_VORTRAG;
						i2c_outdata_tv.outdata[2]=F_SW_OFF;
						i2c_outdata_tv.outdata[3]=0x00;
						state_vortrag = F_SW_OFF;
					} else {
						i2c_outdata_tv.outdata[0]=C_VIRT;
						i2c_outdata_tv.outdata[1]=VIRT_VORTRAG;
						i2c_outdata_tv.outdata[2]=F_SW_ON;
						i2c_outdata_tv.outdata[3]=0x00;
						state_vortrag = F_SW_ON;
					}
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata_tv.mcb);
					for ( i=0;i< RINGBUFFERSIZE;i++) {
						rb[RB_VORTRAG].clickstate_free[i]=0;
						rb[RB_VORTRAG].clickstate[i]=0;
					}
					rb[RB_VORTRAG].clickpos=0;

					// process short click
				}
			if ( ( rb[RB_VORTRAG].clickstate_free[rb[RB_VORTRAG].clickpos - 1] > TASTER_MIN ) && 
					 ( rb[RB_VORTRAG].clickstate[rb[RB_VORTRAG].clickpos] >= SHORT_TASTER ) && 
					 ( rb[RB_VORTRAG].clickstate_free[rb[RB_VORTRAG].clickpos] > TASTER_MIN ) )
				{
					if ( state_vortrag == F_SW_ON ){
						i2c_outdata_tv.outdata[0]=C_VIRT;
						i2c_outdata_tv.outdata[1]=VIRT_VORTRAG;
						i2c_outdata_tv.outdata[2]=F_SW_OFF;
						i2c_outdata_tv.outdata[3]=0x00;
						state_vortrag = F_SW_OFF;
					} else {
						i2c_outdata_tv.outdata[0]=C_VIRT;
						i2c_outdata_tv.outdata[1]=VIRT_VORTRAG;
						i2c_outdata_tv.outdata[2]=F_SW_ON;
						i2c_outdata_tv.outdata[3]=0x00;
						state_vortrag = F_SW_ON;
					}
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata_tv.mcb);
					for ( i=0;i< RINGBUFFERSIZE;i++) {
						rb[RB_VORTRAG].clickstate_free[i]=0;
						rb[RB_VORTRAG].clickstate[i]=0;
					}
					rb[RB_VORTRAG].clickpos=0;

					// process long click
				}
		}
					 
	sei();
};

AVRX_SIGINT(TASTER_LOUNGE)
{
	static t_i2cMessage_out i2c_outdata_tl;
	uint8_t i=0;
	cli();
	if( ( rb[RB_LOUNGE].clickstate_free[rb[RB_LOUNGE].clickpos] > TASTER_MIN ) &&
			( rb[RB_LOUNGE].clickstate[rb[RB_LOUNGE].clickpos] == 0) ) rb[RB_LOUNGE].clickpos++;

	if( rb[RB_LOUNGE].clickpos > RINGBUFFERSIZE) rb[RB_LOUNGE].clickpos=0;
	// der taster faengt bei 1 an, der free bei 0
	if( rb[RB_LOUNGE].clickpos != 0)	
		{
			rb[RB_LOUNGE].clickstate[rb[RB_LOUNGE].clickpos]++;

			if ( ( rb[RB_LOUNGE].clickstate_free[rb[RB_LOUNGE].clickpos-1] > TASTER_MIN ) && 
					 ( rb[RB_LOUNGE].clickstate[rb[RB_LOUNGE].clickpos] < SHORT_TASTER ) && 
					 ( rb[RB_LOUNGE].clickstate_free[rb[RB_LOUNGE].clickpos] > TASTER_MIN ) )
				{
					// process short click
					if ( state_lounge == F_SW_ON ){
						i2c_outdata_tl.outdata[0]=C_SW;
						i2c_outdata_tl.outdata[1]=SWL_LOUNGE;
						i2c_outdata_tl.outdata[2]=F_SW_OFF;
						i2c_outdata_tl.outdata[3]=0x00;
						state_lounge = F_SW_OFF;
					} else {
						i2c_outdata_tl.outdata[0]=C_SW;
						i2c_outdata_tl.outdata[1]=SWL_LOUNGE;
						i2c_outdata_tl.outdata[2]=F_SW_ON;
						i2c_outdata_tl.outdata[3]=0x00;
						state_lounge = F_SW_ON;
					}

					AvrXSendMessage(&i2cQueue_out, &i2c_outdata_tl.mcb);
					for ( i=0;i< RINGBUFFERSIZE;i++) {
						rb[RB_LOUNGE].clickstate_free[i]=0;
						rb[RB_LOUNGE].clickstate[i]=0;
					}
					rb[RB_LOUNGE].clickpos=0;

				}
			if ( ( rb[RB_LOUNGE].clickstate_free[rb[RB_LOUNGE].clickpos-1] > TASTER_MIN ) && 
					 ( rb[RB_LOUNGE].clickstate[rb[RB_LOUNGE].clickpos] >= SHORT_TASTER ) && 
					 ( rb[RB_LOUNGE].clickstate_free[rb[RB_LOUNGE].clickpos] > TASTER_MIN ) )
				{
					// process long click
					if ( state_lounge == F_SW_ON ){
						i2c_outdata_tl.outdata[0]=C_SW;
						i2c_outdata_tl.outdata[1]=SWL_LOUNGE;
						i2c_outdata_tl.outdata[2]=F_SW_OFF;
						i2c_outdata_tl.outdata[3]=0x00;
						state_lounge = F_SW_OFF;
					} else {
						i2c_outdata_tl.outdata[0]=C_SW;
						i2c_outdata_tl.outdata[1]=SWL_LOUNGE;
						i2c_outdata_tl.outdata[2]=F_SW_ON;
						i2c_outdata_tl.outdata[3]=0x00;
						state_lounge = F_SW_ON;
					}
					AvrXSendMessage(&i2cQueue_out, &i2c_outdata_tl.mcb);
					for ( i=0;i< RINGBUFFERSIZE;i++) {
						rb[RB_LOUNGE].clickstate_free[i]=0;
						rb[RB_LOUNGE].clickstate[i]=0;
					}
					rb[RB_LOUNGE].clickpos=0;


				}
		}

	sei();
};

int main(void)
{
	AvrXSetKernelStack(0);

	MCUCR |= _BV(SE);      	    // Enable "sleep" mode (low power when idle)
	TCNT0 |= TCNT0_INIT;		    // Load overflow counter of timer0
	TCCR0 |= TMC8_CK256;		// Set Timer0 to CPUCLK/256
//	TCCR0B |= TMC8_CK256;		  // set timer0 atmega644p
	TIMSK |= _BV(TOIE0);		  // Enable interrupt flag
//	TIMSK0 |= _BV(TOIE0);		  // Enable interrupt flag atmega644p

	DDRA |= _BV(PA2) | _BV(PA3) | _BV(PA4); // Status LED G R B
	DDRA &= ~(_BV(PA0) | _BV(PA1)); // Eingänge HS, rcd

	DDRB &= ~_BV(PB2); // Eingang 
	DDRD &= ~_BV(PD3); // Eingang 
	PORTB |= _BV(PB2); // Pullup Taster vortrag
	PORTD |= _BV(PD3); // Pullup Taster lounge
/*
	MCUCR |= _BV(ISC11);							// Trigger Interrupt on any logical change on pin pd2
	GICR |= _BV(INT1) | _BV(INT2);		// Enable External Interrupt Request 1 / 2
	MCUCSR &= ~_BV(ISC2);
	
	TCCR1A |= _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);	// FastPWM, Set OC1X on Compare Match, clear OC1X at BOTTOM, (non inverting mode).
	TCCR1B |= _BV(WGM12) | _BV(CS11);	// FastPWM bit 2, clk/64
	
//	TIMSK |= _BV(TOIE0);							// Enable Timer0 Overflow Interrupt
*/

//	_delay_ms(1000);

/*
** Initiate TWI Master Interface with bitrate of 100000 Hz
*/
	if (!TWIM_Init(100000))
	{
		while (1);
	}
	//InitSerialIO(UBRR_INIT);    // Initialize USART baud rate generator
	xlap_init();

	AvrXRunTask(TCB(cancom_in));
	AvrXRunTask(TCB(cancom_out));
	AvrXRunTask(TCB(i2ccom_in));
	AvrXRunTask(TCB(i2ccom_out));

	//	AvrXRunTask(TCB(laptask));
	AvrXRunTask(TCB(switchtask));
	//	AvrXRunTask(TCB(switch_lounge));
	//	AvrXRunTask(TCB(switch_vortrag));
	AvrXRunTask(TCB(led));
	
	/* Needed for EEPROM access in monitor */
	//AvrXSetSemaphore(&EEPromMutex);

	Epilog();                   // Switch from AvrX Stack to first task
	while(1);
};
