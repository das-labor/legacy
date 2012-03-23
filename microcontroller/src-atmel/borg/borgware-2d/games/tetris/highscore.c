#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include "../../config.h"
#include "../../scrolltext/scrolltext.h"
#include "../../joystick/joystick.h"
#include "../../compat/eeprom.h"
#include "highscore.h"


tetris_highscore_table_t g_highScoreTable EEMEM;


uint16_t tetris_highscore_inputName(void)
{
#ifdef SCROLLTEXT_SUPPORT
	char pszNick[4] = {'A', 'A', 'A', '\0'};
	char pszTmp[26];
	uint8_t nPos = 0, nBlink = 0, nHadfire = 0;

	while (1)
	{
		// we need our own blink interval
		nBlink = (nBlink + 1) % 4u;

		// construct command for scrolltext and execute
		static uint8_t const nOffset[3] = {15, 19, 23};
		snprintf(pszTmp, sizeof(pszTmp), "x%u+p1#%c#x%u+p1#%c#x%up1#%c",
				nOffset[nPos]     , (!nBlink && nPos == 0) ? ' ' : pszNick[0],
				nOffset[nPos] -  8, (!nBlink && nPos == 1) ? ' ' : pszNick[1],
				nOffset[nPos] - 15, (!nBlink && nPos == 2) ? ' ' : pszNick[2]);
		scrolltext(pszTmp);

		// up and down control current char
		if (JOYISUP)
		{
			pszNick[nPos]++;
			if (pszNick[nPos] == '`')
			{
				pszNick[nPos] = 'A';
			}
			else if (pszNick[nPos] == '[')
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
			else if (pszNick[nPos] == '^')
			{
				pszNick[nPos] = 'Z';
			}
		}

		// left and right control char selections
		if (JOYISLEFT && nPos > 0)
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
			if (nPos++ == 2)
			{
				break;
			}
		}

		if (!JOYISFIRE)
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


uint16_t tetris_highscore_retrieveHighScore(tetris_highscore_index_t nIndex)
{
	uint16_t nHighScore =
			eeprom_read_word(&g_highScoreTable.nHighScore[nIndex]);

	// a score of 65535 is most likely caused by uninitialized EEPROM addresses
	return nHighScore == UINT16_MAX ? 0 : nHighScore;
}
