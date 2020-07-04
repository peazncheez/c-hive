//
//  board.h
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#ifndef board_h
#define board_h

#include <stdio.h>
#include "piece.h"

void print_board(piece* pieces, int num_pieces);

typedef struct point_list_node {
    point* point;
    struct point_list_node* next;
} point_list_node;

/*
 CW from top right - RT, R, RB, LB, L, LT
 
 / \
 | |
 \ /
 */
typedef enum {
    L,
    R,
    LT,
    RT,
    LB,
    RB,
    NUM_BOARD_DIR,
} board_dir;

// visible for testing
point_list_node* get_neighbors(point p);

// visible for testing
point_list_node* get_possible_grasshopper_moves(point p, point_list_node* other_piece_positions);

#endif /* board_h */
