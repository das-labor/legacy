//carriage_driver.h
//used hardware:
//* Timer1 PWM (OC1A, OC1B)

//bts7741g
#define bts7741g_IH1 PB5
#define bts7741g_IH2 PB6
#define bts7741g_IL1 PF7
#define bts7741g_IL2 PD5

//TODO: OC - LOW
#define bts7741g_IH1_LOW {PORTB &= ~_BV(bts7741g_IH1);}
#define bts7741g_IH2_LOW {PORTB &= ~_BV(bts7741g_IH2);}
#define bts7741g_IL1_LOW {PORTF &= ~_BV(bts7741g_IL1);}
#define bts7741g_IL2_LOW {PORTD &= ~_BV(bts7741g_IL2);}

#define bts7741g_IH1_HIGH {PORTB |= _BV(bts7741g_IH1);}
#define bts7741g_IH2_HIGH {PORTB |= _BV(bts7741g_IH2);}
#define bts7741g_IL1_HIGH {PORTF |= _BV(bts7741g_IL1);}
#define bts7741g_IL2_HIGH {PORTD |= _BV(bts7741g_IL2);}

//IH1 <- OC1A
//IH2 <- OC1B


void bts7741g_stop()

void bts7741g_freerunning()

//if !speed stop condition is called
void bts7741g_forward(uint16_t speed)

//if !speed stop condition is called
void bts7741g_backwards(uint16_t speed)

//Timer1
void timer1_init()
