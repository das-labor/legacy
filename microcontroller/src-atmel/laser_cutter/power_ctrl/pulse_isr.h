
#define TICK_CHARGE_START                          1
#define TICK_END_OF_CHARGE  (TICK_CHARGE_START  + 30)
#define TICK_CHARGE_STOP    (TICK_END_OF_CHARGE +  2)
#define TICK_PULSE_ON       (TICK_CHARGE_STOP   +  2)
#define TICK_PULSE_OFF      (TICK_PULSE_ON      +  1)

#define TICK_SEQUENCE_COUNT (TICK_PULSE_OFF     +  1)

#define MIN_FIRE_PERIOD 49  /*50ms = 20Hz*/


void init_pulse(void);
