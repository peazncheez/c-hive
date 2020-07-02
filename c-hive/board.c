//
//  board.c
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#include "board.h"
#include "piece.h"
#include "util.h"
#include "limits.h"
#include "stdbool.h"

static int BOARD_PADDING = 3;

//int get_x(void* void_piece) {
//    piece* as_piece = (piece*) void_piece;
//    return as_piece->point.x;
//}
//
//int get_y(void* void_piece) {
//    piece* as_piece = (piece*) void_piece;
//    return as_piece->point.y;
//}

bool is_placed(int x, int y) {
    return x != NOT_PLACED.x || y != NOT_PLACED.y;
}

void print_board(piece* pieces, int num_pieces) {
    int cur_piece_index;
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;
    
    piece* placed_pieces[num_pieces];
    int num_placed_pieces = 0;
    
    for (cur_piece_index = 0; cur_piece_index < num_pieces; cur_piece_index++) {
        piece* cur_piece = (pieces + cur_piece_index);
        int cur_x = cur_piece->point.x;
        int cur_y = cur_piece->point.y;
        if (!is_placed(cur_x, cur_y)) {
            continue;
        }
        
        placed_pieces[num_placed_pieces++] = cur_piece;
        
        min_x = min_x > cur_x ? cur_x : min_x;
        min_y = min_y > cur_y ? cur_y : min_y;
        max_x = cur_x > max_x ? cur_x : max_x;
        max_y = cur_y > max_y ? cur_y : max_y;
    }
    
    if (num_placed_pieces == 0) {
        min_x = 0;
        min_y = 0;
        max_x = 0;
        max_y = 0;
    }
    
    int min_display_x = min_x - BOARD_PADDING;
    int min_display_y = min_y - BOARD_PADDING;
    int max_display_x = max_x + BOARD_PADDING;
    int max_display_y = max_y + BOARD_PADDING;
    
    int y_range = max_display_y - min_display_y;
    int x_range = max_display_x - min_display_x;
    
    char BOARD_STRINGS[y_range][x_range];
    
    int cur_x, cur_y;
    for (cur_y = 0; cur_y < y_range; cur_y++) {
        for (cur_x = 0; cur_x < x_range; cur_x++) {
            BOARD_STRINGS[cur_y][cur_x] = '*';
        }
    }
        
    for (cur_piece_index = 0; cur_piece_index < num_placed_pieces; cur_piece_index++) {
        piece* cur_piece = *(placed_pieces + cur_piece_index);
        // piece positions can go negative, but our strings are always 0-indexed so we must
        // translate
        BOARD_STRINGS[cur_piece->point.y - min_display_y][cur_piece->point.x - min_display_x] = PIECE_TYPE_TO_CHAR[cur_piece->type];
    }
    
    for (cur_y = 0; cur_y < y_range; cur_y++) {
        if ((cur_y % 2) != 0) {
            printf("  ");
        }
        for (cur_x = 0; cur_x < x_range; cur_x++) {
            printf("%c", BOARD_STRINGS[cur_y][cur_x]);
            printf("   ");
        }
        printf("\n");
    }
    printf("\n");
}

//int compare( const void* a, const void* b){
//     int int_a = * ( (int*) a );
//     int int_b = * ( (int*) b );
//
//     if ( int_a == int_b ) return 0;
//     else if ( int_a < int_b ) return -1;
//     else return 1;
//}
