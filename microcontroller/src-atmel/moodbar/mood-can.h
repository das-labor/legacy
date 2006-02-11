#ifndef MOOD_CAN_H
#define MOOD_CAN_H

void mcan_init();
void mcan_process_messages();

typedef struct{
	uint16_t zeroval;
	uint16_t minval;
	uint16_t maxval;
	uint16_t preheat;
	int8_t cal;
}pl_param_t;


#endif  // MOOD_CAN_H
