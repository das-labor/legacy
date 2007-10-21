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

#include "config.h"


#include "static_scripts.h"
#include "pwm.h"
#include <stdint.h>
#include <avr/pgmspace.h>


/* local prototypes */
void sleep_handler(struct thread_t *current_thread);
void wait_handler(struct thread_t *current_thread);

/* opcode handlers */
/* {{{ */
uint8_t opcode_handler_nop(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_fade_channel(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_fade_channels(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_jump(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_set_channel(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_sleep(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_wait(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_clear(uint8_t parameters[], struct thread_t *current_thread);
uint8_t opcode_handler_stop(uint8_t parameters[], struct thread_t *current_thread);
/* }}} */


/* opcode lookup table */
/* {{{ */
uint8_t (*opcode_lookup_table[])(uint8_t parameters[], struct thread_t *current_thread) = {
    &opcode_handler_nop,            /* opcode 0x00 */
    &opcode_handler_fade_channel,   /* opcode 0x10 */
    &opcode_handler_fade_channels,  /* opcode 0x20 */
    &opcode_handler_jump,           /* opcode 0x30 */
    &opcode_handler_set_channel,    /* opcode 0x40 */
    &opcode_handler_sleep,          /* opcode 0x50 */
    &opcode_handler_wait,           /* opcode 0x60 */
    &opcode_handler_clear,          /* opcode 0x70 */
    &opcode_handler_stop            /* opcode 0x80 */
        };
/* }}} */


/* init all structures in the global array 'script_threads' */
void init_script_threads(void)
/* {{{ */ {
    uint8_t i;

    /* iterate over all threads */
    for (i=0; i<MAX_THREADS; i++) {
        script_threads[i].handler.execute = 0;
        script_threads[i].handler.position = 0;

        script_threads[i].flags.channel_target_reached = 0;
        script_threads[i].flags.disabled = 1;

        script_threads[i].handler_stack_offset = 0;

#if SCRIPT_SPEED_CONTROL
        script_threads[i].speed_adjustment = 0;
#endif
    }
}
/* }}} */

/* iterate over all threads and execute each, if enabled */
void execute_script_threads(void)
/* {{{ */ {
    uint8_t i;
    uint8_t target_mask = 0;

    /* check for channels which reached their targets */
    for (i=0; i<PWM_CHANNELS; i++) {
        if (global_pwm.channels[i].flags.target_reached) {
            target_mask |= global_pwm.channels[i].mask;
            global_pwm.channels[i].flags.target_reached = 0;
        }
    }

    /* iterate over all threads */
    for (i=0; i<MAX_THREADS; i++) {
        /* update channel target reached mask */
        script_threads[i].flags.channel_target_reached |= target_mask;

        /* execute enabled threads */
        if (!script_threads[i].flags.disabled) {
            script_threads[i].handler.execute(&script_threads[i]);
        }
    }
}
/* }}} */

/* memory handlers */
void memory_handler_flash(struct thread_t *current_thread)
/* {{{ */ {
    uint8_t opcode;
    uint8_t parameters[4];
    uint8_t i;

    while (1) {
        /* read opcode and parameters and call the appropiate opcode processing function */
        opcode = pgm_read_byte((current_thread->handler.position)++);

        /* safe flags as first parameter */
        parameters[0] = (opcode & 0x0f);

        /* extract real opcode */
        opcode >>= 4;

        /* load other parameters */
        for (i=1; i<4; i++) {
            parameters[i] = pgm_read_byte((current_thread->handler.position)++);
        }

        /* call opcode handler */
        i = (opcode_lookup_table[opcode])(parameters, current_thread);

        if (i == OP_RETURN_STOP) {
            (*current_thread).flags.disabled = 1;
            break;
        } else if (i == OP_RETURN_BREAK) {
            break;
        }
    }
}
/* }}} */

void memory_handler_eeprom(struct thread_t *current_thread)
/* {{{ */ {
    uint8_t opcode;
    uint8_t parameters[4];
    uint8_t i;

    while (1) {
        /* read opcode and parameters and call the appropiate opcode processing function */
        opcode = eeprom_read_byte((uint8_t *)(current_thread->handler.position)++);

        /* safe flags as first parameter */
        parameters[0] = (opcode & 0x0f);

        /* extract real opcode */
        opcode >>= 4;

        /* load other parameters */
        for (i=1; i<4; i++) {
            parameters[i] = eeprom_read_byte((uint8_t *)(current_thread->handler.position)++);
        }

        /* call opcode handler */
        i = (opcode_lookup_table[opcode])(parameters, current_thread);

        if (i == OP_RETURN_STOP) {
            (*current_thread).flags.disabled = 1;
            break;
        } else if (i == OP_RETURN_BREAK) {
            break;
        }
    }
}
/* }}} */

/* other (module-local) handlers */
void sleep_handler(struct thread_t *current_thread)
/* {{{ */ {
    /* restore old handler, if time is over */
    if ((--current_thread->handler.position) == 0) {
        current_thread->handler_stack_offset--;
        current_thread->handler.execute = current_thread->handler_stack[current_thread->handler_stack_offset].execute;
        current_thread->handler.position = current_thread->handler_stack[current_thread->handler_stack_offset].position;
    }

}
/* }}} */

void wait_handler(struct thread_t *current_thread)
/* {{{ */ {
    /* restore old handler and clear target_reached mask, if a channel we wait
     * for has reached it's target */

    if (current_thread->handler.position & current_thread->flags.channel_target_reached) {
        /* clear bitmask */
        current_thread->flags.channel_target_reached &= ~current_thread->handler.position;

        /* restore old handler */
        current_thread->handler_stack_offset--;
        current_thread->handler.execute = current_thread->handler_stack[current_thread->handler_stack_offset].execute;
        current_thread->handler.position = current_thread->handler_stack[current_thread->handler_stack_offset].position;
    }

}
/* }}} */


/* opcode handlers */
uint8_t opcode_handler_nop(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    /* do exactly nothing */
    (void) parameters;
    (void) current_thread;

    return OP_RETURN_BREAK;
}
/* }}} */

uint8_t opcode_handler_fade_channel(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    uint16_t speed = (parameters[3] << 8) + parameters[2];

#if SCRIPT_SPEED_CONTROL
    if ((current_thread->speed_adjustment) < 0) {
        speed >>= -current_thread->speed_adjustment;
        if (speed == 0)
            speed = 1;
    } else if (current_thread->speed_adjustment > 0) {
        speed <<= current_thread->speed_adjustment;
        if (speed == 0)
            speed = 0xff;
    }
#else
    (void) current_thread;
#endif

    global_pwm.channels[parameters[0]].target_brightness = parameters[1];
    global_pwm.channels[parameters[0]].speed_l = LOW(speed);
    global_pwm.channels[parameters[0]].speed_h = HIGH(speed);

    return OP_RETURN_OK;
}
/* }}} */

uint8_t opcode_handler_fade_channels(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    (void) current_thread;

    global_pwm.channels[0].target_brightness = parameters[1];
    global_pwm.channels[1].target_brightness = parameters[2];
    global_pwm.channels[2].target_brightness = parameters[3];

    return OP_RETURN_OK;
}
/* }}} */

uint8_t opcode_handler_jump(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    int16_t offset = ((int8_t) parameters[1]);

    /* correct offset so that -1 really jumps to the instruction _before_ this one */
    if (offset < 0)
        offset--;

    /* comput the byte offset */
    offset *= 4;

    /* save new offset */
    current_thread->handler.position += offset;

    return OP_RETURN_OK;
}
/* }}} */

uint8_t opcode_handler_set_channel(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    (void) current_thread;

    global_pwm.channels[parameters[1]].brightness = parameters[2];
    global_pwm.channels[parameters[1]].target_brightness = parameters[2];

    return OP_RETURN_OK;
}
/* }}} */

uint8_t opcode_handler_sleep(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    uint16_t speed = (parameters[2] << 8) + parameters[1];

#if SCRIPT_SPEED_CONTROL
    if ((current_thread->speed_adjustment) < 0) {
        speed <<= current_thread->speed_adjustment;
        if (speed == 0)
            speed = 0xff;
    } else if (current_thread->speed_adjustment > 0) {
        speed >>= -current_thread->speed_adjustment;
        if (speed == 0)
            speed = 1;
    }
#endif

    /* save old handler and old position onto the handler stack in the current thread */
    current_thread->handler_stack[current_thread->handler_stack_offset].execute = current_thread->handler.execute;
    current_thread->handler_stack[current_thread->handler_stack_offset].position = current_thread->handler.position;

    current_thread->handler.execute = &sleep_handler;
    current_thread->handler.position = speed;

    current_thread->handler_stack_offset++;

    return OP_RETURN_BREAK;
}
/* }}} */

uint8_t opcode_handler_wait(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    /* save old handler and old position onto the handler stack in the current thread */
    current_thread->handler_stack[current_thread->handler_stack_offset].execute = current_thread->handler.execute;
    current_thread->handler_stack[current_thread->handler_stack_offset].position = current_thread->handler.position;

    current_thread->handler.execute = &wait_handler;
    current_thread->handler.position = parameters[1];

    current_thread->handler_stack_offset++;

    return OP_RETURN_BREAK;
}
/* }}} */

uint8_t opcode_handler_clear(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    (void) parameters;

    /* clear channel target reached flags for this thread */
    current_thread->flags.channel_target_reached = 0;

    return OP_RETURN_OK;
}
/* }}} */

uint8_t opcode_handler_stop(uint8_t parameters[], struct thread_t *current_thread)
/* {{{ */ {
    (void) current_thread;
    (void) parameters;

    return OP_RETURN_STOP;
}
/* }}} */



