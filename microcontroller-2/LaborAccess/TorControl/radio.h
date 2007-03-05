#ifndef RADIOCONTROL_H_
#define RADIOCONTROL_H_

#ifndef RADIO_TX_PORT
	#define RADIO_TX_PORT PORTD
#endif
#ifndef RADIO_TX_BIT
	#define RADIO_TX_BIT _BV(PD0)
#endif

// Number of outstanding Tor-Open commands
extern uint8_t radio_txcount;

// to be called at 2500 Hz 
void radio_tick ( void );

AVRX_EXTERNTASK(laptask);

#endif
