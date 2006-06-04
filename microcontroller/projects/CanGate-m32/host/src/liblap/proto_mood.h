#ifndef MOOD_H 
#define MOOD_H

// functions
unsigned char mood_set_mood( can_addr dst,unsigned char mood, unsigned char val1, unsigned char val2, unsigned char val3, unsigned char val4 );

int mood_get_mood(can_addr dst,int mood,int moodled);

#endif
