#ifndef TETRIS_PLAYFIELD_H_
#define TETRIS_PLAYFIELD_H_

#include <inttypes.h>
// #include "../config.h"
#include "piece.h"


/*********
 * types *
 *********/

/* directions to which a piece can be moved */
typedef enum tetris_playfield_direction_t_
{
    TETRIS_PFD_LEFT,
    TETRIS_PFD_RIGHT
}
tetris_playfield_direction_t;


/* status of the playfield */
typedef enum tetris_playfield_status_t_
{
    TETRIS_PFS_READY,    /* ready to get next piece */
    TETRIS_PFS_HOVERING, /* piece is still hovering */
    TETRIS_PFS_DOCKED,   /* piece has been docked */
    TETRIS_PFS_GAMEOVER	 /* playfield is filled up */
}
tetris_playfield_status_t;


/* tetris_playfield_t */
typedef struct tetris_playfield_t_
{
    tetris_piece_t* pPiece;           /* currently falling piece */
    int8_t nColumn;                   /* horiz. piece position (0 is left) */
    int8_t nRow;                      /* vert. piece position (0 is top) */
    int8_t nWidth;                    /* width of playfield */
    int8_t nHeight;                   /* height of playfield */
    tetris_playfield_status_t status; /* status */
    uint16_t *contents;               /* playfield itself */
}
tetris_playfield_t;


/*****************************
 *  construction/destruction *
 *****************************/
tetris_playfield_t* tetris_playfield_construct(int8_t nWidth, int8_t nHeight);
void tetris_playfield_destruct(tetris_playfield_t* pPl);


/********************************
 *  playfield related functions *
 ********************************/

/* Function:           tetris_playfield_reset
 * Description:        resets playfield to begin a new game
 * Argument pPl:       playfield to perform action on
 * Return value:       void
 */
void tetris_playfield_reset(tetris_playfield_t* pPl);


/* Function:            tetris_playfield_insertPiece
 * Description:         inserts a new piece
 * Argument pPl:        playfield to perform action on
 * Argument pPiece:     piece to be inserted
 * Argument ppOldPiece: indirect pointer to former piece for deallocation
 * Return value:        0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_insertPiece(tetris_playfield_t* pPl,
                                     tetris_piece_t* pPiece,
                                     tetris_piece_t** ppOldPiece);


/* Function:           tetris_playfield_advancePiece
 * Description:        lowers piece by one row or finally docks it
 * Argument pPl:       playfield to perform action on
 * Return value:       playfield status (see tetris_playfield_status_t)
 */
tetris_playfield_status_t tetris_playfield_advancePiece(tetris_playfield_t* pPl);


/* Function:           tetris_playfield_movePiece
 * Description:        moves piece to the given direction
 * Argument pPl:       playfield to perform action on
 * Argument direction: direction (see tetris_playfield_direction_t)
 * Return value:       1 if piece could be moved, 0 otherwise 
 */
uint8_t tetris_playfield_movePiece(tetris_playfield_t* pPl,
                                   tetris_playfield_direction_t direction);


/* Function:           tetris_playfield_removeCompletedLines
 * Description:        removes completed lines (if any) and lowers the dump
 * Argument pPl:       playfield to perform action on
 * Return value:       number of removed lines (0-4)
 */
uint8_t tetris_playfield_removeCompleteLines(tetris_playfield_t* pPl);


/* Function:           collision
 * Description:        detects if piece collides with s.th. at a given position
 * Argument pPl:       playfield to perform action on
 * Argument nColumn:   column where the piece should be moved
 * Argument nRow:      row where the piece should be moved
 * Return value:       1 for collision, 0 otherwise
 */
uint8_t tetris_playfield_collision(tetris_playfield_t* pPl,
                                   int8_t nColumn,
                                   int8_t nRow);
                                    
#endif /*TETRIS_PLAYFIELD_H_*/
