/*
 * {{{
 *    uses parts of fnordlicht firmware next generation
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
#ifndef PWM_H
#define PWM_H

void pwm_handler();



#define _TIMSK_TIMER1 TIMSK

/* color <-> channel assignment */
#define CHANNEL_RED     0
#define CHANNEL_GREEN   1
#define CHANNEL_BLUE    2

#define PWM_CHANNELS 3

/* macros for extracting low and high byte */
#define LOW(x) (uint8_t)(0x00ff & (x))
#define HIGH(x) (uint8_t)((0xff00 & (x)) >> 8)

struct flags_t {
    /* set by pwm interrupt after burst, signals the beginning of a new pwm
     * cycle to the main loop. */
    uint8_t new_cycle:1;
    /* set by pwm interrupt after last interrupt in the current cycle, signals
     * the main loop to rebuild the pwm timslot table */
    uint8_t last_pulse:1;
};

struct global_t {
    struct flags_t flags;
};

extern volatile struct global_t global;


/* possible pwm interrupts in a pwm cycle */
#define PWM_MAX_TIMESLOTS (PWM_CHANNELS+1)

/* contains all the data for one color channel */
struct channel_t
/*{{{*/ {
    union {
        /* for adding fade-speed to brightness, and save the remainder */
        uint16_t brightness_and_remainder;

        /* for accessing brightness directly */
        struct {
            uint8_t remainder;
            uint8_t brightness;
        };
    };

    /* desired brightness for this channel */
    uint8_t target_brightness;

    /* fade speed, the msb is added directly to brightness,
     * the lsb is added to the remainder until an overflow happens */
    union {
        /* for accessing speed as an uint16_t */
        uint16_t speed;

        /* for accessing lsb und msb directly */
        struct {
            uint8_t speed_l;
            uint8_t speed_h;
        };
    };

    /* output mask for switching on the leds for this channel */
    uint8_t mask;

    /* flags for this channel, implemented as a bitvector field */
    struct {
        /* this channel reached has recently reached it's desired target brightness */
        uint8_t target_reached:1;
    } flags;

}; /*}}}*/

struct global_pwm_t {
    /* current channel records */
    struct channel_t channels[3];
};

extern volatile struct global_pwm_t global_pwm;

/* prototypes */
void init_timer1(void);
void init_pwm(void);
void update_pwm_timeslots(void);
void update_brightness(void);
#endif
