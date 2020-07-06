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

point_list_node* get_occupied_neighbors(point p, point_list_node* piece_positions) {
    point_list_node* neighbors = get_neighbors(p);
    point_list_node* occupied_neighbors = intersect_lists(neighbors, piece_positions);
    destroy_list(neighbors);
    return occupied_neighbors;
}

// TODO filter out places where other pieces are
point_list_node* get_possible_queen_moves(point p) {
    return get_neighbors(p);
}

point_list_node* get_possible_beetle_moves(point p) {
    return get_neighbors(p);
}

// spider_path contains all points on the path leading up to p, includes the initial position (if p != initial position), and must be sorted from most recent to least recent
point_list_node* get_possible_spider_moves_helper(point p, point_list_node* spider_path, point_list_node* other_piece_positions) {
    if (size(spider_path) == 3) {
        return create_list_and_point(p);
    }
    
    point_list_node* next_moves = NULL;
    
    point_list_node* occupied_neighbors_at_prev_space = get_occupied_neighbors(p, other_piece_positions);
    point_list_node* next_spider_spaces = get_neighbors(p);
    point_list_node* cur_space = next_spider_spaces;
    while (cur_space != NULL) {
        point cur_point = *(cur_space->point);
        point_list_node* occupied_neighbors = get_occupied_neighbors(cur_point, other_piece_positions);
        
        /**
         Space is ineligible if:
         1) it backtracks on the path the spider is taking as part of this move
         2) another piece is present at this location
         3) this space doesn't have any neighbors (spider must remain in direct contact with the hive)
         4) it does not remain in direct contact with a piece that it was previously in direct contact with (we use this as a way to tell if the spider is "moving around" its neighbors instead of jumping "across" and acquiring an entirely new set of neighbors)
         */
        if (!contains_point(spider_path, cur_point)
            && !contains_point(other_piece_positions, cur_point)
            && !is_empty(occupied_neighbors)
            // TODO - deal with empty list intersection
            && intersect(occupied_neighbors_at_prev_space, occupied_neighbors)) {
            
            point_list_node* new_spider_path = prepend_point_list(spider_path, create_point(p));
            point_list_node* next_moves_for_space = get_possible_spider_moves_helper(cur_point, new_spider_path, other_piece_positions);
            
            // maybe faster to prepend to next_moves?
            next_moves = append_list(next_moves, next_moves_for_space);
            
            destroy_node_and_point(new_spider_path);
        }
        
        destroy_list(occupied_neighbors);
        cur_space = cur_space->next;
    }
    
    destroy_list(occupied_neighbors_at_prev_space);
    destroy_list(next_spider_spaces);
    return next_moves;
}

point_list_node* get_possible_spider_moves(point p, point_list_node* all_piece_positions) {
    point_list_node* other_piece_positions = exclude_nodes_with_point(all_piece_positions, p);
    point_list_node* moves = get_possible_spider_moves_helper(p, NULL, other_piece_positions);
    destroy_list(other_piece_positions);
    return moves;
}

point_list_node* get_possible_grasshopper_moves(point p, point_list_node* all_piece_positions) {
    point_list_node* possible_moves = NULL;
    
    for (int cur_board_dir = 0; cur_board_dir < NUM_BOARD_DIR; cur_board_dir++) {
        point cur_hop_pos = p;
        point* new_hop_pos;
        bool is_first_iteration = true;
        while (true) {
            new_hop_pos = translate_point(cur_hop_pos, cur_board_dir);
            cur_hop_pos = *new_hop_pos;
            free(new_hop_pos);

            bool piece_at_cur_hop_pos = contains_point(all_piece_positions, cur_hop_pos);
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


point_list_node* get_possible_moves(piece* piece_to_move, point_list_node* all_piece_positions) {
    point piece_to_move_pos = *(piece_to_move->point);
    switch (piece_to_move->type) {
        case QUEEN:
            return get_possible_queen_moves(piece_to_move_pos);
        case BEETLE:
            return get_possible_beetle_moves(piece_to_move_pos);
        case GRASSHOPER:
            return get_possible_grasshopper_moves(piece_to_move_pos, all_piece_positions);
        case SPIDER:
            return get_possible_spider_moves(piece_to_move_pos, all_piece_positions);
        default:
            return NULL;
    }
}

// assumes piece_to_move has been placed already
point_list_node* get_legal_moves(piece* piece_to_move, piece* placed_pieces, int num_placed_pieces) {
    point_list_node* cur_piece_positions = to_point_list(placed_pieces, num_placed_pieces);
    point_list_node* possible_moves = get_possible_moves(piece_to_move, cur_piece_positions);
    
    return possible_moves;
}
