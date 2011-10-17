#include <avr/io.h>
#include "hgrobo_config.h"

/******************
*
*	FUNCTION PROTOTYPES
*
*******************/

/*static inline void mot2_enable();
static inline void mot2_disable();
*/
static inline void mot2_fwd();
static inline void mot2_rwd();
static inline void mot2_brk();
static inline void mot2_dty(uint16_t dty);

static inline void mot1_fwd();
static inline void mot1_rwd();
static inline void mot1_brk();
static inline void mot1_dty(uint16_t dty);

void mot_init(void);
void pwm_init(void);


/******************
*
*	MOTOR 2 IMPLEMENTATION
*
*******************/

/* brake motor stuff
//turn on motor 2 power
static inline void mot2_enable()
{
	MOT_PORT_OUT |= _BV(MOT_PIN_ENA2);
}

//turn off motor 2 power
static inline void mot2_disable()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_ENA2));
}
*/

//motor 2 forward
static inline void mot2_fwd()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_RWD2));
	MOT_PORT_OUT |= _BV(MOT_PIN_FWD2);
}

//motor 2 back
static inline void mot2_rwd()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_FWD2));
	MOT_PORT_OUT |= _BV(MOT_PIN_RWD2);
}


//motor 2 brake
static inline void mot2_brk()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_RWD2) | _BV(MOT_PIN_FWD2));
}

//set motor 2 dutycycle
static inline void mot2_dty(uint16_t dty)
{
	OCR1B = dty;
}


/******************
*
*	MOTOR 1 IMPLEMENTATION
*
*******************/

//motor 1 forward
static inline void mot1_fwd()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_RWD1));
	MOT_PORT_OUT |= _BV(MOT_PIN_FWD1);
}

//motor 1 back
static inline void mot1_rwd()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_FWD1));
	MOT_PORT_OUT |= _BV(MOT_PIN_RWD1);
}

//motor 1 brake
static inline void mot1_brk()
{
	MOT_PORT_OUT &= ~(_BV(MOT_PIN_RWD1) | _BV(MOT_PIN_FWD1));
}

//set motor 1 dutycycle
static inline void mot1_dty(uint16_t dty)
{
	OCR1A = dty;
}
