

#include <stdint.h>
#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "i2c_com.h"
#include "../include/PowerCommander.h"

#define LAMP_OUT_TAFEL   0
#define LAMP_OUT_BEAMER  1
#define LAMP_OUT_SCHRANK 2
#define LAMP_OUT_FLIPPER 3

static void lamp_out(void * num, uint8_t val) {
	uint8_t i = (uint8_t) (uint16_t) num;
	switch (i) {
		case LAMP_OUT_TAFEL:
			set_pwm   (PWM_TAFEL, val);
			set_output(SWL_TAFEL, val);
			break;
		case LAMP_OUT_BEAMER:
			set_pwm   (PWM_BEAMER, val);
			set_output(SWL_BEAMER, val);
			break;
		case LAMP_OUT_SCHRANK:
			set_pwm   (PWM_SCHRANK, val);
			set_output(SWL_SCHRANK, val);
			break;
		case LAMP_OUT_FLIPPER:
			set_pwm   (PWM_FLIPPER, val);
			set_output(SWL_FLIPPER, val);
			break;
	}
}

#define NV_IDX_LAMP_CONTROLLER_VORTRAG 0x0100

void lamp_out_init () {
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x50, lamp_out, (void*)LAMP_OUT_TAFEL);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x51, lamp_out, (void*)LAMP_OUT_BEAMER);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x52, lamp_out, (void*)LAMP_OUT_SCHRANK);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x53, lamp_out, (void*)LAMP_OUT_FLIPPER);
}
