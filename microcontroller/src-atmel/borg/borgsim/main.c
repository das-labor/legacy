#include <ncurses.h>
#include "programm.h"
#include "borgsim.h"
#include "simpixel.h"

int main (void){

	init_curses();
	clear_screen(2);
	wait(100);
	clear_screen(0);
	//borg_hw_init();
	//sei();
	
//	for(;;){
//		snake();
		tetris();
		tetris();
		tetris();
//		matrix();
//		fadein();
//		joern1();
//test1();
//		schachbrett(20);
//		spirale(20);
//		labor_borg();
//		feuer();
		

//		testline();
//		test1();
//		draw_l();

//		clear_screen;
//     		wait(500);	
//	}
	end_curses();
}
