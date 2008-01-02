/*
Get ADC values
V0.1
02.02.2008
Hansi
*/

//ADC Interrupt Handler
//Saves all current sensor values to corresponding globals
SIGNAL(SIG_ADC)
{
    static unsigned char kanal=0,state = 0;
    static unsigned int gier1, roll1, nick1;
    ANALOG_OFF;
    switch(state++)
        {
        case 0:
            gier1 = ADC;
            kanal = 1;
            ZaehlMessungen++;
            break;
        case 1:
            roll1 = ADC;
            kanal = 2;
            break;
        case 2:
            nick1 = ADC;
            kanal = 4;
            break;
        case 3:
            UBat = (3 * UBat + ADC / 3) / 4;//(UBat + ((ADC * 39) / 256) + 19)  / 2;
            kanal = 6;
            break;
        case 4:
            Aktuell_ay = NeutralAccY - ADC;
            AdWertAccRoll = Aktuell_ay;
            kanal = 7;
            break;
        case 5:
            Aktuell_ax = ADC - NeutralAccX;
            AdWertAccNick =  Aktuell_ax;
                    kanal = 0;
            break;
        case 6:
            if(PlatinenVersion == 10)  AdWertGier = (ADC + gier1) / 2;
                        else                                       AdWertGier = ADC + gier1;
            kanal = 1;
            break;
        case 7:
            if(PlatinenVersion == 10)  AdWertRoll = (ADC + roll1) / 2;
                        else                                       AdWertRoll = ADC + roll1;
            kanal = 2;
            break;
        case 8:
            if(PlatinenVersion == 10)  AdWertNick = (ADC + nick1) / 2;
                        else                                       AdWertNick = ADC + nick1;
//AdWertNick = 0;
//AdWertNick += Poti2;            
            kanal = 5;
            break;
       case 9:
            AdWertAccHoch =  (signed int) ADC - NeutralAccZ;
            AdWertAccHoch += abs(Aktuell_ay) / 4 + abs(Aktuell_ax) / 4;
            if(AdWertAccHoch > 1) 
             {
              if(NeutralAccZ < 800) NeutralAccZ+= 0.02; 
             }  
             else if(AdWertAccHoch < -1)
             {
              if(NeutralAccZ > 600) NeutralAccZ-= 0.02;
             } 
            messanzahl_AccHoch = 1;
            Aktuell_az = ADC;
            Mess_Integral_Hoch += AdWertAccHoch;      // Integrieren
            Mess_Integral_Hoch -= Mess_Integral_Hoch / 1024; // d�mfen
                kanal = 3;
            break;
        case 10:
            tmpLuftdruck += ADC;
            if(++messanzahl_Druck >= 5) 
                {
                MessLuftdruck = ADC;
                messanzahl_Druck = 0;
                                HoeheD = (int)(StartLuftdruck - tmpLuftdruck - HoehenWert);  // D-Anteil = neuerWert - AlterWert
                Luftdruck = (tmpLuftdruck + 3 * Luftdruck) / 4;
                HoehenWert = StartLuftdruck - Luftdruck;
                tmpLuftdruck = 0;
                } 
            kanal = 0;
            state = 0;
            break;
        default: 
            kanal = 0;
            state = 0;
            break;
        } 
    ADMUX = kanal;
    if(state != 0) ANALOG_ON;
}
