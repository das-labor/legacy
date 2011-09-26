
//Atmega port definitions

#define LED_READY PB7
#define LED_CALIBRATING PC6
#define LED_USBON PF4
#define LED_RUNNING PF5


#define LED_READY_ON {PORTB |= _BV(LED_READY);}
#define LED_READY_OFF {PORTB &= ~ _BV(LED_READY);}
#define CONFIGURE_LED_READY DDRD |= _BV(LED_READY);

#define LED_CALIBRATING_ON {PORTC |= _BV(LED_CALIBRATING);}
#define LED_CALIBRATING_OFF {PORTC &= ~ _BV(LED_CALIBRATING);}
#define CONFIGURE_LED_CALIBRATING DDRC |= _BV(LED_CALIBRATING);

#define LED_USBON_ON {PORTF |= _BV(LED_USBON);}
#define LED_USBON_OFF {PORTF &= ~ _BV(LED_USBON);}
#define CONFIGURE_LED_USBON DDRF |= _BV(LED_USBON);

#define LED_RUNNING_ON {PORTF |= _BV(LED_RUNNING);}
#define LED_RUNNING_OFF {PORTF &= ~ _BV(LED_RUNNING);}
#define CONFIGURE_LED_RUNNING DDRF |= _BV(LED_RUNNING);

#define LASER_SUPPLY_EN PF0
#define LASER_SUPPLY_ON  {PORTF |= _BV(LASER_SUPPLY_EN);}
#define LASER_SUPPLY_OFF {PORTF &= ~_BV(LASER_SUPPLY_EN);}

#define KEY_ON PD0
#define KEY_START PD1

#define CONFIGURE_KEY_ON PORTD|=_BV(PD0);
#define CONFIGURE_KEY_START PORTD|=_BV(PD1);

#define KEY_ON_PIN (PIND & _BV(KEY_ON))
#define KEY_START_PIN (PIND & _BV(KEY_START))

#define SUPPLY_SENSE PD3
#define SUPPLY_SENSE_PIN (PIND & _BV(PD3))

#define IH1 PB5
#define IL1 PF7
#define IH2 PB6
#define IL2 PD5

#define HSYNC PD2

#define ENABLE_INT3 EIMSK |=_BV(INT3);
#define DISABLE_INT3 EIMSK &=~BV(INT3);
#define CONFIGURE_INT3 EICRA|=_BV(ISC30);	//any edge

#define ENABLE_INT2 EIMSK |=_BV(INT2);
#define DISABLE_INT2 EIMSK &=~BV(INT2);
#define CONFIGURE_INT2 EICRA|=_BV(ISC21);	//falling edge
 
#define ENABLE_INT0 EIMSK |=_BV(INT0);
#define DISABLE_INT0 EIMSK &=~BV(INT0);
#define CONFIGURE_INT0 EICRA|=_BV(ISC01);	//falling edge

#define ENABLE_INT1 EIMSK |=_BV(INT1);
#define DISABLE_INT1 EIMSK &=~BV(INT1);
#define CONFIGURE_INT1 EICRA|=_BV(ISC11);	//falling edge
 
#define ENABLE_INT6 EIMSK |=_BV(INT6);
#define DISABLE_INT6 EIMSK &=~BV(INT6);
#define CONFIGURE_INT6 EICRB|=_BV(ISC61);		//falling edge


#define ENABLE_USB_INT UDIEN = (1<<EORSTE)|(1<<SOFE);
#define DISABLE_USB_INT UDIEN &= ~((1<<EORSTE)|(1<<SOFE));

#define POWER_DOWN {SMCR |= _BV(SM1)|_BV(SE);SLEEP;}
#define POWER_REDUCE {PRR0 = _BV(PRTWI)|_BV(PRADC); PRR1 = _BV(PRUSART1);}

#define ROT1_PIN (EIFR & _BV(INTF6))
#define CLEAR_ROT1_PIN {EIFR &= ~_BV(INTF6);}

#define HSYNC_PIN (EIFR & _BV(INTF2))
#define CLEAR_HSYNC_PIN {EIFR &= ~_BV(INTF6);}

//hardware definition
#define MASQ0MB11RR_MAX_FREQ 200

#define EXPOSER_OFF 0
#define EXPOSER_RUNNING _BV(1)
#define EXPOSER_ON _BV(2)
#define EXPOSER_USB_GO _BV(4)

#define EXPOSER_IS_ON (exposer.status & EXPOSER_ON)
#define EXPOSER_IS_RUNNING (exposer.status & EXPOSER_RUNNING)

#define EXPOSER_BUFFER_SIZE 512

typedef struct{
	uint8_t buffer[EXPOSER_BUFFER_SIZE];
	uint16_t bitpointer;	//points to one bit in the buffer
	uint8_t *bufpointer;
}exposer_buffer_t;

	
typedef struct{
	uint8_t status;
	uint16_t currentline;	//TODO: uint32_t ??
	uint16_t endline;
	uint16_t fastforwardspeed;
	uint16_t plotstartpos;
	uint8_t linesperrotaryenctick;
	uint16_t plotforwardspeed;
	uint16_t backspeed;
	exposer_buffer_t bufferA;
	exposer_buffer_t bufferB;
} exposer_t;


extern exposer_t exposer;