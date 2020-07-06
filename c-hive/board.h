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
#include "point_list.h"

void print_board(piece* pieces, int num_pieces);

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
static int BOARD_DIR_TO_OPPOSITE_BOARD_DIR[NUM_BOARD_DIR] = {1, 0, 5, 4, 3, 2};

// visible for testing
point_list_node* get_neighbors(point p);

// visible for testing
point_list_node* get_possible_grasshopper_moves(point p, point_list_node* other_piece_positions);

// visible for testing
point_list_node* get_possible_spider_moves(point p, point_list_node* other_piece_positions);

int size(point_list_node* list);

bool contains_point(point_list_node* head, point p);

#endif /* board_h */
