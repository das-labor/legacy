#include "programs.h"

/** Display Thread 
 *  This function is used as an invinity loop with the playlist.
 *  Here you call your animation function. normally you will put it to 
 *  the begin to see it then the simulator starts.
 */
void *display_loop(void * unused) {
	while (1) {
	    setplane_test();
		shift_test();
		test3();
		test2();
		test2();	
		
    /*  flury();
		rotatedScaledCube();
		growingCubeFilled();
		planeAnimation2(80);
		rotatePixmap(1);
		rotatePixmap(2);
		growingCubeFilled();
		rotatePixmap(0);
        movingArrows();
        growingCubeFilled();
        scrolltext("Borg 3D!!!", 120);
        growingCubeFilled();
        joern1();
		growingCubeFilled();
		growingCubeFilled();
		gameOfLife(0, 400);
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		growingCubeFilled();
		spirale();
		matrix();
		test3();
		coolAnim();
		growingCubeFilled();
		growingCubeFilled();
		coolAnim();
		test2();
		test2();
		test2();
		test2();
		snake3d();
		feuer();
*/
	}
}

