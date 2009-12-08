#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "can.h"
#include "can_handler.h"
#include "lap.h"
#include "../lab-irkit.h"

//teufel test code
//volume down = 010 100 010000
uint16_t ir_testTeufel2[] PROGMEM =
{PT_OFF, PT_ON, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_OFF}; 

//teufel test code
//volume up = 010 100 100000
uint16_t ir_testTeufel3[] PROGMEM =
{PT_OFF, PT_ON, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_ON, PT_OFF, PT_OFF, PT_OFF, PT_OFF, PT_OFF}; 

// nec test
// address 16bit          command 8bit - power
//                                  inverted 8bit cmd
// 0001 0000 1100 1000 + 1110 0001 (0001 1110)

uint16_t ir_acer_address[] PROGMEM =
{PNEC_AGC_BURST, PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_OFF, PNEC_OFF, 
 PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_ON, PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_OFF,
 PNEC_OFF, PNEC_OFF};

uint16_t ir_acer_power[] PROGMEM =
{PNEC_ON, PNEC_ON, PNEC_ON, PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_ON,
 PNEC_OFF, PNEC_OFF, PNEC_OFF, PNEC_ON, PNEC_ON, PNEC_ON, PNEC_ON, PNEC_OFF,
 PNEC_OFF};




