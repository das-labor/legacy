/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief  Writing EEPROM and Flash in ATxmega256A3 / ATxmega256A3B - Rev B.
*
*      This file contains an example application that demonstrates Self-programming
*      for ATxmega256A3 / ATxmega256A3B - Rev B.
*
* \par Application note:
*      AVR1008: Writing EEPROM and Flash in ATxmega256A3 / ATxmega256A3B - Rev B
*
* \par Documentation
*      For comprehensive code documentation, supported compilers, compiler
*      settings and supported devices see readme.html
*
* \author
*      Atmel Corporation: http://www.atmel.com \n
*      Support email: avr@atmel.com
*
* $Revision: 2740 $
* $Date: 2009-09-02 10:43:01 +0200 (on, 02 sep 2009) $  \n
*
* Copyright (c) 2009, Atmel Corporation All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of ATMEL may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
* SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/* Define flash memory and page size of target MCU */
//#define APP_END 0x40000
//#define FLASH_PAGE_SIZE 512 //Bytes

#include "sp_driver.h"


/* Temporary storage used for NVM-workaround. */
uint8_t sleepCtr;
uint8_t statusStore;
uint8_t pmicStore;
uint8_t globalInt;
uint8_t spmintStore;

/* SPM wakeup interrupt */
ISR(NVM_SPM_vect)
{
	/* Disable the SPM interrupt */
	NVM.INTCTRL = (NVM.INTCTRL & ~NVM_SPMLVL_gm);
	/* Restore sleep settings */
	SLEEP.CTRL = sleepCtr;
	/* Restore PMIC status and control registers */
	PMIC.STATUS = statusStore;
	PMIC.CTRL = pmicStore;
	/* Restore SPM interruptsettings */
	NVM.INTCTRL = spmintStore;
	/* Restore global interrupt settings */
	SREG = globalInt;
	return;
}

/* Set interrupt vector location to boot section of flash */
void PMIC_SetVectorLocationToBoot( void )
{
	uint8_t temp = PMIC.CTRL | PMIC_IVSEL_bm;
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = temp;
}

/*Set interrupt vector location to application section of flash */
void PMIC_SetVectorLocationToApplication( void )
{
	uint8_t temp = PMIC.CTRL & ~PMIC_IVSEL_bm;
	CCP = CCP_IOREG_gc;
	PMIC.CTRL = temp;
}

/* Save register settings before entering sleep mode */
void Prepare_to_Sleep( void )
{
	sleepCtr = SLEEP.CTRL;
	/* Set sleep mode to IDLE */
	SLEEP.CTRL =  0x00; 
	/* Save the PMIC Status and control registers */
	statusStore = PMIC.STATUS;								
	pmicStore = PMIC.CTRL;		
	/* Enable only the highest level of interrupts */									
	PMIC.CTRL = (PMIC.CTRL & ~(PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm)) | PMIC_HILVLEN_bm;
	/* Save SREG for later use */
	globalInt = SREG;
	/* Enable global interrupts */
	sei();
	/* Save SPM interrupt settings for later */ 
	spmintStore = NVM.INTCTRL;
}

/* New function declarations used for the NVM-workaround */
void EraseApplicationPage(uint32_t address)
{
	/*Set the correct settings and store critical registers before NVM-workaround*/
	Prepare_to_Sleep();
	/*Assembly "function" to preform page erase*/
	SP_EraseApplicationPage(address);
}

void EraseWriteApplicationPage(uint32_t address)
{
	/*Set the correct settings and store critical registers before NVM-workaround*/
	Prepare_to_Sleep(); 
	/*Assembly "function" to preform page erase-write*/
	SP_EraseWriteApplicationPage(address);
}

void ClearFlashBuffer(void)
{
	/*Set the correct settings and store critical registers before NVM-workaround*/
	Prepare_to_Sleep();
	
	/*Assembly "function" to erase flash buffer*/
	SP_EraseFlashBuffer();
}

void LoadFlashWord(uint32_t address, uint16_t word)
{
	/*Set the correct settings and store critical registers before NVM-workaround*/
	Prepare_to_Sleep();   
	/*Assembly "function" to load flash buffer*/
   	SP_LoadFlashWord(address, word);
}



void init_workaround(){
	PMIC_SetVectorLocationToBoot();

}


#if 0
/*! \brief Example to show how to read and write to the flash.
*/
int main(void)
{
	/* Assume success until proven otherwise. */
	bool success = true;
	PMIC_SetVectorLocationToBoot();
	/* Fill the entire application section of the flash memory with bogus data */
	for (uint32_t address = 0; address < APP_END; address+=FLASH_PAGE_SIZE) {
	
	   	/* Load flash buffer with data */
		ClearFlashBuffer(); //Clear the flash buffer first to avoid data corruption
		for (uint16_t i = 0; i < FLASH_PAGE_SIZE; i += 2) {
			uint8_t lowByte = i;
			uint8_t highByte = i+1;
			
			LoadFlashWord((uint32_t)i, ((uint16_t) highByte << 8) | lowByte);
			SP_WaitForSPM();
		}
		
		/* Perform erase-page write. */ 
		EraseWriteApplicationPage(address);
	}
	
	/* Check if data is written correctly */
	for (uint32_t address = 0; address < APP_END; address++)
	{
	   if(SP_ReadByte(address) != (uint8_t)address) success = false;
	}
	
	/* Allow for breakpoint to check the value of "success". */
	if (success) {
		while(true){
			nop();
		}
	} else {
		while(true){
			nop();
		}
	}
}
#endif