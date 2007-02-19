#include "logic.h"

#include <stdio.h>
#include "piece.h"
#include "playfield.h"


int main (int argc, char* argv[])
{

    tetris_playfield_t* pPl = tetris_playfield_construct(10, 16);
    tetris_piece_t* pPiece;
    tetris_piece_t* pOldPiece;

    int i = 0;
    while (pPl->status != TETRIS_PFS_GAMEOVER)
    {
        pPiece = tetris_piece_construct(TETRIS_PC_LINE, TETRIS_PC_ANGLE_0);

        tetris_playfield_insertPiece(pPl, pPiece, &pOldPiece);
        if (pOldPiece != NULL)
        {
            tetris_piece_destruct(pOldPiece);
            pOldPiece = NULL;
        }

        while (tetris_playfield_advancePiece(pPl) == TETRIS_PFS_HOVERING)
        {
            printf("piece: %d   column: %d   row: %d\n", i, pPl->nColumn, pPl->nRow);
        }

        i++;

        if (pPl->status == TETRIS_PFS_DOCKED)
            pPl->status = TETRIS_PFS_READY;
    }

    printf("Game Over!\n");

    tetris_playfield_destruct (pPl);
    tetris_piece_destruct (pPiece);

    return 0;
}

