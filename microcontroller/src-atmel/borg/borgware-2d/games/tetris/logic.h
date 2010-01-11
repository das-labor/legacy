#ifndef TETRIS_LOGIC_H_
#define TETRIS_LOGIC_H_

#include <inttypes.h>
#include "piece.h"

/*********
 * types *
 *********/

typedef struct tetris_logic_t
{
	uint8_t nBastet;               // is gametype bastet?
	uint16_t nScore;               // score of the player
	uint16_t nHighscore;           // highscore
	uint16_t nHighscoreName;       // name of the person who achieved highscore
	uint8_t nLevel;                // current level
	uint16_t nLines;               // number of completed lines
	tetris_piece_t *pPreviewPiece; // the piece intended to be the next one
}
tetris_logic_t;

/****************************
 * construction/destruction *
 ****************************/

/* Function:         tetris_logic_construct
 * Description:      constructs a logic object
 * Argument nBastet: 0 for normal tetris, 1 for bastet
 * Return value:     pointer to a newly created logic object
 */
tetris_logic_t *tetris_logic_construct(uint8_t nBastet);

/* Function:     tetris_logic_destruct
 * Description:  destructs a logic object
 * Argument pIn: pointer to the logic object to be destructed
 * Return value: void
 */
void tetris_logic_destruct(tetris_logic_t *pLogic);

/***************************
 * logic related functions *
 ***************************/

/* Function:     tetris
 * Description:  runs the tetris game
 * Return value: void
 */
void tetris();


/* Function:     tetris_bastet
 * Description:  runs the bastet game
 * Return value: void
 */
void tetris_bastet();


/* Function:         tetris_main
 * Description:      runs the tetris game
 * Argument nBastet: 0 for normal Tetris, 1 for Bastet
 * Return value:     void
 */
void tetris_main(int8_t nBastet);

/* Function:        tetris_logic_singleDrop
 * Description:     add points which result from single step dropping
 * Argument pLogic: the logic object we want to modify
 * Argument nLines: the number of rows involved
 * Return value:    void
 */
void tetris_logic_singleDrop(tetris_logic_t *pLogic,
                             uint8_t nLines);


/* Function:        tetris_logic_completeDrop
 * Description:     add points which result from a complete drop
 * Argument pLogic: the logic object we want to modify
 * Argument nLines: the number of rows involved
 * Return value:    void
 */
void tetris_logic_completeDrop(tetris_logic_t *pLogic,
                               uint8_t nLines);


/* Function:          tetris_logic_removedLines
 * Description:       add points which result from removed rows
 * Argument pLogic:   the logic object we want to modify
 * Argument nRowMask: see tetris_playfield_completeLines
 * Return value:      void
 */
void tetris_logic_removedLines(tetris_logic_t *pLogic,
                               uint8_t nRowMask);


/*********************
 * get/set functions *
 *********************/

/* Function:        tetris_logic_getScore
 * Description:     returns the current score
 * Argument pLogic: the logic object we want information from
 * Return value:    the score as uint16_t
 */
uint16_t tetris_logic_getScore(tetris_logic_t *pLogic);


/* Function:        tetris_logic_getHighscore
 * Description:     returns the current highscore
 * Argument pLogic: the logic object we want information from
 * Return value:    the highscore as uint16_t
 */
uint16_t tetris_logic_getHighscore(tetris_logic_t *pLogic);


/* Function:             tetris_logic_setHighscore
 * Description:          set highscore
 * Argument pLogic:      the logic object we want to modify
 * Argmument nHighscore: highscore
 */
void tetris_logic_setHighscore(tetris_logic_t *pLogic,
                               uint16_t nHighscore);


/* Function:        tetris_logic_getHighscoreName
 * Description:     returns the current highscore name
 * Argument pLogic: the logic object we want information from
 * Return value:    the highscore name packed as uint16_t
 */
uint16_t tetris_logic_getHighscoreName(tetris_logic_t *pLogic);


/* Function:                 tetris_logic_setHighscoreName
 * Description:              set highscore name
 * Argument pLogic:          the logic object we want to modify
 * Argmument nHighscoreName: highscore name
 */
void tetris_logic_setHighscoreName(tetris_logic_t *pLogic,
                                   uint16_t nHighscoreName);


/* Function:        tetris_logic_getLevel
 * Description:     returns the current level
 * Argument pLogic: the logic object we want information from
 * Return value:    the level as uint8_t
 */
uint8_t tetris_logic_getLevel(tetris_logic_t *pLogic);


/* Function:        tetris_logic_getLines
 * Description:     returns the number of completed lines
 * Argument pLogic: the logic object we want information from
 * Return value:    number of completed lines as uint16_t
 */
uint16_t tetris_logic_getLines(tetris_logic_t *pLogic);


/* Function:        tetris_logic_setPreviewPiece
 * Description:     help for the view to determine the preview piece
 * Argument pLogic: the logic object we want to modify
 * Argument pPiece: pointer to piece intended to be the next one
 * Return value:    void
 */
void tetris_logic_setPreviewPiece(tetris_logic_t *pLogic,
                                  tetris_piece_t *pPiece);


/* Function:        tetris_logic_getPreviewPiece
 * Description:     returns piece which was set via tetris_logic_setPreviewPiece
 * Argument pLogic: the logic object we want information from
 * Return value:    the piece intended to be the next one
 */
tetris_piece_t* tetris_logic_getPreviewPiece(tetris_logic_t *pLogic);


#endif /*TETRIS_LOGIC_H_*/
