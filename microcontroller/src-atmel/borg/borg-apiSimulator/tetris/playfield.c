#include <stdlib.h>
#include "playfield.h"

/*****************************
 *  construction/destruction *
 *****************************/
tetris_playfield_t* tetris_playfield_construct(uint8_t nWidth, uint8_t nHeight) {
	if (nWidth <= 16) {
		tetris_playfield_t* p_playfield = (tetris_playfield_t*) malloc(sizeof(tetris_playfield_t));
		if (p_playfield != NULL) {
			p_playfield->contents = (uint16_t*) malloc(sizeof(uint16_t) * nHeight);
			if (p_playfield->contents != NULL) {				
				p_playfield->nWidth = nWidth;
				p_playfield->nHeight = nHeight;
				tetris_playfield_reset(p_playfield);
				
				return p_playfield;
			} else {
				free(p_playfield);
				p_playfield = NULL;
			} 
		}
	}
	return NULL;
}

void tetris_playfield_destruct(tetris_playfield_t* p_pl) {
	if (p_pl != NULL) {
		if (p_pl->contents != NULL) {
			free(p_pl->contents);
		}
		free(p_pl);
	}
}


/********************************
 *  playfield related functions *
 ********************************/

/* Function:           tetris_playfield_reset
 * Description:        Resets playfield to begin a new game;
 * Argument p_pl:      The playfield to perform action on
 * Return value:       void
 */
void tetris_playfield_reset(tetris_playfield_t* p_pl) {
	if (p_pl != NULL) {
		p_pl->nFillHeight = 0;
		p_pl->p_piece = NULL;
		p_pl->status = TETRIS_PFS_DOCKED;
		p_pl->nColumn = 0;
		p_pl->nRow = 0;
		
		/* clear contents */
		if (p_pl->contents != NULL) {
			int i;
			for (i = 0; i < p_pl->nHeight; ++i) {
				p_pl->contents[i] = 0;
			}
		}
	}
}


/* Function:           tetris_playfield_insertPiece
 * Description:        Inserts a new piece
 * Argument p_pl:      The playfield to perform action on
 * Argument p_piece:   The piece to be insertes
 * Return value:       0 corresponds to false, anything other to true
 */
uint8_t tetris_playfield_insertPiece(tetris_playfield_t* p_pl, tetris_piece_t* p_piece) {
	/* a piece can only be inserted if the playfield is in state TETRIS_PFS_DOCKED */
	if (p_pl->status == TETRIS_PFS_DOCKED) {
		/* replace old piece */
		p_pl->p_piece = p_piece;
		
		/* set start position (in the middle of the top line) */
		p_pl->nColumn = (p_pl->nWidth - 2) / 2;
		p_pl->nRow = -3;
		
		/* OK */
		return 1;
	} else {
		/* wrong state */
		return 0;
	}
}


/* Function:           collisionDetect
 * Description:        
 * Argument p_pl:      The playfield to perform action on
 * Argument p_pc:      The piece which shoud be checked
 * Argument nRow:      The row where the piece should be moved
 * Argument nColumn:   The column where the piece should be moved
 * Return value:       1 for collision, 0 otherwise
 */
uint16_t tetris_playfield_collisionDetect(tetris_playfield_t* p_pl, tetris_piece_t* p_pc, int8_t nRow, int8_t nColumn) {
	/* only allow coordinates which are within sane ranges */
	if ((nRow < p_pl->nWidth) && (nColumn < p_pl->nHeight)) {
		
		uint16_t piece = tetris_piece_getBitfield(p_pc);
		uint16_t nPlayfieldRow;
		uint16_t nPieceRow;
		int8_t y;		

		for (y = (nRow < 0) ? -nRow : 0; (y < 4) && ((y + nRow) < p_pl->nHeight); ++y) {
			
			/* negative shifting values are bad, so we have to work around them */
			if (nColumn < 0) {
				nPlayfieldRow = ((p_pl->contents[y + nRow] & (0x000F >> -nColumn)) << -nColumn) | (0x000F >> (4 + nColumn));
			/* if piece passes right border, we insert some matter in playfield compare row */
			} else if ((nColumn + 3) >= p_pl->nWidth) {
				nPlayfieldRow = (p_pl->contents[y + nRow] >> nColumn) | (((0x000F << (nColumn + 4 - p_pl->nWidth))) & 0x000F);
			} else {
				nPlayfieldRow = (p_pl->contents[y + nRow] & (0x000F << nColumn)) >> nColumn;
			}
			
			nPieceRow = (piece & (0x000F << (y * 4))) >> (0x000F << (y * 4)); 

			if ((nPlayfieldRow & nPieceRow) != 0) {
				return 1;
			}
		}
	}
	
	return 0;
}


/* Function:           tetris_playfield_advancePiece
 * Description:        Lowers piece by one row or finally docks it
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns a value of enum tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_advancePiece(tetris_playfield_t* p_pl) {
	return TETRIS_PFS_DOCKED;
}


/* Function:           tetris_playfield_movePiece
 * Description:        Moves piece to the given direction
 * Argument p_pl:      The playfield to perform action on
 * Argument direction: Direction (see tetris_playfield_direction_t)
 * Return value:       void
 */
void tetris_playfield_movePiece(tetris_playfield_t* p_pl, enum tetris_playfield_direction_t direction) {
}


/* Function:           tetris_playfield_dropPiece
 * Description:        Drops piece immediately
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns a value of enum tetris_playfield_status_t (see above)
 */
enum tetris_playfield_status_t tetris_playfield_dropPiece(tetris_playfield_t* p_pl) {
	return TETRIS_PFS_DOCKED;
}


/* Function:           tetris_playfield_removeCompletedLines
 * Description:        Removes completed lines (if any) and lowers the dump correlatively
 * Argument p_pl:      The playfield to perform action on
 * Return value:       Returns number of removed lines (0-4)
 */
uint8_t tetris_playfield_removeCompleteLines(tetris_playfield_t* p_pl) {
	return 0;
}
