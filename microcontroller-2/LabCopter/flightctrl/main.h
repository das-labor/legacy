/***************************************************************************
 *            main.h
 *
 *  Wed Jan  2 21:04:06 2008
 *  Copyright  2008  gast
 *  <gast@<host>>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
 #if defined (__AVR_ATmega644__)
#define SYSCLK	20000000L	//Quarz Frequenz in Hz
//#define SYSCLK	16000000L	//Quarz Frequenz in Hz
#endif

#ifndef EEMEM
#define EEMEM __attribute__ ((section (".eeprom")))
#endif

// neue Hardware
#define ROT_OFF   {if(PlatinenVersion == 10) PORTB &=~0x01; else  PORTB |= 0x01;}
#define ROT_ON    {if(PlatinenVersion == 10) PORTB |= 0x01; else  PORTB &=~0x01;}
#define ROT_FLASH PORTB ^= 0x01
#define GRN_OFF   PORTB &=~0x02 
#define GRN_ON    PORTB |= 0x02 
#define GRN_FLASH PORTB ^= 0x02

#define F_CPU SYSCLK
//#ifndef F_CPU
//#error ################## F_CPU nicht definiert oder ungültig #############
//#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define EEPROM_ADR_VALID            1
#define EEPROM_ADR_ACTIVE_SET       2
#define EEPROM_ADR_LAST_OFFSET      3

#define EEPROM_ADR_ACC_NICK         4
#define EEPROM_ADR_ACC_ROLL         6
#define EEPROM_ADR_ACC_Z            8

#define EEPROM_ADR_PARAM_BEGIN      100

#define CFG_HOEHENREGELUNG       0x01
#define CFG_HOEHEN_SCHALTER      0x02
#define CFG_HEADING_HOLD         0x04
#define CFG_KOMPASS_AKTIV        0x08
#define CFG_KOMPASS_FIX          0x10
#define CFG_GPS_AKTIV            0x20
#define CFG_ACHSENKOPPLUNG_AKTIV 0x40
#define CFG_DREHRATEN_BEGRENZER  0x80

#define CFG_LOOP_OBEN       0x01
#define CFG_LOOP_UNTEN      0x02
#define CFG_LOOP_LINKS      0x04
#define CFG_LOOP_RECHTS     0x08

//#define  SYSCLK  
//extern unsigned long SYSCLK;
extern volatile int i_Nick[20],i_Roll[20],DiffNick,DiffRoll;
extern volatile unsigned char SenderOkay;
extern unsigned char CosinusNickWinkel, CosinusRollWinkel;
extern unsigned char PlatinenVersion;

void ReadParameterSet (unsigned char number, unsigned char *buffer, unsigned char length);
void WriteParameterSet(unsigned char number, unsigned char *buffer, unsigned char length);
extern unsigned char GetActiveParamSetNumber(void);
extern unsigned char EEPromArray[];

#define DELAY_US(x)	((unsigned int)( (x) * 1e-6 * F_CPU ))

struct str_DebugOut
{
 unsigned char Digital[2];
 unsigned int Analog[32];    // Debugwerte
};

extern struct str_DebugOut    DebugOut;

struct str_Debug
{
 unsigned char Digital[2];
 unsigned char RemoteTasten;
 unsigned int Analog[4];
}; 
extern struct str_Debug       DebugIn;

struct str_VersionInfo
{
  unsigned char Hauptversion;
  unsigned char Nebenversion;
  unsigned char PCKompatibel;
  unsigned char Rserved[7]; 
};   
extern struct str_VersionInfo VersionInfo;

#define VERSION_HAUPTVERSION 0
#define VERSION_NEBENVERSION 67
#define VERSION_INDEX 6
#define VERSION_KOMPATIBEL 7
