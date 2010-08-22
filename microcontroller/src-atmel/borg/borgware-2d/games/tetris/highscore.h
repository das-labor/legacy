#ifndef TETRIS_HIGHSCORE_H_
#define TETRIS_HIGHSCORE_H_

/**
 * indexes for different tetris variants
 */
typedef enum tetris_highscore_index_t
{
	TETRIS_HISCORE_TETRIS, /**< high score index for the standard variant */
	TETRIS_HISCORE_BASTET, /**< high score index for the bastet variant */
	TETRIS_HISCORE_FP,     /**< high score index for the first person variant */
	TETRIS_HISCORE_PAD,    /**< don't use (padding for an even array boundary)*/
	TETRIS_HISCORE_END     /**< boundary for the high score array */
} tetris_highscore_index_t;


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
uint16_t tetris_highscore_retrieveHighscore(tetris_highscore_index_t nIndex);


/**
 * saves the high score into the storage (EEPROM)
 * @param nIndex the variant dependent index of the high score
 * @param nHighscoreName the high score
 */
void tetris_highscore_saveHighscore(tetris_highscore_index_t nIndex,
                                    uint16_t nHighscore);


/**
 * retrieves the champion's initials from storage (EEPROM)
 * @param nIdx the variant dependent index of the high score
 * @return the initials of the champion packed into a uint16_t
 */
uint16_t tetris_highscore_retrieveHighscoreName(tetris_highscore_index_t nIdx);


/**
 * saves the champion's initials into the storage (EEPROM)
 * @param nIndex the variant dependent index of the high score
 * @param nHighscoreName the initials of the champion packed into a uint16_t
 */
void tetris_highscore_saveHighscoreName(tetris_highscore_index_t nIndex,
                                        uint16_t nHighscoreName);

#endif /*TETRIS_HIGHSCORE_H_*/
