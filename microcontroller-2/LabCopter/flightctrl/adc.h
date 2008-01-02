#define INT_ADC_OFF ADCSRA = 0
#define INT_ADC_ON ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADIE)
