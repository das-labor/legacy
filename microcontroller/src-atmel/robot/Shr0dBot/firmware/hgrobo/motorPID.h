#ifndef _MOTORPID_H
#define _MOTORPID_H

/******************
*
*	VARIOUS INCLUDES
*
*******************/

#include <inttypes.h>


/******************
*
*	VARIOUS DECLARATIONS
*
*******************/

//this struct holds all motor pid variables
typedef struct
{
	int16_t dState;		//last error
	int16_t iState;		//current integrator state
	uint8_t pGain;    	//proportional gain
	uint8_t iGain;    	//integral gain
	uint8_t dGain;     	//derivative gain
} MPID_State;


/******************
*
*	FUNCTION PROTOTYPES
*
*******************/

//Motor PID function
int16_t MotorPID(MPID_State *state, int16_t error, int16_t value);

#endif
