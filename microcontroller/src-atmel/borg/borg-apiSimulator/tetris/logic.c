#include "logic.h"

#include <stdio.h>
#include "piece.h"
#include "playfield.h"
#include "view.h"

void tetris ()
{

    tetris_playfield_t* pPl = tetris_playfield_construct(10, 16);
    
    tetris_view_t* pView = tetris_view_construct(pPl);
    
    tetris_piece_t* pPiece;
    tetris_piece_t* pOldPiece;

    int i = 0;
    
    /*
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
	*/
	
	pPl->dump[0]  = 0x03DF;
	pPl->dump[1]  = 0x03DF;
	pPl->dump[2]  = 0x03DF;
	pPl->dump[3]  = 0x03DF;
	pPl->dump[4]  = 0x03DF;
	pPl->dump[5]  = 0x03DF;
	pPl->dump[6]  = 0x03FF;
	pPl->dump[7]  = 0x03FF;
	pPl->dump[8]  = 0x03FF;
	pPl->dump[9]  = 0x03FF;
	pPl->dump[10] = 0x03DF;
	pPl->dump[11] = 0x03DF;
	pPl->dump[12] = 0x03DF;
	pPl->dump[13] = 0x03DF;
	pPl->dump[14] = 0x03DF;
	pPl->dump[15] = 0x03DF;
	
	pPl->nRow = 15;
	tetris_view_updatePlayfield(pView);


	for (i = 0; i < 16; i++)
		printf("%04x, ", pPl->dump[i]);
	printf("\n");

	pPl->nRow = 6;
	pPl->status = TETRIS_PFS_DOCKED;
	tetris_playfield_removeCompleteLines(pPl);

	pPl->nRow = 15;
	tetris_view_updatePlayfield(pView);

	for (i = 0; i < 16; i++)
		printf("%04x, ", pPl->dump[i]);
		
	tetris_view_destruct(pView);
	tetris_playfield_destruct(pPl);
}

