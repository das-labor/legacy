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
	
	pPl->contents[0]  = 0x0110;
	pPl->contents[1]  = 0x0111;
	pPl->contents[2]  = 0x0112;
	pPl->contents[3]  = 0x0113;
	pPl->contents[4]  = 0x0114;
	pPl->contents[5]  = 0x0115;
	pPl->contents[6]  = 0x03FF;
	pPl->contents[7]  = 0x03FF;
	pPl->contents[8]  = 0x0116;
	pPl->contents[9]  = 0x03FF;
	pPl->contents[10] = 0x0117;
	pPl->contents[11] = 0x0118;
	pPl->contents[12] = 0x0119;
	pPl->contents[13] = 0x011A;
	pPl->contents[14] = 0x011B;
	pPl->contents[15] = 0x011C;


	for (i = 0; i < 16; i++)
		printf("%04x, ", pPl->contents[i]);
	printf("\n");

	pPl->nRow = 6;
	pPl->status = TETRIS_PFS_DOCKED;
	tetris_playfield_removeCompleteLines(pPl);

	for (i = 0; i < 16; i++)
		printf("%04x, ", pPl->contents[i]);

    return 0;
}

