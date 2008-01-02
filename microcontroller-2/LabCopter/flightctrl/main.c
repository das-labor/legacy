/*
	This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "uart.h"
#include "globals.h"
#include "init.h"

//BLOB
#include "main.h"
#include "_Settings.h"
#include "rc.h"
#include "fc.h"
#include "twimaster.h"
#include "timer0.h"

unsigned char EEPromArray[E2END+1] EEMEM;
unsigned char PlatinenVersion = 10;

struct str_VersionInfo VersionInfo;

// -- Parametersatz aus EEPROM lesen ---
// number [0..5]   
void ReadParameterSet(unsigned char number, unsigned char *buffer, unsigned char length)
{
   if (number > 5) number = 5;
   eeprom_read_block(buffer, &EEPromArray[EEPROM_ADR_PARAM_BEGIN + length * number], length);
}


// -- Parametersatz ins EEPROM schreiben ---
// number [0..5]   
void WriteParameterSet(unsigned char number, unsigned char *buffer, unsigned char length)
{
   if(number > 5) number = 5;	
   eeprom_write_block(buffer, &EEPromArray[EEPROM_ADR_PARAM_BEGIN + length * number], length);
   eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET], number);  				// diesen Parametersatz als aktuell merken
}

unsigned char GetActiveParamSetNumber(void)
{
 unsigned char set;
 set = eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET]);
 if(set > 5) 
  { 
   set = 2;   
   eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET], set);  				// diesen Parametersatz als aktuell merken
  } 
 return(set);
}

//blobby init function
void blob_init(void)
{
	unsigned int timer;
	
    DDRB  = 0x00; 
    PORTB = 0x00; 
    for(timer = 0; timer < 1000; timer++); // verzögern
    if(PINB & 0x01) PlatinenVersion = 11; else PlatinenVersion = 10;
    DDRC  = 0x81; // SCL
    PORTC = 0xff; // Pullup SDA
    DDRB  = 0x1B; // LEDs und Druckoffset
    PORTB = 0x01; // LED_Rot
    DDRD  = 0x3E; // Speaker & TXD & J3 J4 J5
    DDRD  |=0x80; // J7
    
	PORTD = 0xF7; // LED
 
    
    MCUSR &=~(1<<WDRF);
    WDTCSR |= (1<<WDCE)|(1<<WDE);
    WDTCSR = 0;

    beeptime = 2000;

	StickGier = 0; PPM_in[K_GAS] = 0;StickRoll = 0; StickNick = 0;

    ROT_OFF;
    
    Timer_Init();
    rc_sum_init();
	i2c_init();
}

//blobby main func
int blob_main (void)
{
	unsigned char i;
	
	sei();

    VersionInfo.Hauptversion = VERSION_HAUPTVERSION;
    VersionInfo.Nebenversion = VERSION_NEBENVERSION;
    VersionInfo.PCKompatibel = VERSION_KOMPATIBEL;
    
	//printf("\n\rFlightControl\n\rHardware:%d.%d\n\rSoftware:V%d.%d%c ",PlatinenVersion/10,PlatinenVersion%10, VERSION_HAUPTVERSION, VERSION_NEBENVERSION,VERSION_INDEX + 'a');
	//printf("\n\r==============================");
	
	GRN_ON;

#define EE_DATENREVISION 68 // wird angepasst, wenn sich die EEPROM-Daten geändert haben
    if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_VALID]) != EE_DATENREVISION) 
	{ 
		
	  //printf("\n\rInit. EEPROM: Generiere Default-Parameter...");
		
	  DefaultKonstanten1();
	  for (i=0;i<6;i++)  
      {
       if(i==2) DefaultKonstanten2(); // Kamera
       if(i==3) DefaultKonstanten3(); // Beginner
       if(i>3)  DefaultKonstanten2(); // Kamera
       WriteParameterSet(i, (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
      } 
	  eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET], 3); // default-Setting
	  eeprom_write_byte(&EEPromArray[EEPROM_ADR_VALID], EE_DATENREVISION);
	}

    /*if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK]) > 4) 
     {
       printf("\n\rACC nicht abgeglichen!");
     }*/
	
	ReadParameterSet(GetActiveParamSetNumber(), (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
	
    //printf("\n\rBenutze Parametersatz %d", GetActiveParamSetNumber());

    
	//keine hoehenregelung
	/*if(EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG)
	 { 
	   printf("\n\rAbgleich Luftdrucksensor..");
	   timer = SetDelay(1000);  
       SucheLuftruckOffset();
   	   while (!CheckDelay(timer));
       printf("OK\n\r");
	}*/
	
	SetNeutral();

	ROT_OFF;
    
    beeptime = 2000;
    DebugIn.Analog[1] = 1000;
    DebugIn.Digital[0] = 0x55;	

    
	/*printf("\n\rSteuerung: ");
	if (EE_Parameter.GlobalConfig & CFG_HEADING_HOLD) printf("HeadingHold");
	else printf("Neutral");
	
	printf("\n\n\r");*/
	
	//LCD??
    //LcdClear();
	
    I2CTimeout = 5000;
	while (1)
	{
        if (UpdateMotor)      // ReglerIntervall
            { 
//            SPI_TransmitByte();
			UpdateMotor=0; 
//PORTD |= 0x08;
            MotorRegler();  
//PORTD &= ~0x08;
            SendMotorData();
            ROT_OFF;
            if(PcZugriff) PcZugriff--; 
             else 
              {
               DubWiseKeys[0] = 0; 
               DubWiseKeys[1] = 0;
               ExternStickNick = 0;
               ExternStickRoll = 0;
               ExternStickGier = 0;
              }
            if(SenderOkay)  SenderOkay--;
            if(!I2CTimeout)
                {
                 I2CTimeout = 5;
                 i2c_reset();
                  if((BeepMuster == 0xffff) && MotorenEin) 
                   { 
                    beeptime = 10000;
                    BeepMuster = 0x0080;
                   } 
                }
            else         
                {
                 I2CTimeout--;
                 ROT_OFF; 
                }
            }
            if(SIO_DEBUG)
              {
               DatenUebertragung();
               BearbeiteRxDaten();
              }
              else BearbeiteRxDaten();
         if(CheckDelay(timer))
            { 
            if(UBat < EE_Parameter.UnterspannungsWarnung)
                {     
                  if(BeepMuster == 0xffff) 
                   { 
                    beeptime = 6000;
                    BeepMuster = 0x0300;
                   } 
                }
       //      SPI_StartTransmitPacket();
			 timer = SetDelay(100);  
            } 
    }
 return (1);
}


int main()
{
	//old init
	blob_init()
		
	/*** INITIALIZATION ***/
	//move reworked init funcs here; leave rest in blob_init
	uart_init();
	ADC_Init();

	//old main
	blob_main();
}
