//
//  piece.c
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#include "piece.h"
#include <stdlib.h>


int getStartingPieceCountPerColor() {
    return sum(PIECE_TYPE_TO_INITIAL_COUNT_PER_COLOR, NUM_PIECE_TYPES);
}

piece* getInitialPieces() {
    int total_count_per_player = getStartingPieceCountPerColor();
    piece *initial_pieces = malloc(sizeof(piece) * total_count_per_player);

    int cur_color = 0;
    int cur_piece_type = 0;
    int cur_piece_type_num = 0;
    int cur_index = 0;
    for (cur_color = 0; cur_color < NUM_COLORS; cur_color++) {
        for (cur_piece_type = 0; cur_piece_type < NUM_PIECE_TYPES; cur_piece_type++) {
            for (cur_piece_type_num = 0; cur_piece_type_num < PIECE_TYPE_TO_INITIAL_COUNT_PER_COLOR[cur_piece_type]; cur_piece_type_num++) {
                initial_pieces[cur_index].color = cur_color;
                initial_pieces[cur_index].point = NOT_PLACED;
                initial_pieces[cur_index].type = cur_piece_type;
                initial_pieces[cur_index].piece_num = cur_piece_type_num;
                cur_index++;
            }
        }
    }
    
    return initial_pieces;
}

