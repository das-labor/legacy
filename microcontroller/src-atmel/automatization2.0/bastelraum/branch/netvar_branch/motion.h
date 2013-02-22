
#pragma once

/* treshold value for motion detection (~3mV) */
#define MOTION_TRESHOLD 20

/* how many ticks to ignore in an interval of 2 minutes */
#define MOTION_NUM_IGNORE 4

extern void motion_init(void);
extern void motion_tick(void);

/* predefined ADC initialization values for motion detectors */
#define NUM_DETECTORS 2

/* pre-defined bits for ADMUX: 1.1V reference voltage */
#define ADMUX_PREDEF (_BV(REFS1))

#define M_IDLE_TRESHOLD 4   /* warning after n * 2 minutes */
#define M_OFF_TRESHOLD 1    /* shut off power n * 2 minutes after idle limit is reached */
#define M_WARN_LENGTH 10    /* length of warning period:  n * 0.5s -> 5s */
