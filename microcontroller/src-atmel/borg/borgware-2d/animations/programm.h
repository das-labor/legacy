#ifndef PROGRAMM_H_
#define PROGRAMM_H_

#include "../pixel.h"
#include "../util.h"


void test_level(unsigned char level);
void test_palette();
void test_palette2();

#ifdef ANIMATION_OFF
inline static void off()
{
	clear_screen(0);

	while(1)
		wait(100);

}
#endif

void spirale(unsigned int delay);
void joern1();
void schachbrett(unsigned char times);
void feuer();
void random_bright(unsigned cycles);

#endif /* PROGRAMM_H_ */
