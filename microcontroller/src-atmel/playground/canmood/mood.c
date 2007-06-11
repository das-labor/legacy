#include <avr/io.h>
#include <string.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "xlap.h"
#include "pwm.h"

TimerControlBlock switchtimer2;
uint8_t mprog = 1;

AVRX_GCC_TASKDEF(mood, 20, 4) {
	uint8_t i;
	while(1) {
		for (i=0; i<3; i++) {
			global_pwm.channels[i].speed = 0x00100;
		}
		global_pwm.channels[0].target_brightness = 255;
		global_pwm.channels[1].target_brightness = 0;
		global_pwm.channels[2].target_brightness = 0;
		AvrXDelay(&switchtimer2, 10000);
		global_pwm.channels[0].target_brightness = 255;
		global_pwm.channels[1].target_brightness = 255;
		global_pwm.channels[2].target_brightness = 0;
		AvrXDelay(&switchtimer2, 10000);
		global_pwm.channels[0].target_brightness = 0;
		global_pwm.channels[1].target_brightness = 255;
		global_pwm.channels[2].target_brightness = 0;
		AvrXDelay(&switchtimer2, 10000);
		global_pwm.channels[0].target_brightness = 0;
		global_pwm.channels[1].target_brightness = 255;
		global_pwm.channels[2].target_brightness = 255;
		AvrXDelay(&switchtimer2, 10000);
		global_pwm.channels[0].target_brightness = 0;
		global_pwm.channels[1].target_brightness = 0;
		global_pwm.channels[2].target_brightness = 255;
		AvrXDelay(&switchtimer2, 10000);
		global_pwm.channels[0].target_brightness = 255;
		global_pwm.channels[1].target_brightness = 0;
		global_pwm.channels[2].target_brightness = 255;
		AvrXDelay(&switchtimer2, 10000);
	}
}
