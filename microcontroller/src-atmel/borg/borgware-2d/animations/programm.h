#ifndef PROGRAMM_H_
#define PROGRAMM_H_

void off();
void spirale(unsigned int delay);
void joern1();
void joern2();
void draw_line( unsigned char yabs, signed char delta);
void schachbrett(unsigned char times);
void test1();
void test_level1();
void test_level2();
void test_level3();
void test_levels();
void test_palette();
void test_palette2();

// randon dots appear and stay
void dots1();

// rectangle-pattern
void rectangle1();

// random rectangles
void rectangles();

// lines show and disappear quickly
void lines1();

//  8x8 rectangle with a cross inside 
void checkbox();

// a scanner effect with moving h & v lines
void movinglines();

void snake();
void matrix();
void fadein();
void feuer();
void random_bright(unsigned cycles);

#endif /* PROGRAMM_H_ */
