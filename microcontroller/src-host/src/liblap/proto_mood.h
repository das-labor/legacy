#ifndef MOOD_H 
#define MOOD_H

// functions
unsigned char mood_set_mood( can_addr dst,unsigned char mood, unsigned char val1, unsigned char val2, unsigned char val3, unsigned char val4 );

void mood_get_mood(can_addr dst,uint8_t mood,uint8_t * val1,uint8_t * val2,uint8_t * val3,uint8_t * val4);

#endif
