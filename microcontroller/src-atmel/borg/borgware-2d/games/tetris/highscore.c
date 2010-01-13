#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
#ifdef SCROLLTEXT_SUPPORT
	char pszNick[4], pszTmp[40];
	uint8_t nOffset;
	uint8_t nPos = 0, nBlink = 0, nDone = 0, nHadfire = 0;

	sprintf(pszNick, "AAA");
	while (!nDone)
	{
		// we need our own blink interval
		nBlink = (nBlink + 1) % 4;

		// determine start position on screen depending on active character
		switch (nPos)
		{
		case 0:
			nOffset = 15;
			break;
		case 1:
			nOffset = 19;
			break;
		case 2:
			nOffset = 23;
			break;
		}

		// construct command for scrolltext and execute
		sprintf(pszTmp, "x%d+p1#%c#x%d+p1#%c#x%dp1#%c", nOffset,
				(!nBlink && nPos == 0) ? ' ' : pszNick[0], nOffset - 8,
				(!nBlink && nPos == 1) ? ' ' : pszNick[1], nOffset - 15,
				(!nBlink && nPos == 2) ? ' ' : pszNick[2]);
		scrolltext(pszTmp);

		// up and down control current char
		if (JOYISUP)
		{
			pszNick[nPos]++;
			if (pszNick[nPos] == '`')
			{
				pszNick[nPos] = 'A';
			}
			if (pszNick[nPos] == '[')
			{
				pszNick[nPos] = '_';
			}
		}
		else if (JOYISDOWN)
		{
			pszNick[nPos]--;
			if (pszNick[nPos] == '@')
			{
				pszNick[nPos] = '_';
			}
			if (pszNick[nPos] == '^')
			{
				pszNick[nPos] = 'Z';
			}
		}
		// left and right control char selections
		else if (JOYISLEFT && nPos > 0)
		{
			nPos--;
		}
		else if (JOYISRIGHT && nPos < 2)
		{
			nPos++;
		}

		// fire switches to next char or exits
		if (JOYISFIRE && !nHadfire)
		{
			nHadfire = 1;
			switch (nPos)
			{
			case 0:
				nPos = 1;
				break;
			case 1:
				nPos = 2;
				break;
			case 2:
				nDone = 1;
				break;
			}
		}

		if (nHadfire && !JOYISFIRE)
		{
			nHadfire = 0;
		}
	}

	// return result
	return (pszNick[0] - 65) << 10 | (pszNick[1] - 65) << 5 | (pszNick[2] - 65);
#else
	return (0);
#endif
}
