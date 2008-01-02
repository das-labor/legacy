
#define TIMER_TEILER        CK8
#define TIMER_RELOAD_VALUE  250

void Timer_Init(void);
void Delay_ms(unsigned int);
void Delay_ms_Mess(unsigned int);
unsigned int SetDelay (unsigned int t);
char CheckDelay (unsigned int t);

extern volatile unsigned int CountMilliseconds;
extern volatile unsigned char UpdateMotor;
extern volatile unsigned int beeptime;
extern volatile unsigned int cntKompass;
extern int ServoValue;
extern unsigned int BeepMuster;
