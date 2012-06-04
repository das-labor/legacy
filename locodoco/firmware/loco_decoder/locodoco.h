/* locodoco packet handling functions
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "config.h"
#include "../../common/protocol.h"
#include "rfm12.h"
#include "motor.h"

#pragma once



void ldc_packet_handler (void);
