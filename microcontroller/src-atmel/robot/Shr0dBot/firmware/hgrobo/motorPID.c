#include "motorPID.h"
#include "hgrobo_config.h"

//Motor PID function
int16_t MotorPID(MPID_State *state, int16_t error, int16_t value)
{
	int16_t dTerm;

	//calculate the integral state
	state->iState += error * state->iGain;

	//limit the integral state
	if (state->iState > (MPID_IMAX * MPID_ISCALE)) state->iState = MPID_IMAX * MPID_ISCALE;
	else if (state->iState < (MPID_IMIN * MPID_ISCALE)) state->iState = MPID_IMIN * MPID_ISCALE;

	//calculate the derivative term
	dTerm = state->dGain * (int16_t)(state->dState - error);
	state->dState = error;

	//sum up all terms and return the output value
	return (
		(int16_t)(error * state->pGain) +			//calculate the proportional term
		(state->iState / MPID_ISCALE) -				//calculate and add the integral term
		dTerm);										//subtract the derivative term
}