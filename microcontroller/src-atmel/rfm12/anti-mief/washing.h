void wstuff_init ();
void wstuff_custom ();
void wstuff_query ();
void wstuff_dec();
void wstuff_inc();
void wstuff_tick(volatile uint16_t);

#define WSTATE_INIT          0x00
#define WSTATE_SET_REPEAT    0x01
#define WSTATE_SET_RUNTIME   0x02
#define WSTATE_SET_WAITDELAY 0x03
#define WSTATE_DONE          0x04

#define WSTATE_RUNNING       0x05
#define WSTATE_WAITING       0x06
