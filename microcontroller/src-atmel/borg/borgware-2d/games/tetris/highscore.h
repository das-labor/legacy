#ifndef TETRIS_HIGHSCORE_H_
#define TETRIS_HIGHSCORE_H_

#include <stdint.h>
#include "../../compat/eeprom.h"


/**
 * indexes for different tetris variants
 */
enum tetris_highscore_index_e
{
	TETRIS_HISCORE_TETRIS, /**< high score index for the standard variant */
	TETRIS_HISCORE_BASTET, /**< high score index for the bastet variant */
	TETRIS_HISCORE_FP,     /**< high score index for the first person variant */
	TETRIS_HISCORE_PAD,    /**< don't use (padding for an even array boundary)*/
	TETRIS_HISCORE_END     /**< boundary for the high score array */
};
#ifdef NDEBUG
	typedef uint8_t tetris_highscore_index_t;
#else
	typedef enum tetris_highscore_index tetris_highscore_index_t;
#endif


/**
 * type for global high score table
 */
typedef struct tetris_highscore_table_s
{
	uint16_t nHighScore[TETRIS_HISCORE_END];     /**< actual high scores */
	uint16_t nHighScoreName[TETRIS_HISCORE_END]; /**< champions' initials */
}
tetris_highscore_table_t;


/**
 * the actual high score table
 */
extern tetris_highscore_table_t g_highScoreTable EEMEM;


/**
 * lets the user enter his initials (three characters)
 * @return name packed into a uint16_t
 */
uint16_t tetris_highscore_inputName(void);


/**
 * retrieves the high score from storage (EEPROM)
 * @param nIndex the variant dependent index of the high score
 * @return the high score
 */
uint16_t tetris_highscore_retrieveHighScore(tetris_highscore_index_t nIndex);


/**
 * saves the high score into the storage (EEPROM)
 * @param nIdx the variant dependent index of the high score
 * @param nHighscoreName the high score
 */
inline static
void tetris_highscore_saveHighScore(tetris_highscore_index_t nIndex,
                                    uint16_t nHighScore)
{
	if (nHighScore > tetris_highscore_retrieveHighScore(nIndex))
	{
		eeprom_write_word(&g_highScoreTable.nHighScore[nIndex], nHighScore);
	}
}


/**
 * retrieves the champion's initials from storage (EEPROM)
 * @param nIdx the variant dependent index of the high score
 * @return the initials of the champion packed into a uint16_t
 */
inline static
uint16_t tetris_highscore_retrieveHighScoreName(tetris_highscore_index_t nIdx)
{
	uint16_t nHighScoreName =
			eeprom_read_word(&g_highScoreTable.nHighScoreName[nIdx]);

	return nHighScoreName;
}


/**
 * saves the champion's initials into the storage (EEPROM)
 * @param nIndex the variant dependent index of the high score
 * @param nHighscoreName the initials of the champion packed into a uint16_t
 */
inline static
void tetris_highscore_saveHighScoreName(tetris_highscore_index_t nIndex,
                                        uint16_t nHighscoreName)
{
	eeprom_write_word(&g_highScoreTable.nHighScoreName[nIndex], nHighscoreName);
}


#endif /*TETRIS_HIGHSCORE_H_*/
