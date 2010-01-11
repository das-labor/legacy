#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "../../config.h"
#include "../../scrolltext/scrolltext.h"
#include "../../joystick/joystick.h"
#include "highscore.h"

/* Function:     tetris_highscore_inputName
 * Description:  let user input a three character name
 * Return value: name packed into a uint16_t
 */
uint16_t tetris_highscore_inputName(void)
{
	char nick[4], tmp[40];
        uint8_t xpos;
	uint8_t pos=0, blink=0, done=0, hadfire=0;

	sprintf(nick, "AAA");
        while(!done)
        {

          // We need to do our own blink interval          
          blink = (blink+1) % 4;

          // Determine start position on screen
          // depending on active character
          switch (pos)
          {
            case 0: xpos = 15; break;
            case 1: xpos = 19; break;
            case 2: xpos = 23; break;
          }
          
          // Construct command for scrolltext and execute
  	  sprintf(tmp, "x%d+p1#%c#x%d+p1#%c#x%dp1#%c", 
  	    xpos, (!blink && pos == 0) ? ' ' : nick[0],
 	    xpos-8, (!blink && pos == 1 ) ? ' ' : nick[1],
 	    xpos-15, (!blink && pos == 2 ) ? ' ' : nick[2]);
  	  scrolltext(tmp);
 	  

          // up and down control current char
 	  if (JOYISUP) 
 	  { 
 	    nick[pos]++; 
 	    if (nick[pos] == '`') nick[pos] = 'A';
 	    if (nick[pos] == '[') nick[pos] = '_'; 	    
          }
 	  if (JOYISDOWN)
 	  { 
 	    nick[pos]--; 
 	    if (nick[pos] == '@') nick[pos] = '_';
            if (nick[pos] == '^') nick[pos] = 'Z';
          }
          
          // left and right control char selections
          if (JOYISLEFT && pos > 0) pos--; 
          if (JOYISRIGHT && pos < 2) pos++;
          
          // fire switches to next char or exits
          if (JOYISFIRE&&!hadfire)
          {
            hadfire=1;
            switch (pos)
            {
              case 0: pos=1; break;
              case 1: pos=2; break;
              case 2: done=1; break;
            }
          }
          
          if (hadfire&&!JOYISFIRE)
            hadfire=0;
	}

        // return result
	return(
		(nick[0]-65)<<10 |
		(nick[1]-65)<<5  |
		(nick[2]-65) 
	      );
}
