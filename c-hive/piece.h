//
//  piece.h
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#ifndef piece_h
#define piece_h
#include <stdio.h>
#include "util.h"
#include "limits.h"

typedef enum {
    WHITE,
    BLACK,
    NUM_COLORS
} color;

// order-dependent with STARTING_PIECE_COUNT
typedef enum {
    GRASSHOPER,
    SPIDER,
    QUEEN,
    ANT,
    BEETLE,
    NUM_PIECE_TYPES
} piece_type;
static int PIECE_TYPE_TO_INITIAL_COUNT_PER_COLOR[NUM_PIECE_TYPES] = {3, 2, 1, 3, 2};
static char PIECE_TYPE_TO_CHAR[NUM_PIECE_TYPES] = {'G', 'S', 'Q', 'A', 'B'};
static int NUM_SPIDER_HOPS = 3;


typedef struct {
    int x;
    int y;
} point;

typedef struct {
    color color;
    piece_type type;
    int piece_id;
    // null for not yet placed
    point* point;
} piece;

void destroy_piece(piece* piece);

piece* get_initial_pieces(void);
int get_count_per_color(void);
int get_num_pieces(void);


#endif /* piece_h */
