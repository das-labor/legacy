
#include <avr/io.h>


#define NUM_LEDS 10

uint8_t bright[NUM_LEDS];

typedef struct{
	uint16_t mask;
	uint8_t bright;
}pwm_slot_t;

pwm_slot_t pwm_slots[NUM_LEDS];

uint8_t num_slots;

void rebuild_slots(){
	uint8_t x, unsorted;
	uint16_t msk = 0x01;
	for(x=0;x<NUM_LEDS;x++){
		pwm_slots[x].mask = msk;
		pwm_slots[x].bright = bright[x];
	}
	
	num_slots = NUM_LEDS;
	
	do{
		for(x=0;x<num_slots;x++){
			
		}
	}while(unsorted);
	

}
