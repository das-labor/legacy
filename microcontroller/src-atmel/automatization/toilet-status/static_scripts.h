/* vim:fdm=marker ts=4 et ai
 * {{{
 *         fnordlicht firmware next generation
 *
 *    for additional information please
 *    see http://koeln.ccc.de/prozesse/running/fnordlicht
 *
 * (c) by Alexander Neumann <alexander@bumpern.de>
 *     Lars Noschinski <lars@public.noschinski.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * For more information on the GPL, please go to:
 * http://www.gnu.org/copyleft/gpl.html
 }}} */

#ifndef fnordlicht_static_script_h
#define fnordlicht_static_script_h

#include "config.h"

#include <avr/io.h>



#include "pwm.h"
#include <avr/io.h>
#include <stdint.h>
#include <avr/eeprom.h>

#define MAX_THREAD_STACK_DEPTH 3
#define MAX_THREADS 3

/* opcodes */
/* {{{ */
#define OP_NOP              0x00
#define OP_FADE_CHANNEL     0x10
#define OP_FADE_CHANNELS    0x20
#define OP_JUMP             0x30
#define OP_SET_CHANNEL      0x40
#define OP_SLEEP            0x50
#define OP_WAIT             0x60
#define OP_CLEAR            0x70
#define OP_STOP             0x80
/* }}} */

/* opcode macros */
/* {{{ */
#define MACRO_NOP() \
    OP_NOP, 0, 0, 0

#define MACRO_FADE_CHANNEL(channel, target_brightness, speed) \
    (OP_FADE_CHANNEL | channel), target_brightness, LOW(speed), HIGH(speed)

#define MACRO_FADE_CHANNELS(brightness1, brightness2, brightness3) \
    OP_FADE_CHANNELS, brightness1, brightness2, brightness3

#define MACRO_JUMP(offset) \
    OP_JUMP, offset, 0, 0

#define MACRO_SET_CHANNEL(channel, target_brightness) \
    OP_SET_CHANNEL, channel, target_brightness, 0

#define MACRO_SLEEP(delay_cycles) \
    OP_SLEEP, LOW(delay_cycles), HIGH(delay_cycles), 0

#define MACRO_WAIT(eventmask) \
    OP_WAIT, eventmask, 0, 0

#define MACRO_CLEAR() \
    OP_CLEAR, 0, 0, 0

#define MACRO_STOP() \
    OP_STOP, 0, 0, 0
/* }}} */

/* opcode function return values */
/* {{{ */
#define OP_RETURN_OK        0   /* do nothing */
#define OP_RETURN_BREAK     1   /* execution has been completed for this cycle, jump to next thread */
#define OP_RETURN_STOP      2   /* disable this script */
/* }}} */

/* workaround prototype for handler 'execute' function definition */
struct thread_t;

/* structs */
struct script_handler_t
/* {{{ */ {
    void (*execute)(struct thread_t *current_thread);
    uint16_t position;
}; /* }}} */

struct thread_t
/* {{{ */ {
    struct script_handler_t handler;

    struct {
        uint8_t channel_target_reached:PWM_CHANNELS;    /* these channels reached their target brightness value lately */
        uint8_t disabled:1;                             /* disable execution of this thread */
    } flags;

    struct script_handler_t handler_stack[MAX_THREAD_STACK_DEPTH];
    uint8_t handler_stack_offset;

#if SCRIPT_SPEED_CONTROL
    int8_t speed_adjustment;
#endif
}; /* }}} */

/* global variables */
struct thread_t script_threads[MAX_THREADS];

/* prototypes */
void init_script_threads(void);
void execute_script_threads(void);

/* memory handlers */
void memory_handler_flash(struct thread_t *current_thread);
void memory_handler_eeprom(struct thread_t *current_thread);


#endif

