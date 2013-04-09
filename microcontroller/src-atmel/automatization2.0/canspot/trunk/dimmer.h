#define NUM_LAMPS 4

typedef struct {
	uint8_t end_bright[NUM_LAMPS];
	uint16_t delay_rl[NUM_LAMPS];
	uint16_t delay[NUM_LAMPS];
} dimmer_ramp;


extern void dimmer_init(void);
extern uint8_t Bright[NUM_LAMPS];
extern dimmer_ramp Ramp;

