/*
Get ADC values
V0.1
02.02.2008
Hansi
*/

#include "adc.h"
#include "globals.h"

/**********
	ADC CHANNEL ASSIGNMENTS
CH0: Gier
CH1: Roll
CH2: Nick
CH4: Battery
CH3: Air Pressure
CH5: ACC_Z
CH6: ACC_Y
CH7: ACC_X
**********/

//ADC Interrupt Handler
//Saves all current sensor values to corresponding globals
SIGNAL(SIG_ADC)
{
	//statekeeping vars
    static uint8_t kanal, state;
	
	channel = 0;
	state = 0;

    switch(state++)
    {
		//Gier Measurement
        case 0:
            CurrentGier = ADC;
		
			//switch to channel 1 (ROLL)
            channel++;
            break;
		
		//Roll Measurement
        case 1:
            CurrentRoll = ADC;
		
			//switch to channel 2 (NICK)
            channel++;
            break;
		
		//Nick Measurement
        case 2:
            CurrentNick = ADC;
		
			//switch to channel 4 (Battery)
            kanal = 4;
            break;
		
		//Battery Measurement
        case 3:
            //UBat = (3 * UBat + ADC / 3) / 4;//(UBat + ((ADC * 39) / 256) + 19)  / 2;
		
			//switch to channel 5 (ACC_Z)
           	channel++;
            break;
		
		//ACC_Z Measurement
        case 4:
            CurrentACC_Z = ADC;
		
			//switch to channel 6 (ACC_Y)
            channel++;
            break;
		
		//ACC_Y Measurement
        case 5:
            CurrentACC_Y = ADC;
		
			//switch to channel 7 (ACC_X)
            channel++;
            break;
		
		//ACC_X Measurement
        case 6:
            Aktuell_ax = ADC - NeutralAccX;
            AdWertAccNick =  Aktuell_ax;

			//be aware of the missing break
			//at the end of the state machine (turn off)
            //break;
			
		//turn off int
        default:
			INT_ADC_OFF;
            channel = 0;
            state = 0;
            break;
    }
	
	//switch to next channel
    ADMUX = channel;
}
