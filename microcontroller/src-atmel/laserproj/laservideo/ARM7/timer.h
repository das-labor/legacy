extern volatile unsigned long timeval;

void __attribute__ ((interrupt("IRQ"))) tmr0_interrupt(void);

void __attribute__ ((interrupt("IRQ"))) rtc_interrupt(void);

void __attribute__ ((interrupt("IRQ"))) tmr1_interrupt(void);
