#include "blinkenlights.h"

#include "borg_hw.h"
#include "pixel.h"


void showframe_blm(unsigned char *blm){
	for(int y=0;y < NUM_ROWS; y++){
		for(int x=0;x < NUM_COLS; x++, blm++){
			if(blm=='1')
				setpixel((pixel){x,y},3);
			else
				setpixel((pixel){x,y},0);
		}
	}
}

/*	Parser idee:
	
	bufferfill;
	find "# duration = XXX" line; ?? or ignore
	find " # loop = yes|no" line;	use??
	
	ignore empty lines;
	


/* rough parser copy from rtnet code::
	FIXME: can I include string.h??

  // find newline char and make it end of string
           sp = strchr(buf, '\n');
           if (sp)
               *sp = '\0';
   
  //ignore comments and empty lines
  if ((buf[0] == '#') || (buf[0] == '\0'))
               continue

*/