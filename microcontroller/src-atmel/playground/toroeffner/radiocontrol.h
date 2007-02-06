#ifndef RADIOCONTROL_H_
#define RADIOCONTROL_H_
#ifndef RADIO_QUEUESIZE
	// size of queue in bytes.
	#define RADIO_QUEUESIZE 8
#endif

#ifndef RADIO_TX_PORT
	#define RADIO_TX_PORT PORTD
#endif
#ifndef RADIO_TX_BIT
	#define RADIO_TX_BIT _BV(PD6)
#endif

// value for output compare match. see ISR in radiocontrol.c
#ifndef RADIO_SEND_TIME
	#define RADIO_SEND_TIME 50  /* one third of real send time */
//	#define RADIO_SEND_TIME 0x20  /* one third of real send time */
#endif
// how many ms * 10 to wait before radio_tx returns an error
#ifndef RADIO_TIMEOUT
	#define RADIO_TIMEOUT 15
#endif
#define RADIO_TIMER_ENABLE() 


/* ___ END OF CONFIG ___ */
#if ( RADIO_QUEUESIZE > 32 )
	#warning RADIO_QUESIZE _must not_ exceed 32 bytes -- set to 32.
	#define RADIO_QUEUESIZE 32
#endif

void radio_init ( void );
uint8_t radio_tx ( uint8_t *in_str );

#endif
