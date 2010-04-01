#ifndef TETRIS_HIGHSCORE_H_
#define TETRIS_HIGHSCORE_H_

/**
 * indexes for different tetris variants
 */
typedef enum tetris_highscore_index_t
{
	TETRIS_HISCORE_TETRIS, /**< highscore index for the standard variant */
	TETRIS_HISCORE_BASTET, /**< highscore index for the bastet variant */
	TETRIS_HISCORE_FP,     /**< highscore index for the first person variant */
	TETRIS_HISCORE_PAD,    /**< don't use (padding for an even array boundary)*/
	TETRIS_HISCORE_END     /**< boundary for the highscore array */
} tetris_highscore_index_t;


/**
 * let user input a three character name
 * @return name packed into a uint16_t
 */
uint16_t tetris_highscore_inputName(void);


/**
 * retrieves the highscore from storage
 * @param nIndex the variant dependent index of the highscore
 * @return the highscore
 */
uint16_t tetris_highscore_retrieveHighscore(tetris_highscore_index_t nIndex);


/**
 * saves the highscore into the storage
 * @param nIndex the variant dependent index of the highscore
 * @param nHighscoreName the highscore
 */
void tetris_highscore_saveHighscore(tetris_highscore_index_t nIndex,
                                    uint16_t nHighscore);


/**
 * retrieves the initials of the champion from storage
 * @param nIdx the variant dependent index of the highscore
 * @return the initials of the champion packed into a uint16_t
 */
uint16_t tetris_highscore_retrieveHighscoreName(tetris_highscore_index_t nIdx);


/**
 * saves the initials of the champion
 * @param nIndex the variant dependent index of the highscore
 * @param nHighscoreName the initials of the champion packed into a uint16_t
 */
void tetris_highscore_saveHighscoreName(tetris_highscore_index_t nIndex,
                                        uint16_t nHighscoreName);

#endif /*TETRIS_HIGHSCORE_H_*/
