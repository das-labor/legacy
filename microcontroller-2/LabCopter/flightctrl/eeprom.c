// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// + Konstanten 
// + 0-250 -> normale Werte
// + 251 -> Poti1
// + 252 -> Poti2
// + 253 -> Poti3
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void DefaultKonstanten1(void)
{
 EE_Parameter.Kanalbelegung[K_NICK]  = 1;
 EE_Parameter.Kanalbelegung[K_ROLL]  = 2;
 EE_Parameter.Kanalbelegung[K_GAS]   = 3;
 EE_Parameter.Kanalbelegung[K_GIER]  = 4;
 EE_Parameter.Kanalbelegung[K_POTI1] = 5;
 EE_Parameter.Kanalbelegung[K_POTI2] = 6;
 EE_Parameter.Kanalbelegung[K_POTI3] = 7;
 EE_Parameter.Kanalbelegung[K_POTI4] = 8;
 EE_Parameter.GlobalConfig = CFG_ACHSENKOPPLUNG_AKTIV;//CFG_HOEHENREGELUNG | /*CFG_HOEHEN_SCHALTER |*/ CFG_KOMPASS_AKTIV | CFG_KOMPASS_FIX;//0x01;    
 EE_Parameter.Hoehe_MinGas = 30;
 EE_Parameter.MaxHoehe     = 251;      // Wert : 0-250   251 -> Poti1
 EE_Parameter.Hoehe_P      = 10;       // Wert : 0-32
 EE_Parameter.Luftdruck_D  = 50;       // Wert : 0-250
 EE_Parameter.Hoehe_ACC_Wirkung = 50;  // Wert : 0-250
 EE_Parameter.Hoehe_Verstaerkung = 4;  // Wert : 0-50
 EE_Parameter.Stick_P = 4; //2         // Wert : 1-6
 EE_Parameter.Stick_D = 8; //8         // Wert : 0-64
 EE_Parameter.Gier_P = 14;             // Wert : 1-20
 EE_Parameter.Gas_Min = 15;            // Wert : 0-32
 EE_Parameter.Gas_Max = 250;           // Wert : 33-250
 EE_Parameter.GyroAccFaktor = 26;      // Wert : 1-64
 EE_Parameter.KompassWirkung = 128;    // Wert : 0-250
 EE_Parameter.Gyro_P = 120; //80          // Wert : 0-250
 EE_Parameter.Gyro_I = 150;               // Wert : 0-250
 EE_Parameter.UnterspannungsWarnung = 94; // Wert : 0-250
 EE_Parameter.NotGas = 35;                // Wert : 0-250     // Gaswert bei Empangsverlust
 EE_Parameter.NotGasZeit = 20;            // Wert : 0-250     // Zeit bis auf NotGas geschaltet wird, wg. Rx-Problemen
 EE_Parameter.UfoAusrichtung = 0;         // X oder + Formation
 EE_Parameter.I_Faktor = 32;
 EE_Parameter.UserParam1 = 32 * 4;             //zur freien Verwendung
 EE_Parameter.UserParam2 = 0;             //zur freien Verwendung
 EE_Parameter.UserParam3 = 0;             //zur freien Verwendung
 EE_Parameter.UserParam4 = 0;             //zur freien Verwendung
 EE_Parameter.UserParam5 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam6 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam7 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam8 = 0;             // zur freien Verwendung
 EE_Parameter.ServoNickControl = 100;     // Wert : 0-250     // Stellung des Servos
 EE_Parameter.ServoNickComp = 40;         // Wert : 0-250     // Einfluss Gyro/Servo
 EE_Parameter.ServoNickCompInvert = 0;    // Wert : 0-250     // Richtung Einfluss Gyro/Servo
 EE_Parameter.ServoNickMin = 50;          // Wert : 0-250     // Anschlag
 EE_Parameter.ServoNickMax = 150;         // Wert : 0-250     // Anschlag
 EE_Parameter.ServoNickRefresh = 5;
 EE_Parameter.LoopGasLimit = 50;
 EE_Parameter.LoopThreshold = 90;         // Wert: 0-250  Schwelle für Stickausschlag
 EE_Parameter.LoopHysterese = 50;
 EE_Parameter.LoopConfig = 0;             // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts / wird getrennt behandelt
 EE_Parameter.AchsKopplung1 = 100;
 EE_Parameter.AchsGegenKopplung1 = 10;
 EE_Parameter.WinkelUmschlagNick = 100;
 EE_Parameter.WinkelUmschlagRoll = 100;
 EE_Parameter.GyroAccAbgleich = 50;        // 1/k 
 EE_Parameter.Driftkomp = 4;              
 EE_Parameter.DynamicStability = 100;
 memcpy(EE_Parameter.Name, "Sport\0", 12);   
}
void DefaultKonstanten2(void)
{
 EE_Parameter.Kanalbelegung[K_NICK]  = 1;
 EE_Parameter.Kanalbelegung[K_ROLL]  = 2;
 EE_Parameter.Kanalbelegung[K_GAS]   = 3;
 EE_Parameter.Kanalbelegung[K_GIER]  = 4;
 EE_Parameter.Kanalbelegung[K_POTI1] = 5;
 EE_Parameter.Kanalbelegung[K_POTI2] = 6;
 EE_Parameter.Kanalbelegung[K_POTI3] = 7;
 EE_Parameter.Kanalbelegung[K_POTI4] = 8;
 EE_Parameter.GlobalConfig = CFG_ACHSENKOPPLUNG_AKTIV;//CFG_HOEHENREGELUNG | /*CFG_HOEHEN_SCHALTER |*/ CFG_KOMPASS_AKTIV;//0x01;    
 EE_Parameter.Hoehe_MinGas = 30;
 EE_Parameter.MaxHoehe     = 251;         // Wert : 0-250   251 -> Poti1
 EE_Parameter.Hoehe_P      = 10;          // Wert : 0-32
 EE_Parameter.Luftdruck_D  = 50;          // Wert : 0-250
 EE_Parameter.Hoehe_ACC_Wirkung = 50;     // Wert : 0-250
 EE_Parameter.Hoehe_Verstaerkung = 2;     // Wert : 0-50
 EE_Parameter.Stick_P = 4; //2            // Wert : 1-6
 EE_Parameter.Stick_D = 0; //8            // Wert : 0-64
 EE_Parameter.Gier_P = 10;                // Wert : 1-20
 EE_Parameter.Gas_Min = 15;               // Wert : 0-32
 EE_Parameter.Gas_Max = 250;              // Wert : 33-250
 EE_Parameter.GyroAccFaktor = 26;         // Wert : 1-64
 EE_Parameter.KompassWirkung = 128;       // Wert : 0-250
 EE_Parameter.Gyro_P = 175; //80          // Wert : 0-250
 EE_Parameter.Gyro_I = 175;               // Wert : 0-250
 EE_Parameter.UnterspannungsWarnung = 94; // Wert : 0-250
 EE_Parameter.NotGas = 35;                // Wert : 0-250     // Gaswert bei Empangsverlust
 EE_Parameter.NotGasZeit = 20;            // Wert : 0-250     // Zeit bis auf NotGas geschaltet wird, wg. Rx-Problemen
 EE_Parameter.UfoAusrichtung = 0;         // X oder + Formation
 EE_Parameter.I_Faktor = 32;
 EE_Parameter.UserParam1 = 20 * 4;             // zur freien Verwendung
 EE_Parameter.UserParam2 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam3 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam4 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam5 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam6 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam7 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam8 = 0;             // zur freien Verwendung
 EE_Parameter.ServoNickControl = 100;     // Wert : 0-250     // Stellung des Servos
 EE_Parameter.ServoNickComp = 40;         // Wert : 0-250     // Einfluss Gyro/Servo
 EE_Parameter.ServoNickCompInvert = 0;    // Wert : 0-250     // Richtung Einfluss Gyro/Servo
 EE_Parameter.ServoNickMin = 50;          // Wert : 0-250     // Anschlag
 EE_Parameter.ServoNickMax = 150;         // Wert : 0-250     // Anschlag
 EE_Parameter.ServoNickRefresh = 5;
 EE_Parameter.LoopGasLimit = 50;
 EE_Parameter.LoopThreshold = 90;         // Wert: 0-250  Schwelle für Stickausschlag
 EE_Parameter.LoopHysterese = 50;
 EE_Parameter.LoopConfig = 0;             // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts 
 EE_Parameter.AchsKopplung1 = 100;        // Faktor, mit dem Gier die Achsen Roll und Nick verkoppelt
 EE_Parameter.AchsGegenKopplung1 = 10;
 EE_Parameter.WinkelUmschlagNick = 100;
 EE_Parameter.WinkelUmschlagRoll = 100;
 EE_Parameter.GyroAccAbgleich = 100;        // 1/k 
 EE_Parameter.Driftkomp = 4;              
 EE_Parameter.DynamicStability = 75;
 memcpy(EE_Parameter.Name, "Normal\0", 12);   
}

void DefaultKonstanten3(void)
{
 EE_Parameter.Kanalbelegung[K_NICK]  = 1;
 EE_Parameter.Kanalbelegung[K_ROLL]  = 2;
 EE_Parameter.Kanalbelegung[K_GAS]   = 3;
 EE_Parameter.Kanalbelegung[K_GIER]  = 4;
 EE_Parameter.Kanalbelegung[K_POTI1] = 5;
 EE_Parameter.Kanalbelegung[K_POTI2] = 6;
 EE_Parameter.Kanalbelegung[K_POTI3] = 7;
 EE_Parameter.Kanalbelegung[K_POTI4] = 8;
 EE_Parameter.GlobalConfig = CFG_DREHRATEN_BEGRENZER | CFG_ACHSENKOPPLUNG_AKTIV;///*CFG_HOEHEN_SCHALTER |*/ CFG_KOMPASS_AKTIV;//0x01;    
 EE_Parameter.Hoehe_MinGas = 30;
 EE_Parameter.MaxHoehe     = 251;         // Wert : 0-250   251 -> Poti1
 EE_Parameter.Hoehe_P      = 10;          // Wert : 0-32
 EE_Parameter.Luftdruck_D  = 50;          // Wert : 0-250
 EE_Parameter.Hoehe_ACC_Wirkung = 50;     // Wert : 0-250
 EE_Parameter.Hoehe_Verstaerkung = 2;     // Wert : 0-50
 EE_Parameter.Stick_P = 3; //2            // Wert : 1-6
 EE_Parameter.Stick_D = 0; //8            // Wert : 0-64
 EE_Parameter.Gier_P  = 8;                // Wert : 1-20
 EE_Parameter.Gas_Min = 15;               // Wert : 0-32
 EE_Parameter.Gas_Max = 250;              // Wert : 33-250
 EE_Parameter.GyroAccFaktor = 26;         // Wert : 1-64
 EE_Parameter.KompassWirkung = 128;       // Wert : 0-250
 EE_Parameter.Gyro_P = 200; //80          // Wert : 0-250
 EE_Parameter.Gyro_I = 175;               // Wert : 0-250
 EE_Parameter.UnterspannungsWarnung = 94; // Wert : 0-250
 EE_Parameter.NotGas = 35;                // Wert : 0-250     // Gaswert bei Empangsverlust
 EE_Parameter.NotGasZeit = 20;            // Wert : 0-250     // Zeit bis auf NotGas geschaltet wird, wg. Rx-Problemen
 EE_Parameter.UfoAusrichtung = 0;         // X oder + Formation
 EE_Parameter.I_Faktor = 10;
 EE_Parameter.UserParam1 = 20 * 4;             // zur freien Verwendung
 EE_Parameter.UserParam2 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam3 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam4 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam5 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam6 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam7 = 0;             // zur freien Verwendung
 EE_Parameter.UserParam8 = 0;             // zur freien Verwendung
 EE_Parameter.ServoNickControl = 100;     // Wert : 0-250     // Stellung des Servos
 EE_Parameter.ServoNickComp = 40;         // Wert : 0-250     // Einfluss Gyro/Servo
 EE_Parameter.ServoNickCompInvert = 0;    // Wert : 0-250     // Richtung Einfluss Gyro/Servo
 EE_Parameter.ServoNickMin = 50;          // Wert : 0-250     // Anschlag
 EE_Parameter.ServoNickMax = 150;         // Wert : 0-250     // Anschlag
 EE_Parameter.ServoNickRefresh = 5;
 EE_Parameter.LoopGasLimit = 50;
 EE_Parameter.LoopThreshold = 90;         // Wert: 0-250  Schwelle für Stickausschlag
 EE_Parameter.LoopHysterese = 50;
 EE_Parameter.LoopConfig = 0;             // Bitcodiert: 0x01=oben, 0x02=unten, 0x04=links, 0x08=rechts 
 EE_Parameter.AchsKopplung1 = 100;        // Faktor, mit dem Gier die Achsen Roll und Nick verkoppelt
 EE_Parameter.AchsGegenKopplung1 = 10;
 EE_Parameter.WinkelUmschlagNick = 100;
 EE_Parameter.WinkelUmschlagRoll = 100;
 EE_Parameter.GyroAccAbgleich = 100;        // 1/k 
 EE_Parameter.Driftkomp = 4;              
 EE_Parameter.DynamicStability = 50;
 memcpy(EE_Parameter.Name, "Beginner\0", 12);   
}
