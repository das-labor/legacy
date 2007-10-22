#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"
#include "uart.h"

#ifdef RF_USE_AVRX
	#include "avrx.h"
#endif

extern uint16_t rf12_trans(uint16_t wert);	// transfer 1 word to/from module
extern void rf12_init(void);											// initialize module
extern void rf12_setfreq(unsigned short freq);							// set center frequency
extern void rf12_setbaud(unsigned short baud);							// set baudrate
extern void rf12_setpower(unsigned char power, unsigned char mod);		// set transmission settings
extern void rf12_setbandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi);	// set receiver settings
extern void rf12_txdata(uint8_t *data, uint8_t in_num);


extern uint8_t rf12_rxbyte();
uint8_t rf12_ready_timeout (uint8_t in_waitms);
extern uint8_t rf12_rxarray ( uint8_t *target, uint8_t maxlen );

#define RF12FREQ(freq)	((freq-430.0)/0.0025)							// macro for calculating frequency value out of frequency in MHz

#ifndef rf_cbi
	#define rf_cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#endif
#ifndef rf_sbi
	#define rf_sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))  
#endif
