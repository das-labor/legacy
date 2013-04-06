#include <stdint.h>

#include "netvar/netvar.h"
#include "netvar/netvar_io.h"
#include "i2c_com.h"
#include "../include/PowerCommander.h"

enum {
	LAMP_OUT_TAFEL,
	LAMP_OUT_BEAMER,
	LAMP_OUT_SCHRANK,
	LAMP_OUT_FLIPPER,
	LAMP_OUT_KUECHE
};

static void lamp_out(void *num, uint8_t val) {
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
		case LAMP_OUT_KUECHE:
			set_pwm   (PWM_KUECHE, val);
			set_output(SWL_KUECHE, val);
			break;
	}
}

#define NV_IDX_LAMP_CONTROLLER_VORTRAG 0x0100
#define NV_IDX_LAMP_CONTROLLER_KUECHE  0x0102

void lamp_out_init() {
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x00, lamp_out, (void *) LAMP_OUT_TAFEL);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x01, lamp_out, (void *) LAMP_OUT_BEAMER);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x02, lamp_out, (void *) LAMP_OUT_SCHRANK);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_VORTRAG, 0x03, lamp_out, (void *) LAMP_OUT_FLIPPER);
	new_netvar_output_8(NV_IDX_LAMP_CONTROLLER_KUECHE,  0x00, lamp_out, (void *) LAMP_OUT_KUECHE);
}

