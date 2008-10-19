#include <avr/io.h>
#include "hgrobo_config.h"

/******************
*
*	GLOBAL ENCODER COUNTERS
*
*******************/

int32_t enc_A;
#ifdef ENC_B_ENABLE
int32_t enc_B;
#endif


/******************
*
*	ENCODER IMPLEMENTATION
*
*******************/

//sample the encoder pins
void enc_sample()
{
	static uint8_t lastSample;
	uint8_t tmp, sample;

	//sample port
	sample = ENC_PORT_IN;

	//ENCODER A

#ifdef ENC_A_SAMPLE
	//4x decoding
	tmp = ((((lastSample & _BV(ENC_A_PHSA)) >> ENC_A_PHSA) ^ ((sample & _BV(ENC_A_PHSB)) >> ENC_A_PHSB)) << 1) | (((lastSample & _BV(ENC_A_PHSB)) >> ENC_A_PHSB) ^ ((sample & _BV(ENC_A_PHSA)) >> ENC_A_PHSA));
	//forward motion
	if(tmp == 1)
	{
		enc_A++;
	}
	//backwards
	else if(tmp == 2)
	{
		enc_A--;
	}
#endif


	//ENCODER B

#ifdef ENC_B_ENABLE
#ifdef ENC_B_SAMPLE
	//4x decoding
	tmp = (((lastSample & _BV(ENC_B_PHSA)) >> ENC_B_PHSA) ^ ((sample & _BV(ENC_B_PHSB)) >> ENC_B_PHSB) << 1) | (((lastSample & _BV(ENC_B_PHSB)) >> ENC_B_PHSB) ^ ((sample & _BV(ENC_B_PHSA)) >> ENC_B_PHSA));
	if(tmp == 1)
	{
		enc_B++;
	}
	else if(tmp == 2)
	{
		enc_B--;
	}
	#endif
#endif

	//save last sample
	lastSample = sample;
}


//encoder initialization
void enc_init()
{
	//set encoder pins as input
	ENC_PORT_DDR &= ~(_BV(ENC_A_PHSA) | _BV(ENC_A_PHSB) | _BV(ENC_B_PHSA) | _BV(ENC_B_PHSB));

	//disable pull-ups
	ENC_PORT_OUT &= ~3;

	//sample once (init lastSample before resetting counts)
	enc_sample();

	//reset count
	enc_A = 0; enc_B = 0;
}