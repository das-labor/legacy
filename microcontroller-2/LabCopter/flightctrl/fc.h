/*#######################################################################################
Flight Control
#######################################################################################*/

#ifndef _FC_H
#define _FC_H

extern volatile unsigned int I2CTimeout;
extern unsigned char Sekunde,Minute;
extern volatile long IntegralNick,IntegralNick2;
extern volatile long IntegralRoll,IntegralRoll2;
extern volatile long Mess_IntegralNick,Mess_IntegralNick2;
extern volatile long Mess_IntegralRoll,Mess_IntegralRoll2;
extern volatile long IntegralAccNick,IntegralAccRoll;
extern volatile long Mess_Integral_Hoch;
extern volatile long Integral_Gier,Mess_Integral_Gier,Mess_Integral_Gier2;
extern volatile int  KompassValue;
extern volatile int  KompassStartwert;
extern volatile int  KompassRichtung;
extern int HoehenWert;
extern int SollHoehe;
extern volatile int MesswertNick,MesswertRoll,MesswertGier;
extern volatile int AdNeutralNick,AdNeutralRoll,AdNeutralGier, Mittelwert_AccNick, Mittelwert_AccRoll;
extern volatile int NeutralAccX, NeutralAccY,Mittelwert_AccHoch;
extern volatile float NeutralAccZ;
extern long Umschlag180Nick, Umschlag180Roll;
extern signed int ExternStickNick,ExternStickRoll,ExternStickGier;
extern unsigned char Parameter_UserParam1,Parameter_UserParam2,Parameter_UserParam3,Parameter_UserParam4,Parameter_UserParam5,Parameter_UserParam6,Parameter_UserParam7,Parameter_UserParam8;

void MotorRegler(void);
void SendMotorData(void);
void CalibrierMittelwert(void);
void Mittelwert(void);
void SetNeutral(void);
void Piep(unsigned char Anzahl);
extern void DefaultKonstanten(void);
void DefaultKonstanten1(void);
void DefaultKonstanten2(void);

extern unsigned char h,m,s;
extern volatile unsigned char Timeout ;
extern unsigned char CosinusNickWinkel, CosinusRollWinkel;
extern volatile long IntegralNick,IntegralNick2;
extern volatile long IntegralRoll,IntegralRoll2;
extern volatile long Integral_Gier;
extern volatile long Mess_IntegralNick,Mess_IntegralNick2;
extern volatile long Mess_IntegralRoll,Mess_IntegralRoll2;
extern volatile long Mess_Integral_Gier;
extern volatile int  DiffNick,DiffRoll;
extern int  Poti1, Poti2, Poti3, Poti4; 
extern volatile unsigned char Motor_Vorne,Motor_Hinten,Motor_Rechts,Motor_Links, Count;
extern unsigned char MotorWert[5];
extern volatile unsigned char SenderOkay;
extern int StickNick,StickRoll,StickGier;
extern char MotorenEin;
extern void DefaultKonstanten1(void);
extern void DefaultKonstanten2(void);

#define  STRUCT_PARAM_LAENGE  71
struct mk_param_struct
 {
   unsigned char Kanalbelegung[8];        // GAS[0], GIER[1],NICK[2], ROLL[3], POTI1, POTI2, POTI3
   unsigned char GlobalConfig;            // 0x01=Höhenregler aktiv,0x02=Kompass aktiv, 0x04=GPS aktiv, 0x08=Heading Hold aktiv
   unsigned char Hoehe_MinGas;            // Wert : 0-100
   unsigned char Luftdruck_D;             // Wert : 0-250
   unsigned char MaxHoehe;                // Wert : 0-32
   unsigned char Hoehe_P;                 // Wert : 0-32
   unsigned char Hoehe_Verstaerkung;      // Wert : 0-50
   unsigned char Hoehe_ACC_Wirkung;      // Wert : 0-250
   unsigned char Stick_P;                // Wert : 1-6
   unsigned char Stick_D;                // Wert : 0-64
   unsigned char Gier_P;                 // Wert : 1-20
   unsigned char Gas_Min;                // Wert : 0-32
   unsigned char Gas_Max;                // Wert : 33-250
   unsigned char GyroAccFaktor;          // Wert : 1-64
   unsigned char KompassWirkung;         // Wert : 0-32
   unsigned char Gyro_P;                 // Wert : 10-250
   unsigned char Gyro_I;                 // Wert : 0-250
   unsigned char UnterspannungsWarnung;  // Wert : 0-250
   unsigned char NotGas;                 // Wert : 0-250     //Gaswert bei Empängsverlust
   unsigned char NotGasZeit;             // Wert : 0-250     // Zeitbis auf NotGas geschaltet wird, wg. Rx-Problemen
   unsigned char UfoAusrichtung;         // X oder + Formation
   unsigned char I_Faktor;               // Wert : 0-250
   unsigned char UserParam1;             // Wert : 0-250
   unsigned char UserParam2;             // Wert : 0-250
   unsigned char UserParam3;             // Wert : 0-250
   unsigned char UserParam4;             // Wert : 0-250
   unsigned char ServoNickControl;       // Wert : 0-250     // Stellung des Servos
   unsigned char ServoNickComp;          // Wert : 0-250     // Einfluss Gyro/Servo
   unsigned char ServoNickMin;           // Wert : 0-250     // Anschlag
   unsigned char ServoNickMax;           // Wert : 0-250     // Anschlag
   unsigned char ServoNickRefresh;       //
   unsigned char LoopGasLimit;           // Wert: 0-250  max. Gas während Looping
   unsigned char LoopThreshold;          // Wert: 0-250  Schwelle für Stickausschlag
   unsigned char LoopHysterese;          // Wert: 0-250  Hysterese für Stickausschlag
   unsigned char AchsKopplung1;          // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick koppelt (NickRollMitkopplung)
   unsigned char AchsGegenKopplung1;     // Wert: 0-250  Faktor, mit dem Gier die Achsen Roll und Nick Gegenkoppelt (NickRollGegenkopplung)
   unsigned char WinkelUmschlagNick;     // Wert: 0-250  180°-Punkt
   unsigned char WinkelUmschlagRoll;     // Wert: 0-250  180°-Punkt
   unsigned char GyroAccAbgleich;        // 1/k  (Koppel_ACC_Wirkung)
   unsigned char Driftkomp;              
   unsigned char DynamicStability;
   unsigned char UserParam5;             // Wert : 0-250
   unsigned char UserParam6;             // Wert : 0-250
   unsigned char UserParam7;             // Wert : 0-250
   unsigned char UserParam8;             // Wert : 0-250

//------------------------------------------------
   unsigned char LoopConfig;             // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
   unsigned char ServoNickCompInvert;    // Wert : 0-250   0 oder 1  // WICHTIG!!! am Ende lassen
   unsigned char Reserved[4];
   char Name[12];
 }; 


/*
unsigned char ServoNickMax;           // Wert : 0-250
    unsigned char ServoNickRefresh;       //
    unsigned char LoopGasLimit;           // Wert: 0-250  max. Gas während Looping
    unsigned char LoopThreshold;          // Wert: 0-250  Schwelle für Stickausschlag
//------------------------------------------------
    unsigned char LoopConfig;             // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
    unsigned char ServoNickCompInvert;    // Wert : 0-250   0 oder 1  // WICHTIG!!! am Ende lassen
    unsigned char Reserved[4];
    char Name[12];
*/
	
extern struct mk_param_struct EE_Parameter;

extern unsigned char Parameter_Luftdruck_D;
extern unsigned char Parameter_MaxHoehe;
extern unsigned char Parameter_Hoehe_P;
extern unsigned char Parameter_Hoehe_ACC_Wirkung;
extern unsigned char Parameter_KompassWirkung;
extern unsigned char Parameter_Gyro_P;
extern unsigned char Parameter_Gyro_I;
extern unsigned char Parameter_Gier_P;
extern unsigned char Parameter_ServoNickControl;
extern unsigned char Parameter_AchsKopplung1;
extern unsigned char Parameter_AchsGegenKopplung1;


#endif //_FC_H

