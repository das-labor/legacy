#include "main.h"

volatile unsigned int CountMilliseconds = 0;
volatile static unsigned int tim_main;
volatile unsigned char UpdateMotor = 0;
volatile unsigned int cntKompass = 0;
volatile unsigned int beeptime = 0;
unsigned int BeepMuster = 0xffff;
int ServoValue = 0;

enum {
  STOP             = 0,
  CK               = 1,
  CK8              = 2,
  CK64             = 3,
  CK256            = 4,
  CK1024           = 5,
  T0_FALLING_EDGE  = 6,
  T0_RISING_EDGE   = 7
};


SIGNAL (SIG_OVERFLOW0)    // 8kHz
{
    static unsigned char cnt_1ms = 1,cnt = 0;
    unsigned char pieper_ein = 0;
//    TCNT0 -= 250;//TIMER_RELOAD_VALUE;

   if(!cnt--)
    {
     cnt = 9; 
     cnt_1ms++;
     cnt_1ms %= 2;
     if(!cnt_1ms) UpdateMotor = 1;
     CountMilliseconds++;
     }   

     if(beeptime > 1)
        {
        beeptime--;      
        if(beeptime & BeepMuster) 
         {
          pieper_ein = 1;
         }
         else pieper_ein = 0;
        }
     else 
      {
       pieper_ein = 0;
       BeepMuster = 0xffff;
      } 


     if(pieper_ein)
        {
          if(PlatinenVersion == 10) PORTD |= (1<<2); // Speaker an PORTD.2
          else                      PORTC |= (1<<7); // Speaker an PORTC.7
        }
     else  
        {
         if(PlatinenVersion == 10) PORTD &= ~(1<<2);
         else                      PORTC &= ~(1<<7);
        } 
 
 if(EE_Parameter.GlobalConfig & CFG_KOMPASS_AKTIV)
 {
  if(PINC & 0x10)
   {
    cntKompass++; 
   }
  else
   {
    if((cntKompass) && (cntKompass < 4000)) 
    { 
     KompassValue = cntKompass;
    } 
//     if(cntKompass < 10) cntKompass = 10;
//     KompassValue = (unsigned long)((unsigned long)(cntKompass-10)*720L + 1L) / 703L;
     KompassRichtung = ((540 + KompassValue - KompassStartwert) % 360) - 180;
    cntKompass = 0;
   } 
 }
}


void Timer_Init(void)
{
    tim_main = SetDelay(10);
    TCCR0B = CK8;
    TCCR0A = (1<<COM0A1)|(1<<COM0B1)|3;//fast PWM
    OCR0A =  0;
    OCR0B = 120;
    TCNT0 = (unsigned char)-TIMER_RELOAD_VALUE;  // reload
    //OCR1  = 0x00;

    TCCR2A=(1<<COM2A1)|(1<<COM2A0)|3;
    TCCR2B=(0<<CS20)|(1<<CS21)|(1<<CS22);
    
//    TIMSK2 |= _BV(TOIE2);
TIMSK2 |= _BV(OCIE2A);

    TIMSK0 |= _BV(TOIE0);
    OCR2A = 10;
    TCNT2 = 0;
    
}

// -----------------------------------------------------------------------

unsigned int SetDelay (unsigned int t)
{
//  TIMSK0 &= ~_BV(TOIE0);
  return(CountMilliseconds + t + 1);                                             
//  TIMSK0 |= _BV(TOIE0);
}

// -----------------------------------------------------------------------
char CheckDelay(unsigned int t)
{
//  TIMSK0 &= ~_BV(TOIE0);
  return(((t - CountMilliseconds) & 0x8000) >> 9);
//  TIMSK0 |= _BV(TOIE0);
}

// -----------------------------------------------------------------------
void Delay_ms(unsigned int w)
{
 unsigned int akt;
 akt = SetDelay(w);
 while (!CheckDelay(akt));
}

void Delay_ms_Mess(unsigned int w)
{
 unsigned int akt;
 akt = SetDelay(w);
 while (!CheckDelay(akt)) ANALOG_ON;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
//  Servo ansteuern
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
SIGNAL(SIG_OUTPUT_COMPARE2A)
{
  static unsigned char timer = 10;
  
  if(!timer--)  
    {
     TCCR2A=(1<<COM2A1)|(0<<COM2A0)|3;  
     ServoValue =  Parameter_ServoNickControl;
     if(EE_Parameter.ServoNickCompInvert & 0x01) ServoValue += ((long) EE_Parameter.ServoNickComp * (IntegralNick / 128)) / 512;
     else ServoValue -= ((long) EE_Parameter.ServoNickComp * (IntegralNick / 128)) / 512;
     
     if(ServoValue < EE_Parameter.ServoNickMin) ServoValue = EE_Parameter.ServoNickMin;
     else if(ServoValue > EE_Parameter.ServoNickMax) ServoValue = EE_Parameter.ServoNickMax;

     OCR2A = ServoValue;// + 75;
     timer = EE_Parameter.ServoNickRefresh;
    }
    else 
    {
     TCCR2A =3;
     PORTD&=~0x80;
    } 
}
