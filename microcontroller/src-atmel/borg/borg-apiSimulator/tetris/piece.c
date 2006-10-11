#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "piece.h"

tetris_piece_t* tetris_piece_construct (enum tetris_piece_type_t t) {
    static tetris_piece_t pc_line = {
                                        {0,0,1,0},
                                        {0,0,1,0},
                                        {0,0,1,0},
                                        {0,0,1,0}
                                    };

    static tetris_piece_t pc_t = {
                                     {0,0,0,0},
                                     {0,0,1,0},
                                     {0,1,1,1},
                                     {0,0,0,0}
                                 };

    static tetris_piece_t pc_square = {
                                          {0,0,0,0},
                                          {0,1,1,0},
                                          {0,1,1,0},
                                          {0,0,0,0}
                                      };

    static tetris_piece_t pc_l = {
                                     {0,0,1,0},
                                     {0,0,1,0},
                                     {0,0,1,1},
                                     {0,0,0,0}
                                 };

    static tetris_piece_t pc_lback = {
                                         {0,0,1,0},
                                         {0,0,1,0},
                                         {0,1,1,0},
                                         {0,0,0,0}
                                     };

    static tetris_piece_t pc_s = {
                                     {0,0,0,0},
                                     {0,0,1,1},
                                     {0,1,1,0},
                                     {0,0,0,0}
                                 };

    static tetris_piece_t pc_sback = {
                                         {0,0,0,0},
                                         {0,1,1,0},
                                         {0,0,1,1},
                                         {0,0,0,0}
                                     };

    tetris_piece_t* p_piece = (tetris_piece_t*) malloc (sizeof(tetris_piece_t));

    switch (t) {
    case TETRIS_PT_LINE:
        memcpy(p_piece, &pc_line, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_T:
        memcpy(p_piece, &pc_t, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_SQUARE:
        memcpy(p_piece, &pc_square, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_L:
        memcpy(p_piece, &pc_l, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_LBACK:
        memcpy(p_piece, &pc_lback, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_S:
        memcpy(p_piece, &pc_s, sizeof(tetris_piece_t));
        break;
    case TETRIS_PT_SBACK:
        memcpy(p_piece, &pc_sback, sizeof(tetris_piece_t));
        break;
    default:
        memset(p_piece, 0, sizeof(tetris_piece_t));
    }

    return p_piece;
}

void tetris_piece_destruct(tetris_piece_t* p_pc) {
    if (p_pc != 0)
        free(p_pc);
}

void tetris_piece_rotate(tetris_piece_t* p_pc_src, tetris_piece_t* p_pc_dst, enum tetris_piece_rotation_t r) {
    uint8_t y = 0;
    uint8_t x = 0;

    if ((p_pc_src != NULL) && (p_pc_dst != NULL)) {
        for (y = 0; y < 4; ++y) {
            for (x = 0; x < 4; x++) {
                if (r == TETRIS_PR_CLOCKWISE)
                    *p_pc_dst[x][y] = *p_pc_src[y][3-x];
                else
                    *p_pc_dst[x][y] = *p_pc_src[3-y][x];
            }
        }
    }
}
