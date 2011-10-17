#ifndef _HGROBO_CONFIG_H
#define _HGROBO_CONFIG_H

/******************
*
*	MOTOR
*
*******************/

//motor port
#define MOT_PORT_DDR        DDRC
#define MOT_PORT_OUT		PORTC

//motor pins
#define MOT_PIN_FWD1		5
#define MOT_PIN_RWD1		6
#define MOT_PIN_FWD2		2
#define MOT_PIN_RWD2		3
#define MOT_PIN_ENA2		4


/******************
*
*	PWM
*
*******************/

//pwm direction port
#define PWM_PORT_DDR		DDRD

//pwm pins
#define PWM_PIN_A			5
#define PWM_PIN_B			4


/******************
*
*	ENCODERS
*
*******************/

//enable/disable encoder B
//this strips encoder B code from the source,
//in case only one encoder is connected
#define ENC_B_ENABLE		0

//sample encoder A/B in enc_sample()
//this allows for manually sampling one encoder,
//while using an edge trigger int on the other
//(edge trigger ints are not implemented yet)
#define ENC_A_SAMPLE		1
#define ENC_B_SAMPLE		0

//encoder port
#define ENC_PORT_DDR		DDRB
#define ENC_PORT_IN			PINB
#define ENC_PORT_OUT		PORTB

//encoder pins
#define ENC_A_PHSA			1
#define ENC_A_PHSB			0
#define ENC_B_PHSA			2
#define ENC_B_PHSB			3


/******************
*
*	MOTOR PID
*
*******************/

//process values
#define MPID_IMAX	255
#define MPID_IMIN	-255
#define MPID_ISCALE	64


#endif
