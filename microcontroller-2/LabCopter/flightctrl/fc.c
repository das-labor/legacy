/*#######################################################################################
Flight Control
#######################################################################################*/
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Copyright (c) 04.2007 Holger Buss
// + Nur für den privaten Gebrauch
// + www.MikroKopter.com
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Es gilt für das gesamte Projekt (Hardware, Software, Binärfiles, Sourcecode und Dokumentation), 
// + dass eine Nutzung (auch auszugsweise) nur für den privaten (nicht-kommerziellen) Gebrauch zulässig ist. 
// + Sollten direkte oder indirekte kommerzielle Absichten verfolgt werden, ist mit uns (info@mikrokopter.de) Kontakt 
// + bzgl. der Nutzungsbedingungen aufzunehmen. 
// + Eine kommerzielle Nutzung ist z.B.Verkauf von MikroKoptern, Bestückung und Verkauf von Platinen oder Bausätzen,
// + Verkauf von Luftbildaufnahmen, usw.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Werden Teile des Quellcodes (mit oder ohne Modifikation) weiterverwendet oder veröffentlicht, 
// + unterliegen sie auch diesen Nutzungsbedingungen und diese Nutzungsbedingungen incl. Copyright müssen dann beiliegen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Sollte die Software (auch auszugesweise) oder sonstige Informationen des MikroKopter-Projekts
// + auf anderen Webseiten oder sonstigen Medien veröffentlicht werden, muss unsere Webseite "http://www.mikrokopter.de"
// + eindeutig als Ursprung verlinkt werden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Keine Gewähr auf Fehlerfreiheit, Vollständigkeit oder Funktion
// + Benutzung auf eigene Gefahr
// + Wir übernehmen keinerlei Haftung für direkte oder indirekte Personen- oder Sachschäden
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Portierung der Software (oder Teile davon) auf andere Systeme (ausser der Hardware von www.mikrokopter.de) ist nur 
// + mit unserer Zustimmung zulässig
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Die Funktion printf_P() unterliegt ihrer eigenen Lizenz und ist hiervon nicht betroffen
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Redistributions of source code (with or without modifications) must retain the above copyright notice, 
// + this list of conditions and the following disclaimer.
// +   * Neither the name of the copyright holders nor the names of contributors may be used to endorse or promote products derived
// +     from this software without specific prior written permission.
// +   * The use of this project (hardware, software, binary files, sources and documentation) is only permittet 
// +     for non-commercial use (directly or indirectly)
// +     Commercial use (for excample: selling of MikroKopters, selling of PCBs, assembly, ...) is only permitted 
// +     with our written permission
// +   * If sources or documentations are redistributet on other webpages, out webpage (http://www.MikroKopter.de) must be 
// +     clearly linked as origin 
// +   * porting to systems other than hardware from www.mikrokopter.de is not allowed
// +  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// +  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// +  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// +  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// +  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// +  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// +  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// +  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN// +  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// +  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// +  POSSIBILITY OF SUCH DAMAGE. 
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//for memcpy
#include <string.h>

#include "main.h"
#include "_Settings.h"
#include "fc.h"
#include "eeprom.c"
#include "timer0.h"
#include <avr/eeprom.h>
#include "globals.h"
#include "twimaster.h"
#include "adc.h"

unsigned char h,m,s;
volatile unsigned int I2CTimeout = 100;
volatile int MesswertNick,MesswertRoll,MesswertGier;
volatile int AdNeutralNick = 0,AdNeutralRoll = 0,AdNeutralGier = 0,StartNeutralRoll = 0,StartNeutralNick = 0;
volatile int Mittelwert_AccNick, Mittelwert_AccRoll,Mittelwert_AccHoch, NeutralAccX=0, NeutralAccY=0;
volatile float NeutralAccZ = 0;
unsigned char CosinusNickWinkel = 0, CosinusRollWinkel = 0;
volatile long IntegralNick = 0,IntegralNick2 = 0;
volatile long IntegralRoll = 0,IntegralRoll2 = 0;
volatile long IntegralAccNick = 0,IntegralAccRoll = 0,IntegralAccZ = 0;
volatile long Integral_Gier = 0;
volatile long Mess_IntegralNick = 0,Mess_IntegralNick2 = 0;
volatile long Mess_IntegralRoll = 0,Mess_IntegralRoll2 = 0;
volatile long Mess_Integral_Gier = 0,Mess_Integral_Gier2 = 0;
volatile long MittelIntegralNick,MittelIntegralRoll,MittelIntegralNick2,MittelIntegralRoll2;
volatile long Mess_Integral_Hoch = 0;
volatile int  KompassValue = 0;
volatile int  KompassStartwert = 0;
volatile int  KompassRichtung = 0;
unsigned char MAX_GAS,MIN_GAS;
unsigned char Notlandung = 0;
unsigned char HoehenReglerAktiv = 0;
long Umschlag180Nick = 250000L, Umschlag180Roll = 250000L;

float GyroFaktor;
float IntegralFaktor;
volatile int  DiffNick,DiffRoll;
int  Poti1 = 0, Poti2 = 0, Poti3 = 0, Poti4 = 0; 
volatile unsigned char Motor_Vorne,Motor_Hinten,Motor_Rechts,Motor_Links, Count;
unsigned char MotorWert[5];
volatile unsigned char SenderOkay = 0;
int StickNick = 0,StickRoll = 0,StickGier = 0;
char MotorenEin = 0;
int HoehenWert = 0;
int SollHoehe = 0;
int LageKorrekturRoll = 0,LageKorrekturNick = 0;
float Ki =  FAKTOR_I;
unsigned char Looping_Nick = 0,Looping_Roll = 0;
unsigned char Looping_Links = 0, Looping_Rechts = 0, Looping_Unten = 0, Looping_Oben = 0;

unsigned char Parameter_Luftdruck_D  = 48;      // Wert : 0-250
unsigned char Parameter_MaxHoehe     = 251;      // Wert : 0-250
unsigned char Parameter_Hoehe_P      = 16;      // Wert : 0-32
unsigned char Parameter_Hoehe_ACC_Wirkung = 58; // Wert : 0-250
unsigned char Parameter_KompassWirkung = 64;    // Wert : 0-250
unsigned char Parameter_Gyro_P = 150;           // Wert : 10-250
unsigned char Parameter_Gyro_I = 150;           // Wert : 0-250
unsigned char Parameter_Gier_P = 2;             // Wert : 1-20
unsigned char Parameter_I_Faktor = 10;          // Wert : 1-20
unsigned char Parameter_UserParam1 = 0;
unsigned char Parameter_UserParam2 = 0;
unsigned char Parameter_UserParam3 = 0;
unsigned char Parameter_UserParam4 = 0;
unsigned char Parameter_UserParam5 = 0;
unsigned char Parameter_UserParam6 = 0;
unsigned char Parameter_UserParam7 = 0;
unsigned char Parameter_UserParam8 = 0;
unsigned char Parameter_ServoNickControl = 100;
unsigned char Parameter_LoopGasLimit = 70;
unsigned char Parameter_AchsKopplung1 = 0;
unsigned char Parameter_AchsGegenKopplung1 = 0;
unsigned char Parameter_DynamicStability = 100;
struct mk_param_struct EE_Parameter;
signed int ExternStickNick = 0,ExternStickRoll = 0,ExternStickGier = 0, ExternHoehenValue = -20;

void Piep(unsigned char Anzahl)
{
 while(Anzahl--)
 {
  if(MotorenEin) return; //auf keinen Fall im Flug!
  beeptime = 100;
  Delay_ms(250);
 }
}

//############################################################################
//  Nullwerte ermitteln
void SetNeutral(void)
//############################################################################
{
	NeutralAccX = 0; 
	NeutralAccY = 0;
	NeutralAccZ = 0;
    AdNeutralNick = 0;	
	AdNeutralRoll = 0;	
	AdNeutralGier = 0;
    Parameter_AchsKopplung1 = 0;
    Parameter_AchsGegenKopplung1 = 0;
    CalibrierMittelwert();	
    Delay_ms_Mess(100);
	CalibrierMittelwert();
	/* keine hoehenregelung
    if((EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG))  // Höhenregelung aktiviert?
     {    
      if((MessLuftdruck > 950) || (MessLuftdruck < 750)) SucheLuftruckOffset();
     }*/

     AdNeutralNick= AdWertNick;	
	 AdNeutralRoll= AdWertRoll;	
	 AdNeutralGier= AdWertGier;
     StartNeutralRoll = AdNeutralRoll;
     StartNeutralNick = AdNeutralNick;
    if(eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK]) > 4) 
    {
      NeutralAccY = abs(Mittelwert_AccRoll) / ACC_AMPLIFY;
	  NeutralAccX = abs(Mittelwert_AccNick) / ACC_AMPLIFY;
	  NeutralAccZ = Aktuell_az;
    }
    else 
    {
      NeutralAccX = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_NICK+1]);
	  NeutralAccY = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL+1]);
	  NeutralAccZ = (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_Z]) * 256 + (int)eeprom_read_byte(&EEPromArray[EEPROM_ADR_ACC_Z+1]);
    }
    
	Mess_IntegralNick = 0;	
    Mess_IntegralNick2 = 0;
    Mess_IntegralRoll = 0;	
    Mess_IntegralRoll2 = 0;
    Mess_Integral_Gier = 0;	
    MesswertNick = 0;
    MesswertRoll = 0;
    MesswertGier = 0;
	/* keine hoehenregelung
    StartLuftdruck = Luftdruck;
	*/
    HoeheD = 0;
    Mess_Integral_Hoch = 0;
    KompassStartwert = KompassValue;
    GPS_Neutral();
    beeptime = 50;  
	Umschlag180Nick = (long) EE_Parameter.WinkelUmschlagNick * 2500L;
	Umschlag180Roll = (long) EE_Parameter.WinkelUmschlagRoll * 2500L;
    ExternHoehenValue = 0;
}

//############################################################################
// Bearbeitet die Messwerte
void Mittelwert(void)
//############################################################################
{ 	
    static signed long tmpl,tmpl2;	
    MesswertGier = (signed int) AdNeutralGier - AdWertGier;
    MesswertRoll = (signed int) AdWertRoll - AdNeutralRoll;
    MesswertNick = (signed int) AdWertNick - AdNeutralNick;

// Beschleunigungssensor  ++++++++++++++++++++++++++++++++++++++++++++++++
	Mittelwert_AccNick = ((long)Mittelwert_AccNick * 1 + ((ACC_AMPLIFY * (long)AdWertAccNick))) / 2L;
	Mittelwert_AccRoll = ((long)Mittelwert_AccRoll * 1 + ((ACC_AMPLIFY * (long)AdWertAccRoll))) / 2L;
	Mittelwert_AccHoch = ((long)Mittelwert_AccHoch * 1 + ((long)AdWertAccHoch)) / 2L;
    IntegralAccNick += ACC_AMPLIFY * AdWertAccNick;
    IntegralAccRoll += ACC_AMPLIFY * AdWertAccRoll;
    IntegralAccZ    += Aktuell_az - NeutralAccZ;
// Gier  ++++++++++++++++++++++++++++++++++++++++++++++++
            Mess_Integral_Gier +=  MesswertGier;
            Mess_Integral_Gier2 += MesswertGier;
// Kopplungsanteil  +++++++++++++++++++++++++++++++++++++
      if(!Looping_Nick && !Looping_Roll && (EE_Parameter.GlobalConfig & CFG_ACHSENKOPPLUNG_AKTIV))
         {
            tmpl = Mess_IntegralNick / 4096L;
            tmpl *= MesswertGier;
            tmpl *= Parameter_AchsKopplung1;  //125
            tmpl /= 2048L;
            tmpl2 = Mess_IntegralRoll / 4096L;
            tmpl2 *= MesswertGier;
            tmpl2 *= Parameter_AchsKopplung1;
            tmpl2 /= 2048L;
         }
      else  tmpl = tmpl2 = 0;
// Roll  ++++++++++++++++++++++++++++++++++++++++++++++++
            MesswertRoll += tmpl;
            MesswertRoll += (tmpl2*Parameter_AchsGegenKopplung1)/512L; //109
            Mess_IntegralRoll2 += MesswertRoll;
            Mess_IntegralRoll +=  MesswertRoll - LageKorrekturRoll;
            if(Mess_IntegralRoll > Umschlag180Roll) 
            {
             Mess_IntegralRoll  = -(Umschlag180Roll - 10000L);
             Mess_IntegralRoll2 = Mess_IntegralRoll;
            } 
            if(Mess_IntegralRoll <-Umschlag180Roll)
            {
             Mess_IntegralRoll =  (Umschlag180Roll - 10000L);
             Mess_IntegralRoll2 = Mess_IntegralRoll;
            }  
            if(AdWertRoll < 15)   MesswertRoll = -1000;
            if(AdWertRoll <  7)   MesswertRoll = -2000;
            if(PlatinenVersion == 10)
			 {
              if(AdWertRoll > 1010) MesswertRoll = +1000;
              if(AdWertRoll > 1017) MesswertRoll = +2000;
			 }
			 else 
			 {
              if(AdWertRoll > 2020) MesswertRoll = +1000;
              if(AdWertRoll > 2034) MesswertRoll = +2000;
			 }
// Nick  ++++++++++++++++++++++++++++++++++++++++++++++++
            MesswertNick -= tmpl2;
            MesswertNick -= (tmpl*Parameter_AchsGegenKopplung1)/512L;
            Mess_IntegralNick2 += MesswertNick;
            Mess_IntegralNick  += MesswertNick - LageKorrekturNick;
            if(Mess_IntegralNick > Umschlag180Nick) 
            {
             Mess_IntegralNick = -(Umschlag180Nick - 10000L);
             Mess_IntegralNick2 = Mess_IntegralNick;
            } 
            if(Mess_IntegralNick <-Umschlag180Nick) 
            {
             Mess_IntegralNick =  (Umschlag180Nick - 10000L);
             Mess_IntegralNick2 = Mess_IntegralNick;
            } 
            if(AdWertNick < 15)   MesswertNick = -1000;
            if(AdWertNick <  7)   MesswertNick = -2000;
            if(PlatinenVersion == 10)
			 {
              if(AdWertNick > 1010) MesswertNick = +1000;
              if(AdWertNick > 1017) MesswertNick = +2000;
			 }
			 else 
			 {
              if(AdWertNick > 2020) MesswertNick = +1000;
              if(AdWertNick > 2034) MesswertNick = +2000;
			 }
//++++++++++++++++++++++++++++++++++++++++++++++++
// ADC einschalten
    INT_ADC_ON;	
//++++++++++++++++++++++++++++++++++++++++++++++++

    Integral_Gier  = Mess_Integral_Gier;
    IntegralNick = Mess_IntegralNick;
    IntegralRoll = Mess_IntegralRoll;
    IntegralNick2 = Mess_IntegralNick2; 
    IntegralRoll2 = Mess_IntegralRoll2;

  if(EE_Parameter.GlobalConfig & CFG_DREHRATEN_BEGRENZER && !Looping_Nick && !Looping_Roll)
  {
    if(MesswertNick > 200)       MesswertNick += 4 * (MesswertNick - 200);
    else if(MesswertNick < -200) MesswertNick += 4 * (MesswertNick + 200); 
    if(MesswertRoll > 200)       MesswertRoll += 4 * (MesswertRoll - 200);
    else if(MesswertRoll < -200) MesswertRoll += 4 * (MesswertRoll + 200); 
  }
    if(Poti1 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI1]] + 110) Poti1++; else if(Poti1 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI1]] + 110 && Poti1) Poti1--;
    if(Poti2 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI2]] + 110) Poti2++; else if(Poti2 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI2]] + 110 && Poti2) Poti2--;
    if(Poti3 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI3]] + 110) Poti3++; else if(Poti3 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI3]] + 110 && Poti3) Poti3--;
    if(Poti4 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI4]] + 110) Poti4++; else if(Poti4 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI4]] + 110 && Poti4) Poti4--;
    if(Poti1 < 0) Poti1 = 0; else if(Poti1 > 255) Poti1 = 255;
    if(Poti2 < 0) Poti2 = 0; else if(Poti2 > 255) Poti2 = 255;
    if(Poti3 < 0) Poti3 = 0; else if(Poti3 > 255) Poti3 = 255;
    if(Poti4 < 0) Poti4 = 0; else if(Poti4 > 255) Poti4 = 255;
}

//############################################################################
// Messwerte beim Ermitteln der Nullage
void CalibrierMittelwert(void)
//############################################################################
{ 		  
    // ADC auschalten, damit die Werte sich nicht während der Berechnung ändern
	INT_ADC_OFF;
	MesswertNick = AdWertNick;
	MesswertRoll = AdWertRoll;
	MesswertGier = AdWertGier;
	Mittelwert_AccNick = ACC_AMPLIFY * (long)AdWertAccNick;
	Mittelwert_AccRoll = ACC_AMPLIFY * (long)AdWertAccRoll;
	Mittelwert_AccHoch = (long)AdWertAccHoch;
   // ADC einschalten
    INT_ADC_ON;	
    if(Poti1 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI1]] + 110) Poti1++; else if(Poti1 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI1]] + 110 && Poti1) Poti1--;
    if(Poti2 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI2]] + 110) Poti2++; else if(Poti2 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI2]] + 110 && Poti2) Poti2--;
    if(Poti3 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI3]] + 110) Poti3++; else if(Poti3 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI3]] + 110 && Poti3) Poti3--;
    if(Poti4 < PPM_in[EE_Parameter.Kanalbelegung[K_POTI4]] + 110) Poti4++; else if(Poti4 > PPM_in[EE_Parameter.Kanalbelegung[K_POTI4]] + 110 && Poti4) Poti4--;
    if(Poti1 < 0) Poti1 = 0; else if(Poti1 > 255) Poti1 = 255;
    if(Poti2 < 0) Poti2 = 0; else if(Poti2 > 255) Poti2 = 255;
    if(Poti3 < 0) Poti3 = 0; else if(Poti3 > 255) Poti3 = 255;
    if(Poti4 < 0) Poti4 = 0; else if(Poti4 > 255) Poti4 = 255;

	Umschlag180Nick = (long) EE_Parameter.WinkelUmschlagNick * 2500L;
	Umschlag180Roll = (long) EE_Parameter.WinkelUmschlagNick * 2500L;
}

//############################################################################
// Senden der Motorwerte per I2C-Bus
void SendMotorData(void)
//############################################################################
{ 
    if(MOTOR_OFF || !MotorenEin)
        {
        Motor_Hinten = 0;
        Motor_Vorne = 0;
        Motor_Rechts = 0;
        Motor_Links = 0;
        if(MotorTest[0]) Motor_Vorne = MotorTest[0];
        if(MotorTest[1]) Motor_Hinten = MotorTest[1];
        if(MotorTest[2]) Motor_Links = MotorTest[2];
        if(MotorTest[3]) Motor_Rechts = MotorTest[3];
        }

    DebugOut.Analog[12] = Motor_Vorne;
    DebugOut.Analog[13] = Motor_Hinten;
    DebugOut.Analog[14] = Motor_Links;
    DebugOut.Analog[15] = Motor_Rechts;   

    //Start I2C Interrupt Mode
    twi_state = 0;
    motor = 0;
    i2c_start(); 
}



//############################################################################
// Trägt ggf. das Poti als Parameter ein
void ParameterZuordnung(void) 
//############################################################################
{

 #define CHK_POTI(b,a,min,max) { if(a > 250) { if(a == 251) b = Poti1; else if(a == 252) b = Poti2; else if(a == 253) b = Poti3; else if(a == 254) b = Poti4;} else b = a; if(b <= min) b = min; else if(b >= max) b = max;}
 CHK_POTI(Parameter_MaxHoehe,EE_Parameter.MaxHoehe,0,255);
 CHK_POTI(Parameter_Luftdruck_D,EE_Parameter.Luftdruck_D,0,100);
 CHK_POTI(Parameter_Hoehe_P,EE_Parameter.Hoehe_P,0,100);
 CHK_POTI(Parameter_Hoehe_ACC_Wirkung,EE_Parameter.Hoehe_ACC_Wirkung,0,255);
 CHK_POTI(Parameter_KompassWirkung,EE_Parameter.KompassWirkung,0,255);
 CHK_POTI(Parameter_Gyro_P,EE_Parameter.Gyro_P,10,255);
 CHK_POTI(Parameter_Gyro_I,EE_Parameter.Gyro_I,0,255);
 CHK_POTI(Parameter_I_Faktor,EE_Parameter.I_Faktor,0,255); 
 CHK_POTI(Parameter_UserParam1,EE_Parameter.UserParam1,0,255); 
 CHK_POTI(Parameter_UserParam2,EE_Parameter.UserParam2,0,255); 
 CHK_POTI(Parameter_UserParam3,EE_Parameter.UserParam3,0,255); 
 CHK_POTI(Parameter_UserParam4,EE_Parameter.UserParam4,0,255); 
 CHK_POTI(Parameter_UserParam5,EE_Parameter.UserParam5,0,255); 
 CHK_POTI(Parameter_UserParam6,EE_Parameter.UserParam6,0,255); 
 CHK_POTI(Parameter_UserParam7,EE_Parameter.UserParam7,0,255); 
 CHK_POTI(Parameter_UserParam8,EE_Parameter.UserParam8,0,255); 
 CHK_POTI(Parameter_ServoNickControl,EE_Parameter.ServoNickControl,0,255);
 CHK_POTI(Parameter_LoopGasLimit,EE_Parameter.LoopGasLimit,0,255);
 CHK_POTI(Parameter_AchsKopplung1,    EE_Parameter.AchsKopplung1,0,255);
 CHK_POTI(Parameter_AchsGegenKopplung1,EE_Parameter.AchsGegenKopplung1,0,255);
 CHK_POTI(Parameter_DynamicStability,EE_Parameter.DynamicStability,0,255);

 Ki = (float) Parameter_I_Faktor * 0.0001;
 MAX_GAS = EE_Parameter.Gas_Max;
 MIN_GAS = EE_Parameter.Gas_Min;
}


//############################################################################
//
void MotorRegler(void)
//############################################################################
{
	 int motorwert,pd_ergebnis,h,tmp_int;
	 int GierMischanteil,GasMischanteil;
     static long SummeNick=0,SummeRoll=0;
     static long sollGier = 0,tmp_long,tmp_long2;
     static long IntegralFehlerNick = 0;
     static long IntegralFehlerRoll = 0;
  	 static unsigned int RcLostTimer;
  	 static unsigned char delay_neutral = 0;
  	 static unsigned char delay_einschalten = 0,delay_ausschalten = 0;
  	 static unsigned int  modell_fliegt = 0;
     static int hoehenregler = 0;
     static char TimerWerteausgabe = 0;
     static char NeueKompassRichtungMerken = 0;
     static long ausgleichNick, ausgleichRoll;
     
	Mittelwert(); 

    GRN_ON;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Gaswert ermitteln
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
   	GasMischanteil = PPM_in[EE_Parameter.Kanalbelegung[K_GAS]] + 120;
    if(GasMischanteil > MAX_GAS - 20) GasMischanteil = MAX_GAS - 20;
    if(GasMischanteil < 0) GasMischanteil = 0;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Emfang schlecht
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
   if(SenderOkay < 100)
        {
        if(!PcZugriff) 
         {
           if(BeepMuster == 0xffff) 
            { 
             beeptime = 15000;
             BeepMuster = 0x0c00;
            } 
         }
        if(RcLostTimer) RcLostTimer--; 
        else 
         {
          MotorenEin = 0;
          Notlandung = 0;
         } 
        ROT_ON;
        if(modell_fliegt > 2000)  // wahrscheinlich in der Luft --> langsam absenken
            {
            GasMischanteil = EE_Parameter.NotGas;
            Notlandung = 1;
            PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] = 0;
            PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] = 0;
            PPM_in[EE_Parameter.Kanalbelegung[K_GIER]] = 0;
            }
         else MotorenEin = 0;
        }
        else 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Emfang gut
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
        if(SenderOkay > 140)
            {
            Notlandung = 0;
            RcLostTimer = EE_Parameter.NotGasZeit * 50;
            if(GasMischanteil > 40)
                {
                if(modell_fliegt < 0xffff) modell_fliegt++;
                }
            if((modell_fliegt < 200) || (GasMischanteil < 40))
                {
                SummeNick = 0;
                SummeRoll = 0;
                Mess_Integral_Gier = 0;	
                Mess_Integral_Gier2 = 0;
                }
            if((GasMischanteil > 200) && MotorenEin == 0)
                {
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// auf Nullwerte kalibrieren
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
                if(PPM_in[EE_Parameter.Kanalbelegung[K_GIER]] > 75)  // Neutralwerte
                    {
                    if(++delay_neutral > 200)  // nicht sofort
                        {
                        GRN_OFF;
                        MotorenEin = 0;
                        delay_neutral = 0;
                        modell_fliegt = 0;
                        if(PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] > 70 || abs(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]]) > 70)
                        {
                         unsigned char setting=1;
                         if(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] > 70 && PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] < 70) setting = 1;
                         if(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] > 70 && PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] > 70) setting = 2;
                         if(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] < 70 && PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] > 70) setting = 3;
                         if(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] <-70 && PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] > 70) setting = 4;
                         if(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] <-70 && PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] < 70) setting = 5;
                         eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACTIVE_SET], setting);  // aktiven Datensatz merken
                        }
							
						/* keine hoehenregelung
                        if((EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG))  // Höhenregelung aktiviert?
                          {
                             if((MessLuftdruck > 950) || (MessLuftdruck < 750)) SucheLuftruckOffset();
                          }   
							*/
							
	                    ReadParameterSet(GetActiveParamSetNumber(), (unsigned char *) &EE_Parameter.Kanalbelegung[0], STRUCT_PARAM_LAENGE);
                        SetNeutral();
                        Piep(GetActiveParamSetNumber());
                        } 
                    }
                 else 
                if(PPM_in[EE_Parameter.Kanalbelegung[K_GIER]] < -75)  // ACC Neutralwerte speichern
                    {
                    if(++delay_neutral > 200)  // nicht sofort
                        {
                        GRN_OFF;
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_NICK],0xff); // Werte löschen
                        MotorenEin = 0;
                        delay_neutral = 0;
                        modell_fliegt = 0;
                        SetNeutral();
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_NICK],NeutralAccX / 256); // ACC-NeutralWerte speichern
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_NICK+1],NeutralAccX % 256); // ACC-NeutralWerte speichern
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL],NeutralAccY / 256);
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_ROLL+1],NeutralAccY % 256);
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_Z],(int)NeutralAccZ / 256);
                        eeprom_write_byte(&EEPromArray[EEPROM_ADR_ACC_Z+1],(int)NeutralAccZ % 256);
                        Piep(GetActiveParamSetNumber());
                        } 
                    }
                 else delay_neutral = 0;
                }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Gas ist unten
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
            if(GasMischanteil < 35)
                {
                // Starten
                if(PPM_in[EE_Parameter.Kanalbelegung[K_GIER]] < -75)
                    {
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Einschalten
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
                    if(++delay_einschalten > 200)
                        {
                        delay_einschalten = 200;
                        modell_fliegt = 1;
                        MotorenEin = 1;
                        sollGier = 0;
                        Mess_Integral_Gier = 0;	
                        Mess_Integral_Gier2 = 0;
                        Mess_IntegralNick = 0;
                        Mess_IntegralRoll = 0;
                        Mess_IntegralNick2 = IntegralNick;
                        Mess_IntegralRoll2 = IntegralRoll;
                        SummeNick = 0;
                        SummeRoll = 0;
                        }          
                    }  
                    else delay_einschalten = 0;
                //Auf Neutralwerte setzen
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Auschalten
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
                if(PPM_in[EE_Parameter.Kanalbelegung[K_GIER]] > 75)
                    {
                    if(++delay_ausschalten > 200)  // nicht sofort
                        {
                        MotorenEin = 0;
                        delay_ausschalten = 200;
                        modell_fliegt = 0;
                        } 
                    }
                else delay_ausschalten = 0;
                }
            }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// neue Werte von der Funke
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
 if(!NewPpmData-- || Notlandung)  
  {
    int tmp_int;
    ParameterZuordnung();
    StickNick = PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] * EE_Parameter.Stick_P; 
    StickNick += PPM_diff[EE_Parameter.Kanalbelegung[K_NICK]] * EE_Parameter.Stick_D;
    StickRoll = PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] * EE_Parameter.Stick_P;
    StickRoll += PPM_diff[EE_Parameter.Kanalbelegung[K_ROLL]] * EE_Parameter.Stick_D;
    StickGier = -PPM_in[EE_Parameter.Kanalbelegung[K_GIER]];
    GyroFaktor     = ((float)Parameter_Gyro_P + 10.0) / 256.0;
    IntegralFaktor = ((float) Parameter_Gyro_I) / 44000;

#define KEY_VALUE (Parameter_UserParam1 * 4)  //(Poti3 * 8)
if(DubWiseKeys[1]) beeptime = 10;
    if(DubWiseKeys[1] & DUB_KEY_UP)    tmp_int = KEY_VALUE;   else
    if(DubWiseKeys[1] & DUB_KEY_DOWN)  tmp_int = -KEY_VALUE;  else   tmp_int = 0;
    ExternStickNick = (ExternStickNick * 7 + tmp_int) / 8;
    if(DubWiseKeys[1] & DUB_KEY_LEFT)  tmp_int = KEY_VALUE; else
    if(DubWiseKeys[1] & DUB_KEY_RIGHT) tmp_int = -KEY_VALUE; else tmp_int = 0;
    ExternStickRoll = (ExternStickRoll * 7 + tmp_int) / 8;

    if(DubWiseKeys[0] & 8)  ExternStickGier = 50;else
    if(DubWiseKeys[0] & 4)  ExternStickGier =-50;else ExternStickGier = 0;
    if(DubWiseKeys[0] & 2)  ExternHoehenValue++;
    if(DubWiseKeys[0] & 16) ExternHoehenValue--;

    StickNick += ExternStickNick / 8;
    StickRoll += ExternStickRoll / 8;
    StickGier += ExternStickGier;

    if(EE_Parameter.GlobalConfig & CFG_HEADING_HOLD) IntegralFaktor =  0;
    if(GyroFaktor < 0) GyroFaktor = 0;
    if(IntegralFaktor < 0) IntegralFaktor = 0;
    // greift in den Stick ein, um ungewolltes überschlagen zu verhindern
    if(!(EE_Parameter.LoopConfig & CFG_LOOP_LINKS) && !(EE_Parameter.LoopConfig & CFG_LOOP_RECHTS))
     {
      if(IntegralNick >  60000)  
      { 
       StickNick -=  8 * EE_Parameter.Stick_P;      
       if(IntegralNick >  80000) StickNick -= 16 * EE_Parameter.Stick_P;      
      } 
      else
      if(IntegralNick < -60000)  
      {
       StickNick += 8 * EE_Parameter.Stick_P;
       if(IntegralNick >  80000) StickNick -= 16 * EE_Parameter.Stick_P;      
      } 
      if(IntegralRoll >  60000)  
      { 
       StickRoll -=  8 * EE_Parameter.Stick_P;      
       if(IntegralRoll >  80000) StickRoll -= 16 * EE_Parameter.Stick_P;      
      } 
      else
      if(IntegralRoll < -60000)  
      {
       StickRoll += 8 * EE_Parameter.Stick_P;
       if(IntegralRoll >  80000) StickRoll -= 16 * EE_Parameter.Stick_P;      
      } 
     }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Looping?
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
  if((PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] > EE_Parameter.LoopThreshold) && EE_Parameter.LoopConfig & CFG_LOOP_LINKS)  Looping_Links = 1;
  else 
   { 
     {
      if((PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] < (EE_Parameter.LoopThreshold - EE_Parameter.LoopHysterese))) Looping_Links = 0;  
     }  
   } 
  if((PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] < -EE_Parameter.LoopThreshold) && EE_Parameter.LoopConfig & CFG_LOOP_RECHTS) Looping_Rechts = 1;
   else 
   {
   if(Looping_Rechts) // Hysterese
     {
      if(PPM_in[EE_Parameter.Kanalbelegung[K_ROLL]] > -(EE_Parameter.LoopThreshold - EE_Parameter.LoopHysterese)) Looping_Rechts = 0;
     }
   } 

  if((PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] > EE_Parameter.LoopThreshold) && EE_Parameter.LoopConfig & CFG_LOOP_OBEN) Looping_Oben = 1;
  else 
   { 
    if(Looping_Oben)  // Hysterese
     {
      if((PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] < (EE_Parameter.LoopThreshold - EE_Parameter.LoopHysterese))) Looping_Oben = 0;  
     }  
   } 
  if((PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] < -EE_Parameter.LoopThreshold) && EE_Parameter.LoopConfig & CFG_LOOP_UNTEN) Looping_Unten = 1;
   else 
   {
    if(Looping_Unten) // Hysterese
     {
      if(PPM_in[EE_Parameter.Kanalbelegung[K_NICK]] > -(EE_Parameter.LoopThreshold - EE_Parameter.LoopHysterese)) Looping_Unten = 0;
     }
   } 

   if(Looping_Links || Looping_Rechts)   Looping_Roll = 1; else Looping_Roll = 0;
   if(Looping_Oben  || Looping_Unten) {Looping_Nick = 1; Looping_Roll = 0; Looping_Links = 0; Looping_Rechts = 0;} else Looping_Nick = 0;
  } // Ende neue Funken-Werte

  if(Looping_Roll) beeptime = 100;
  if(Looping_Roll || Looping_Nick)
   {
    if(GasMischanteil > EE_Parameter.LoopGasLimit) GasMischanteil = EE_Parameter.LoopGasLimit;
   }
    

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Bei Empfangsausfall im Flug 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
   if(Notlandung)
    {
     StickGier = 0;
     StickNick = 0;
     StickRoll = 0;
     GyroFaktor  = 0.1;
     IntegralFaktor = 0.005;
     Looping_Roll = 0;
     Looping_Nick = 0;
    }  


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Integrale auf ACC-Signal abgleichen
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
#define ABGLEICH_ANZAHL 256L

 MittelIntegralNick  += IntegralNick;    // Für die Mittelwertbildung aufsummieren
 MittelIntegralRoll  += IntegralRoll;
 MittelIntegralNick2 += IntegralNick2;
 MittelIntegralRoll2 += IntegralRoll2;

 if(Looping_Nick || Looping_Roll)
  {
    IntegralAccNick = 0;
    IntegralAccRoll = 0;
    MittelIntegralNick = 0;
    MittelIntegralRoll = 0;
    MittelIntegralNick2 = 0;
    MittelIntegralRoll2 = 0;
    Mess_IntegralNick2 = Mess_IntegralNick;
    Mess_IntegralRoll2 = Mess_IntegralRoll;
    ZaehlMessungen = 0;
    LageKorrekturNick = 0;
    LageKorrekturRoll = 0;
  }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
  if(!Looping_Nick && !Looping_Roll)
  {
   long tmp_long, tmp_long2;
    tmp_long = (long)(IntegralNick / EE_Parameter.GyroAccFaktor - (long)Mittelwert_AccNick);
    tmp_long /= 16;
    tmp_long2 = (long)(IntegralRoll / EE_Parameter.GyroAccFaktor - (long)Mittelwert_AccRoll); 
    tmp_long2 /= 16;
 #define AUSGLEICH 32 //(Parameter_UserParam1 / 2)
    if(tmp_long >  AUSGLEICH)  tmp_long  = AUSGLEICH;
    if(tmp_long < -AUSGLEICH)  tmp_long  =-AUSGLEICH;
    if(tmp_long2 > AUSGLEICH)  tmp_long2 = AUSGLEICH;
    if(tmp_long2 <-AUSGLEICH)  tmp_long2 =-AUSGLEICH;
    Mess_IntegralNick -= tmp_long;
    Mess_IntegralRoll -= tmp_long2;
  }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		

 if(ZaehlMessungen >= ABGLEICH_ANZAHL)
 {
  static int cnt = 0;
  static char last_n_p,last_n_n,last_r_p,last_r_n;
  static long MittelIntegralNick_Alt,MittelIntegralRoll_Alt;
  if(!Looping_Nick && !Looping_Roll)
  {
    MittelIntegralNick  /= ABGLEICH_ANZAHL;
    MittelIntegralRoll  /= ABGLEICH_ANZAHL;
	IntegralAccNick = (EE_Parameter.GyroAccFaktor * IntegralAccNick) / ABGLEICH_ANZAHL;
	IntegralAccRoll = (EE_Parameter.GyroAccFaktor * IntegralAccRoll) / ABGLEICH_ANZAHL;
    IntegralAccZ    = IntegralAccZ / ABGLEICH_ANZAHL;
#define MAX_I 0//(Poti2/10)
// Nick ++++++++++++++++++++++++++++++++++++++++++++++++
    IntegralFehlerNick = (long)(MittelIntegralNick - (long)IntegralAccNick);
    ausgleichNick = IntegralFehlerNick / EE_Parameter.GyroAccAbgleich;
    LageKorrekturNick = ausgleichNick / ABGLEICH_ANZAHL;
// Roll ++++++++++++++++++++++++++++++++++++++++++++++++     
    IntegralFehlerRoll = (long)(MittelIntegralRoll - (long)IntegralAccRoll); 
    ausgleichRoll = IntegralFehlerRoll / EE_Parameter.GyroAccAbgleich;
    LageKorrekturRoll = ausgleichRoll / ABGLEICH_ANZAHL;

//    Mess_IntegralNick -= ausgleichNick;
//    Mess_IntegralRoll -= ausgleichRoll;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Gyro-Drift ermitteln
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
    MittelIntegralNick2 /= ABGLEICH_ANZAHL;
    MittelIntegralRoll2 /= ABGLEICH_ANZAHL;
//    tmp_long  = (long)(MittelIntegralNick2 - (long)IntegralAccNick); 
//    tmp_long2 = (long)(MittelIntegralRoll2 - (long)IntegralAccRoll); 
    tmp_long  = IntegralNick2 - IntegralNick; 
    tmp_long2 = IntegralRoll2 - IntegralRoll; 
    //DebugOut.Analog[25] = MittelIntegralRoll2 / 26;

    IntegralFehlerNick = tmp_long;
    IntegralFehlerRoll = tmp_long2;
    Mess_IntegralNick2 -= IntegralFehlerNick;
    Mess_IntegralRoll2 -= IntegralFehlerRoll;

//    IntegralFehlerNick = (IntegralFehlerNick * 1 + tmp_long) / 2;
//    IntegralFehlerRoll = (IntegralFehlerRoll * 1 + tmp_long2) / 2;


DebugOut.Analog[17] = IntegralAccNick / 26;
DebugOut.Analog[18] = IntegralAccRoll / 26;
DebugOut.Analog[19] = IntegralFehlerNick;// / 26;
DebugOut.Analog[20] = IntegralFehlerRoll;// / 26;
DebugOut.Analog[21] = MittelIntegralNick / 26;
DebugOut.Analog[22] = MittelIntegralRoll / 26;
//DebugOut.Analog[28] = ausgleichNick;
DebugOut.Analog[29] = ausgleichRoll;
DebugOut.Analog[30] = LageKorrekturRoll * 10;

#define FEHLER_LIMIT  (ABGLEICH_ANZAHL * 4)
#define FEHLER_LIMIT2 (ABGLEICH_ANZAHL * 16)
#define BEWEGUNGS_LIMIT 20000
// Nick +++++++++++++++++++++++++++++++++++++++++++++++++
        cnt = 1;// + labs(IntegralFehlerNick) / 4096;
        if(labs(MittelIntegralNick_Alt - MittelIntegralNick) < BEWEGUNGS_LIMIT)
        {
        if(IntegralFehlerNick >  FEHLER_LIMIT2) 
         {
           if(last_n_p) 
           {
            cnt += labs(IntegralFehlerNick) / FEHLER_LIMIT2; 
            ausgleichNick = IntegralFehlerNick / 8;
            if(ausgleichNick > 5000) ausgleichNick = 5000;
            LageKorrekturNick += ausgleichNick / ABGLEICH_ANZAHL;
           } 
           else last_n_p = 1;
         } else  last_n_p = 0;
        if(IntegralFehlerNick < -FEHLER_LIMIT2) 
         {
           if(last_n_n)
            { 
             cnt += labs(IntegralFehlerNick) / FEHLER_LIMIT2; 
             ausgleichNick = IntegralFehlerNick / 8;
             if(ausgleichNick < -5000) ausgleichNick = -5000;
             LageKorrekturNick += ausgleichNick / ABGLEICH_ANZAHL;
            } 
           else last_n_n = 1;
         } else  last_n_n = 0;
        } else cnt = 0;
        if(cnt > EE_Parameter.Driftkomp) cnt = EE_Parameter.Driftkomp;
        if(IntegralFehlerNick >  FEHLER_LIMIT)   AdNeutralNick += cnt;
        if(IntegralFehlerNick < -FEHLER_LIMIT)   AdNeutralNick -= cnt;

// Roll +++++++++++++++++++++++++++++++++++++++++++++++++
        cnt = 1;// + labs(IntegralFehlerNick) / 4096;

        ausgleichRoll = 0;
        if(labs(MittelIntegralRoll_Alt - MittelIntegralRoll) < BEWEGUNGS_LIMIT)
        {
        if(IntegralFehlerRoll >  FEHLER_LIMIT2) 
         {
           if(last_r_p) 
           {
            cnt += labs(IntegralFehlerRoll) / FEHLER_LIMIT2; 
            ausgleichRoll = IntegralFehlerRoll / 8;
            if(ausgleichRoll > 5000) ausgleichRoll = 5000;
            LageKorrekturRoll += ausgleichRoll / ABGLEICH_ANZAHL;
           } 
           else last_r_p = 1;
         } else  last_r_p = 0;
        if(IntegralFehlerRoll < -FEHLER_LIMIT2) 
         {
           if(last_r_n) 
           {
            cnt += labs(IntegralFehlerRoll) / FEHLER_LIMIT2; 
            ausgleichRoll = IntegralFehlerRoll / 8;
            if(ausgleichRoll < -5000) ausgleichRoll = -5000;
            LageKorrekturRoll += ausgleichRoll / ABGLEICH_ANZAHL;
           }
           else last_r_n = 1;
         } else  last_r_n = 0;
        } else 
        {
         cnt = 0;
        } 
DebugOut.Analog[27] = ausgleichRoll;
        if(cnt > EE_Parameter.Driftkomp) cnt = EE_Parameter.Driftkomp;
//if(cnt > 1) beeptime = 50;
        if(IntegralFehlerRoll >  FEHLER_LIMIT)   AdNeutralRoll += cnt;
        if(IntegralFehlerRoll < -FEHLER_LIMIT)   AdNeutralRoll -= cnt;
DebugOut.Analog[23] = AdNeutralNick;//10*(AdNeutralNick - StartNeutralNick);
DebugOut.Analog[24] = 10*(AdNeutralRoll - StartNeutralRoll);
  }
  else 
  {
   LageKorrekturRoll = 0;
   LageKorrekturNick = 0;
  }
  if(!IntegralFaktor) { LageKorrekturRoll = 0; LageKorrekturNick = 0;} // z.B. bei HH
// +++++++++++++++++++++++++++++++++++++++++++++++++++++     
   MittelIntegralNick_Alt = MittelIntegralNick;      
   MittelIntegralRoll_Alt = MittelIntegralRoll;      
// +++++++++++++++++++++++++++++++++++++++++++++++++++++     
    IntegralAccNick = 0;
    IntegralAccRoll = 0;
    IntegralAccZ = 0;
    MittelIntegralNick = 0;
    MittelIntegralRoll = 0;
    MittelIntegralNick2 = 0;
    MittelIntegralRoll2 = 0;
    ZaehlMessungen = 0;
 }
//DebugOut.Analog[31] = StickRoll / (26*IntegralFaktor);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//  Gieren
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
    if(abs(StickGier) > 20) // war 35 
     {
      if(!(EE_Parameter.GlobalConfig & CFG_KOMPASS_FIX)) NeueKompassRichtungMerken = 1;
     }
    tmp_int  = (long) EE_Parameter.Gier_P * ((long)StickGier * abs(StickGier)) / 512L; // expo  y = ax + bx²
    tmp_int += (EE_Parameter.Gier_P * StickGier) / 4; 
    sollGier = tmp_int;
    Mess_Integral_Gier -= tmp_int;   
    if(Mess_Integral_Gier > 50000) Mess_Integral_Gier = 50000;  // begrenzen
    if(Mess_Integral_Gier <-50000) Mess_Integral_Gier =-50000;
  
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//  Kompass
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
    if(KompassValue && (EE_Parameter.GlobalConfig & CFG_KOMPASS_AKTIV)) 
     {
       int w,v;
       static int SignalSchlecht = 0; 
       w = abs(IntegralNick /512); // mit zunehmender Neigung den Einfluss drosseln
       v = abs(IntegralRoll /512);
       if(v > w) w = v; // grösste Neigung ermitteln
       if(w < 25 && NeueKompassRichtungMerken && !SignalSchlecht)     
        { 
         KompassStartwert = KompassValue;
         NeueKompassRichtungMerken = 0;
        }
       w = (w * Parameter_KompassWirkung) / 64;           // auf die Wirkung normieren
       w = Parameter_KompassWirkung - w;                  // Wirkung ggf drosseln
       if(w > 0)
        {
          if(!SignalSchlecht) Mess_Integral_Gier += (KompassRichtung * w) / 32;  // nach Kompass ausrichten
          if(SignalSchlecht) SignalSchlecht--;
        }  
        else SignalSchlecht = 500; // so lange das Signal taub stellen --> ca. 1 sek
     } 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//  Debugwerte zuordnen
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
  if(!TimerWerteausgabe--)
   {
    TimerWerteausgabe = 24;
    DebugOut.Analog[0] = IntegralNick / EE_Parameter.GyroAccFaktor;
    DebugOut.Analog[1] = IntegralRoll / EE_Parameter.GyroAccFaktor;
    DebugOut.Analog[2] = Mittelwert_AccNick;
    DebugOut.Analog[3] = Mittelwert_AccRoll;
    DebugOut.Analog[4] = MesswertGier;
    DebugOut.Analog[5] = HoehenWert;
    DebugOut.Analog[6] =(Mess_Integral_Hoch / 512);
    DebugOut.Analog[8] = KompassValue;
    DebugOut.Analog[9] = CurrentUBat;
    DebugOut.Analog[10] = SenderOkay;
    DebugOut.Analog[16] = Mittelwert_AccHoch;

/*    DebugOut.Analog[16] = motor_rx[0];
    DebugOut.Analog[17] = motor_rx[1];
    DebugOut.Analog[18] = motor_rx[2];
    DebugOut.Analog[19] = motor_rx[3];
    DebugOut.Analog[20] = motor_rx[0] + motor_rx[1] + motor_rx[2] + motor_rx[3];
    DebugOut.Analog[20] /= 14;
    DebugOut.Analog[21] = motor_rx[4];
    DebugOut.Analog[22] = motor_rx[5];
    DebugOut.Analog[23] = motor_rx[6];
    DebugOut.Analog[24] = motor_rx[7];
    DebugOut.Analog[25] = motor_rx[4] + motor_rx[5] + motor_rx[6] + motor_rx[7];
*/
//    DebugOut.Analog[9] = MesswertNick;
//    DebugOut.Analog[9] = SollHoehe;
//    DebugOut.Analog[10] = Mess_Integral_Gier / 128;
//    DebugOut.Analog[11] = KompassStartwert;
//    DebugOut.Analog[10] = Parameter_Gyro_I;    
//    DebugOut.Analog[10] = EE_Parameter.Gyro_I;    
//    DebugOut.Analog[9] = KompassRichtung;    
//    DebugOut.Analog[10] = GasMischanteil;
//    DebugOut.Analog[3] = HoeheD * 32;
//    DebugOut.Analog[4] = hoehenregler;
  }

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//  Drehgeschwindigkeit und -winkel zu einem Istwert zusammenfassen
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
    if(Looping_Nick) MesswertNick = MesswertNick * GyroFaktor;
    else             MesswertNick = IntegralNick * IntegralFaktor + MesswertNick * GyroFaktor;
    if(Looping_Roll) MesswertRoll = MesswertRoll * GyroFaktor;
    else             MesswertRoll = IntegralRoll * IntegralFaktor + MesswertRoll * GyroFaktor;
//    MesswertGier = MesswertGier * (GyroFaktor/2) + Integral_Gier * IntegralFaktor;
    MesswertGier = MesswertGier * (GyroFaktor) + Integral_Gier * IntegralFaktor/2;

DebugOut.Analog[28] = MesswertRoll;
DebugOut.Analog[25] = IntegralRoll * IntegralFaktor;
DebugOut.Analog[31] = StickRoll;// / (26*IntegralFaktor);

    // Maximalwerte abfangen
    #define MAX_SENSOR  2048
    if(MesswertNick >  MAX_SENSOR) MesswertNick =  MAX_SENSOR;
    if(MesswertNick < -MAX_SENSOR) MesswertNick = -MAX_SENSOR;
    if(MesswertRoll >  MAX_SENSOR) MesswertRoll =  MAX_SENSOR;
    if(MesswertRoll < -MAX_SENSOR) MesswertRoll = -MAX_SENSOR;
    if(MesswertGier >  MAX_SENSOR) MesswertGier =  MAX_SENSOR;
    if(MesswertGier < -MAX_SENSOR) MesswertGier = -MAX_SENSOR;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Höhenregelung
// Die Höhenregelung schwächt lediglich das Gas ab, erhöht es allerdings nicht
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//OCR0B = 180 - (Poti1 + 120) / 4;
//DruckOffsetSetting = OCR0B;
 if((EE_Parameter.GlobalConfig & CFG_HOEHENREGELUNG))  // Höhenregelung
  {
    int tmp_int;
    if(EE_Parameter.GlobalConfig & CFG_HOEHEN_SCHALTER)  // Regler wird über Schalter gesteuert
    {
     if(Parameter_MaxHoehe < 50) 
      {
       SollHoehe = HoehenWert - 20;  // Parameter_MaxHoehe ist der PPM-Wert des Schalters
       HoehenReglerAktiv = 0;
      }
      else  
        HoehenReglerAktiv = 1;
    }
    else 
    {
     SollHoehe = ((int) ExternHoehenValue + (int) Parameter_MaxHoehe) * (int)EE_Parameter.Hoehe_Verstaerkung - 20;
     HoehenReglerAktiv = 1;
    }

    if(Notlandung) SollHoehe = 0;
    h = HoehenWert;
    if((h > SollHoehe) && HoehenReglerAktiv)      // zu hoch --> drosseln
     {      h = ((h - SollHoehe) * (int) Parameter_Hoehe_P) / 16; // Differenz bestimmen --> P-Anteil
      h = GasMischanteil - h;         // vom Gas abziehen
      h -= (HoeheD * Parameter_Luftdruck_D)/8;    // D-Anteil
      tmp_int = ((Mess_Integral_Hoch / 512) * (signed long) Parameter_Hoehe_ACC_Wirkung) / 32;
      if(tmp_int > 50) tmp_int = 50;
      else if(tmp_int < -50) tmp_int = -50;
      h -= tmp_int;
      hoehenregler = (hoehenregler*15 + h) / 16;      
      if(hoehenregler < EE_Parameter.Hoehe_MinGas) // nicht unter MIN
       {
         if(GasMischanteil >= EE_Parameter.Hoehe_MinGas) hoehenregler = EE_Parameter.Hoehe_MinGas;
         if(GasMischanteil < EE_Parameter.Hoehe_MinGas) hoehenregler = GasMischanteil;
       }  
      if(hoehenregler > GasMischanteil) hoehenregler = GasMischanteil; // nicht mehr als Gas
      GasMischanteil = hoehenregler;
     } 
  }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// + Mischer und PI-Regler
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
  DebugOut.Analog[7] = GasMischanteil;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Gier-Anteil
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
#define MUL_G  1.0
    GierMischanteil = MesswertGier - sollGier;     // Regler für Gier
//GierMischanteil = 0;

    if(GierMischanteil > (GasMischanteil / 2)) GierMischanteil = GasMischanteil / 2; 
    if(GierMischanteil < -(GasMischanteil / 2)) GierMischanteil = -(GasMischanteil / 2); 
    if(GierMischanteil > ((MAX_GAS - GasMischanteil))) GierMischanteil = ((MAX_GAS - GasMischanteil)); 
    if(GierMischanteil < -((MAX_GAS - GasMischanteil))) GierMischanteil = -((MAX_GAS - GasMischanteil));

    if(GasMischanteil < 20) GierMischanteil = 0;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Nick-Achse
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
    DiffNick = MesswertNick - (StickNick - GPS_Nick);	// Differenz bestimmen
    if(IntegralFaktor) SummeNick += IntegralNick * IntegralFaktor - (StickNick - GPS_Nick); // I-Anteil bei Winkelregelung
    else  SummeNick += DiffNick; // I-Anteil bei HH 
    if(SummeNick > 0) SummeNick-= 2 ; else SummeNick += 2 ; 
    if(SummeNick >  16000) SummeNick =  16000;
    if(SummeNick < -16000) SummeNick = -16000;
    pd_ergebnis = DiffNick + Ki * SummeNick; // PI-Regler für Nick					
    // Motor Vorn
    tmp_int = (long)((long)Parameter_DynamicStability * (long)(GasMischanteil + abs(GierMischanteil)/2)) / 64;
    if(pd_ergebnis >  tmp_int) pd_ergebnis =  tmp_int; 
    if(pd_ergebnis < -tmp_int) pd_ergebnis = -tmp_int; 

    motorwert = GasMischanteil + pd_ergebnis + GierMischanteil;	  // Mischer
	if ((motorwert < 0)) motorwert = 0;
	else if(motorwert > MAX_GAS)  	    motorwert = MAX_GAS;
	if (motorwert < MIN_GAS)            motorwert = MIN_GAS;	
	Motor_Vorne = motorwert;	   
    // Motor Heck
	motorwert = GasMischanteil - pd_ergebnis + GierMischanteil;
	if ((motorwert < 0)) motorwert = 0;
	else if(motorwert > MAX_GAS)	    motorwert = MAX_GAS;
	if (motorwert < MIN_GAS)            motorwert = MIN_GAS;
	Motor_Hinten = motorwert;		
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
// Roll-Achse
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
	DiffRoll = MesswertRoll - (StickRoll  - GPS_Roll);	// Differenz bestimmen
    if(IntegralFaktor) SummeRoll += IntegralRoll * IntegralFaktor - (StickRoll  - GPS_Roll);// I-Anteil bei Winkelregelung
    else 	         SummeRoll += DiffRoll;  // I-Anteil bei HH
    if(SummeRoll > 0) SummeRoll-= 2 ; else SummeRoll += 2 ; 
    if(SummeRoll >  16000) SummeRoll =  16000;
    if(SummeRoll < -16000) SummeRoll = -16000;
    pd_ergebnis = DiffRoll + Ki * SummeRoll;	// PI-Regler für Roll
    tmp_int = (long)((long)Parameter_DynamicStability * (long)(GasMischanteil + abs(GierMischanteil)/2)) / 64;
    if(pd_ergebnis >  tmp_int) pd_ergebnis =  tmp_int; 
    if(pd_ergebnis < -tmp_int) pd_ergebnis = -tmp_int; 
    // Motor Links
    motorwert = GasMischanteil + pd_ergebnis - GierMischanteil;
	if ((motorwert < 0)) motorwert = 0;
	else if(motorwert > MAX_GAS)		motorwert = MAX_GAS;
	if (motorwert < MIN_GAS)            motorwert = MIN_GAS;
	Motor_Links = motorwert;		
    // Motor Rechts
	motorwert = GasMischanteil - pd_ergebnis - GierMischanteil;
	if ((motorwert < 0)) motorwert = 0;
	else if(motorwert > MAX_GAS)	   	motorwert = MAX_GAS;
	if (motorwert < MIN_GAS)            motorwert = MIN_GAS;	
	Motor_Rechts = motorwert;
   // +++++++++++++++++++++++++++++++++++++++++++++++
}

