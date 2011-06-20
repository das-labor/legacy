extern volatile unsigned long timeval;
#define FASTRUN __attribute__ ((long_call, section (".fastrun"))) __attribute__ ((noinline))

FASTRUN void tmr0_interrupt(void);

//void __attribute__ ((interrupt("IRQ"))) rtc_interrupt(void);

void __attribute__ ((interrupt("IRQ"))) tmr1_interrupt(void);

void __attribute__ ((interrupt("IRQ"))) rtc_interrupt(void);

void init_timer0(void);
void init_timer1(void);
void init_timer2(void);
void init_timer3(void);
void init_RTC (void);
