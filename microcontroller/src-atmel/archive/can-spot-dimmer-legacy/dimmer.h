#define NUM_LAMPS 4

typedef struct{
	unsigned char end_bright[NUM_LAMPS];
	unsigned int delay_rl[NUM_LAMPS];
	unsigned int delay[NUM_LAMPS];
}dimmer_ramp;


void dimmer_init();
extern unsigned char Bright[NUM_LAMPS];
extern dimmer_ramp Ramp;
