#include "twi_master/twi_master.h"
#include "temp_regler.h"

#define VENTIL_STEP  0
#define VENTIL_RESET 1

uint8_t ventile[] = {42, 44, 46};

uint8_t temp_soll = 20;
uint8_t temp_ist = 20;

void set_ventil(uint8_t stufe)
{
	uint8_t i = 0;
	while (i < 2)
	{
		if (!TWIM_Start(ventile[i++] + TW_WRITE))
		{
			TWIM_Stop();
		}
		else
		{
			TWIM_Write(VENTIL_STEP);
			TWIM_Write(stufe);
			TWIM_Stop();
		}
	}
}

void set_reset()
{
	uint8_t i = 0;
	while (i < 2)
	{
		if (!TWIM_Start(ventile[i++] + TW_WRITE))
		{
			TWIM_Stop();
		}
		else
		{
			TWIM_Write(VENTIL_RESET);
			TWIM_Write(0);
			TWIM_Stop();
		}
	}
}

void temp_regler()
{
/*
	//labor_stat
	//außen_temp?
	//temp_ist - temp_soll
	//pi -regler achtung windup i mit max stellgröße begrenzen
	//12 bit genauigkeit float oder fixpoint
	int16_t e, esum, y;
	y = Kp * e + Ki * Ta * esum
	// e = w - x;
	e = temp_soll - temp_ist;				//Vergleich
	esum += e;			//Integration I-Anteil
	if (esum < -10) {
		esum = -10;
	}	        //Begrenzung I-Anteil
	if (esum > 10) {
		esum = 10;
	}
	y = Kp*e + Ki*Ta*esum;			//Reglergleichung
	if (y < 0) {y = 0;}			//Begrenzung Stellgröße
	if (y > 255) {y = 255;}
	
	uint8_t ventil;

*/
}

