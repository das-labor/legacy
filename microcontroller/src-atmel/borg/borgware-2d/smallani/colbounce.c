#include <inttypes.h>
#include "../config.h"
#include "../pixel.h"
#include "../util.h"
#include "colbounce.h"

void colbounce(uint8_t times,uint8_t speed)
{
	uint8_t  i, j,k,h;
	
	for(k=0;k<times;k++){
	  clear_screen(0);
	  for (h=0;h<NUM_COLS;h++){
	    for (i=0;i<NUM_COLS;i++){
	      for (j=0;j<NUM_ROWS;j++){
		setpixel( (pixel){i,j},(h==i) ? 1:0);
	      }
	    }
	    wait(speed*10);
	  }
	  for (h=NUM_COLS-1;h>1;h--){
	    for (i=0;i<NUM_COLS;i++){
	      for (j=0;j<NUM_ROWS;j++){
		setpixel( (pixel){i,j},((h-1)==i) ? 1:0);
	      }
	    }
	    wait(speed*10);
	  }

	}

}
