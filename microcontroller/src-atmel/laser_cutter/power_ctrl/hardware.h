
//Schieberegister Bit1-7 = relais ausgänge
//Main power = bit 7
#define SHIFT_SS_PORT B
#define SHIFT_SS_BIT  4

//low = enable
#define SHIFT_OE_PORT B
#define SHIFT_OE_BIT  3

#define SHIFT_MOSI_PORT B
#define SHIFT_MOSI_BIT  5

#define SHIFT_SCK_PORT B
#define SHIFT_SCK_BIT  7

#define OCR1A_PORT D
#define OCR1A_BIT  5

#define FIRE_PORT B
#define FIRE_BIT  0

//low = NT Enable
#define NT_INHIBIT_PORT A
#define NT_INHIBIT_BIT  0

//high = power on
#define NT_POWER_PORT   A
#define NT_POWER_BIT    1

//low =  nt off
#define NT_STATUS_OFF_PORT C
#define NT_STATUS_OFF_BIT  0

//low =  error
#define NT_NO_CURRENT_REV_PORT C
#define NT_NO_CURRENT_REV_BIT  1

//low =  overload
#define NT_OVERLOAD_PORT C
#define NT_OVERLOAD_BIT  2

//low =  interlock_open
#define NT_INTERLOCK_OPEN_PORT C
#define NT_INTERLOCK_OPEN_BIT  3

//low =  thermal overload
#define NT_THERMAL_OVERLOAD_PORT C
#define NT_THERMAL_OVERLOAD_BIT  4

//low =  on
#define NT_STATUS_ON_PORT C
#define NT_STATUS_ON_BIT  5

//low =  end of charge
#define NT_END_OF_CHARGE_PORT C
#define NT_END_OF_CHARGE_BIT  6

//low =  inhibited
#define NT_STATUS_INHIBIT_PORT C
#define NT_STATUS_INHIBIT_BIT  7

//OC1A - 0 ist 0V und volle kanne ist 1400V
#define PWM_U_SOLL_PORT        D
#define PWM_U_SOLL_BIT         5

//reserve OC1B
#define PWM_RESERVE_PORT        D
#define PWM_RESERVE_BIT         4


//Eingang für externen Feuerbefehl
#define FIRE_IN_PORT        A
#define FIRE_IN_BIT         5


//ADC7: U_IST
//über Spannungteiler 15V/2.36V

//ADC6: reserve


//Setzt einen Port am Schieberegister
#define SET_SHIFT_PORT(m)   shift_port |=  m; shift_update()
#define CLEAR_SHIFT_PORT(m) shift_port &= ~m; shift_update()

//Ausgänge des Schieberegisters
#define REL_NT_CONTROL (1<<7) //R1   Ladegerät Steuerung (Schieberegister QH,Bit 7)
#define REL_NT_POWER   (1<<6) //R2   Ladegerät Leistung (Schieberegister QG, Bit 6)
#define REL_SIMMER     (1<<5) //R3   Simmernetzteil (Scheiberegister GF, Bit 5)
#define REL_PUMPE      (1<<4) //R4   Pumpe
                                //R4 - R7 Reserve



