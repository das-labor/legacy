#ifndef _ENCODER_H
#define _ENCODER_H

/******************
*
*	VARIOUS INCLUDES
*
*******************/

#include "hgrobo_config.h"


/******************
*
*	GLOBAL ENCODER COUNTERS
*
*******************/

extern int32_t enc_A;
#ifdef ENC_B_ENABLE
extern int32_t enc_B;
#endif


/******************
*
*	FUNCTION PROTOTYPES
*
*******************/

void enc_sample();		//sample encoders
void enc_init();		//init encoders

#endif
