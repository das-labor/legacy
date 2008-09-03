#include "programs.h"

/** Display Thread 
 *  This function is used as an invinity loop with the playlist.
 *  Here you call your animation function. normally you will put it to 
 *  the begin to see it then the simulator starts.
 */
void *display_loop(void * unused) {
	while (1) {
		rotatedScaledCube();  
		feuer();
		matrix();
	    snake3d();
		spirale();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		coolAnim();
		movingArrows();
		planeAnimation2(30);
		joern1();
		
		setplane_test();
		shift_test();
		test3();
		test2();
		test2();	
	}
}

