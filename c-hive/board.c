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
#include <stdlib.h>


static int BOARD_PADDING = 2;

//int get_x(void* void_piece) {
//    piece* as_piece = (piece*) void_piece;
//    return as_piece->point.x;
//}
//
//int get_y(void* void_piece) {
//    piece* as_piece = (piece*) void_piece;
//    return as_piece->point.y;
//}
//
//bool is_placed(int x, int y) {
//    return x != NOT_PLACED.x || y != NOT_PLACED.y;
//}

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
        // not yet placed
        if (cur_piece->point == NULL) {
            continue;
        }
        
        int cur_x = cur_piece->point->x;
        int cur_y = cur_piece->point->y;
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
    
    char board_strings[y_range][x_range];
    
    int cur_x, cur_y;
    for (cur_y = 0; cur_y < y_range; cur_y++) {
        for (cur_x = 0; cur_x < x_range; cur_x++) {
            board_strings[cur_y][cur_x] = '*';
        }
    }
        
    for (cur_piece_index = 0; cur_piece_index < num_placed_pieces; cur_piece_index++) {
        piece* cur_piece = *(placed_pieces + cur_piece_index);
        // piece positions can go negative, but our strings are always 0-indexed so we must
        // translate
        board_strings[cur_piece->point->y - min_display_y][cur_piece->point->x - min_display_x] = PIECE_TYPE_TO_CHAR[cur_piece->type];
    }
    
    for (cur_y = 0; cur_y < y_range; cur_y++) {
        if ((cur_y % 2) != 0) {
            printf("  ");
        }
        for (cur_x = 0; cur_x < x_range; cur_x++) {
            printf("%c", board_strings[cur_y][cur_x]);
            printf("   ");
        }
        printf("\n");
    }
    printf("\n");
}

// returns new head
point_list_node* prepend_point_list(point_list_node* head, point* p) {
    point_list_node* new_head = malloc(sizeof(point_list_node));
    new_head->point = p;
    new_head->next = head == NULL ? NULL : head;
    return new_head;
}

bool contains_point(point_list_node* head, point p) {
    point_list_node* cur_node = head;
    while (cur_node != NULL) {
        if (cur_node->point->x == p.x && cur_node->point->y == p.y) {
            return true;
        }
        cur_node = cur_node->next;
    }
    return false;
}

point* translate_point(point p, board_dir dir) {
    int x = p.x;
    int y = p.y;
    int new_x = 0, new_y = 0;
    switch (dir) {
        case L:
            new_x = x - 1;
            new_y = y;
            break;
        case R:
            new_x = x + 1;
            new_y = y;
            break;
        case LT:
            new_y = y - 1;
            new_x = (y % 2) != 0 ? x : x - 1;
            break;
        case RT:
            new_y = y - 1;
            new_x = (y % 2) != 0 ? x + 1 : x;
            break;
        case LB:
            new_y = y + 1;
            new_x = (y % 2) != 0 ? x : x - 1;
            break;
        case RB:
            new_y = y + 1;
            new_x = (y % 2) != 0 ? x + 1 : x;
            break;
        default:
            break;
    }
    point* translated_point = malloc(sizeof(point));
    translated_point->x = new_x;
    translated_point->y = new_y;
    return translated_point;
}

point_list_node* get_neighbors(point p) {
    point_list_node* neighbor_list_head = NULL;
    
    for (int cur_board_dir = 0; cur_board_dir < NUM_BOARD_DIR; cur_board_dir++) {
        point* neighbor = translate_point(p, cur_board_dir);
        neighbor_list_head = prepend_point_list(neighbor_list_head, neighbor);
    }
    
    return neighbor_list_head;
}

point_list_node* get_possible_queen_moves(point p) {
    return get_neighbors(p);
}

point_list_node* get_possible_grasshopper_moves(point p, point_list_node* other_piece_positions) {
    point_list_node* possible_moves = NULL;
    
    for (int cur_board_dir = 0; cur_board_dir < NUM_BOARD_DIR; cur_board_dir++) {
        point cur_hop_pos = p;
        point* new_hop_pos;
        bool is_first_iteration = true;
        while (true) {
            new_hop_pos = translate_point(cur_hop_pos, cur_board_dir);
            cur_hop_pos = *new_hop_pos;
            free(new_hop_pos);

            bool piece_at_cur_hop_pos = contains_point(other_piece_positions, cur_hop_pos);
            if (!piece_at_cur_hop_pos) {
                // otherwise, no initial piece to jump over
                if (!is_first_iteration) {
                    point* move = malloc(sizeof(point));
                    move->x = cur_hop_pos.x;
                    move->y = cur_hop_pos.y;
                    possible_moves = prepend_point_list(possible_moves, move);
                }
                break;
            }
            is_first_iteration = false;
        }
    }
    
    return possible_moves;
}


point_list_node* get_possible_moves(piece* piece_to_move) {
    switch (piece_to_move->type) {
        case QUEEN:
            return get_possible_queen_moves(*(piece_to_move->point));
        default:
            return NULL;
    }
}

// assumes piece_to_move has been placed already
point_list_node* get_legal_moves(piece* piece_to_move, piece* placed_pieces, int num_placed_pieces) {
    point_list_node* possible_moves = get_possible_moves(piece_to_move);
    
    return possible_moves;
}





//int compare( const void* a, const void* b){
//     int int_a = * ( (int*) a );
//     int int_b = * ( (int*) b );
//
//     if ( int_a == int_b ) return 0;
//     else if ( int_a < int_b ) return -1;
//     else return 1;
//}
