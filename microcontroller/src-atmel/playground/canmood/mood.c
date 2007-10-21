#include <avr/io.h>
#include <string.h>

#include <avrx-io.h>
#include <avrx-signal.h>
#include "avrx.h"               // AvrX System calls/data structures

#include "xcan.h"
#include "xlap.h"
#include "pwm.h"

#include "testscript.h"

AVRX_GCC_TASKDEF(mood, 20, 4) {

}
