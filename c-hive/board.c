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
    
    piece placed_pieces[num_pieces];
    int num_placed_pieces = 0;
    
    for (cur_piece_index = 0; cur_piece_index < num_pieces; cur_piece_index++) {
        piece cur_piece = *(pieces + cur_piece_index);
        // not yet placed
        if (cur_piece.point == NULL) {
            continue;
        }
        
        int cur_x = cur_piece.point->x;
        int cur_y = cur_piece.point->y;
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
        piece* cur_piece = (placed_pieces + cur_piece_index);
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
        neighbor_list_head = prepend_point_to_list(neighbor_list_head, neighbor);
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

//
/**
 path_so_far contains all points on the path leading up to p, includes the initial position (if p != initial position), and must be sorted from most recent to least recent
 
 N.b. the visited set tracks spaces we've seen before on any branch of the DFS. When there is a bound to the number of hops a piece can make (i.e. the spider), each node in this list uses the payload field to track the number of hops when this space was visited before. If the space has been visited with the same payload, it's not visited again. We must add this payload check to the spider's case as well because re-visiting the same space with a different number of hops taken to arrive at that space will produce different moves.
 When there isn't a bound to the number of hops a piece can make (i.e. the ant), the payload is always set to 0. In that case, if we've ever seen this space before, it's safe to ignore it in all future encounters.
 
 Note that the visited check is different from the backtracking check - the visited check applies to all branches of the DFS, while backtracking just applies to path_so_far. Both checks are necessary - the above payload logic and their treatment of the initial position are two reasons why.
 */
point_list_node* get_possible_moves_helper(point p, point_list_node* path_so_far, point_list_node** visited, point_list_node* other_piece_positions, int min_hops_inclusive, int max_hops_inclusive) {
    point_list_node* next_moves = NULL;

    int hops_so_far = size(path_so_far);
    if (hops_so_far >= min_hops_inclusive) {
        next_moves = create_list_and_point(p);
        if (hops_so_far == max_hops_inclusive) {
            return next_moves;
        }
    }
    
    
    point_list_node* occupied_neighbors_at_prev_space = get_occupied_neighbors(p, other_piece_positions);
    point_list_node* next_hop_spaces = get_neighbors(p);
    point_list_node* cur_space = next_hop_spaces;
    while (cur_space != NULL) {
        point cur_point = *(cur_space->point);
        
        point_list_node* occupied_neighbors = get_occupied_neighbors(cur_point, other_piece_positions);
        // see comment on method for decsription of payload use
        int visited_node_payload = max_hops_inclusive == INT_MAX ? 0 : hops_so_far;
        
        /**
         Space is ineligible if:
         1) it backtracks on the path the piece is taking as part of this move
         2) another piece is present at this location
         3) this space doesn't have any neighbors (piece must remain in direct contact with the hive)
         4) it does not remain in direct contact with a piece that it was previously in direct contact with (we use this as a way to tell if the piece is "moving around" its neighbors instead of jumping "across" and acquiring an entirely new set of neighbors)
         5) it hasn't been visited before on any branch of the DFS with the same payload - see comment on method for full description of payload and visited set
         */
        if (!contains_point(path_so_far, cur_point)
            && !contains_point(other_piece_positions, cur_point)
            && !is_empty(occupied_neighbors)
            && intersect(occupied_neighbors_at_prev_space, occupied_neighbors)
            && !contains_point_and_payload(*visited, cur_point, visited_node_payload)) {

            point_list_node* visited_node = create_list_with_payload_and_point(cur_point, visited_node_payload);
            append_node_to_list(visited, visited_node);
            
            point_list_node* new_path_so_far = prepend_point_to_list(path_so_far, create_point(p));
            point_list_node* next_moves_for_space = get_possible_moves_helper(cur_point, new_path_so_far, visited, other_piece_positions, min_hops_inclusive, max_hops_inclusive);
            
            // maybe faster to prepend to next_moves?
             next_moves = append_list(next_moves, next_moves_for_space);
            
            destroy_node_and_point(new_path_so_far);
        }
        
        destroy_list(occupied_neighbors);
        cur_space = cur_space->next;
    }
    
    destroy_list(occupied_neighbors_at_prev_space);
    destroy_list(next_hop_spaces);
    return next_moves;
}

point_list_node* get_possible_ant_moves(point p, point_list_node* all_piece_positions) {
    point_list_node* other_piece_positions = exclude_nodes_with_point(all_piece_positions, p);
    point_list_node** visited = malloc(sizeof(point_list_node*));
    *visited = NULL;
    
    point_list_node* moves = get_possible_moves_helper(p, NULL, visited, other_piece_positions, 1, INT_MAX);
    
    destroy_list(*visited);
    free(visited);
    destroy_list(other_piece_positions);
    return moves;
}

point_list_node* get_possible_spider_moves(point p, point_list_node* all_piece_positions) {
    point_list_node* other_piece_positions = exclude_nodes_with_point(all_piece_positions, p);
    point_list_node** visited = malloc(sizeof(point_list_node*));
    *visited = NULL;
    
    point_list_node* moves = get_possible_moves_helper(p, NULL, visited, other_piece_positions, NUM_SPIDER_HOPS, NUM_SPIDER_HOPS);
    destroy_list(*visited);
    free(visited);
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
                    possible_moves = prepend_point_to_list(possible_moves, move);
                }
                break;
            }
            is_first_iteration = false;
        }
    }
    
    return possible_moves;
}

// TODO handle beetles and z axis and shit
// n.b. not adjacent to self
bool** to_adjacency_matrix(piece* pieces) {
    int num_pieces = get_num_pieces();
    bool** adjacency_matrix = malloc(sizeof(bool*) * num_pieces);
    for (int i = 0; i < num_pieces; i++) {
        adjacency_matrix[i] = malloc(sizeof(bool) * num_pieces);
        for (int j = 0; j < num_pieces; j++) {
            adjacency_matrix[i][j] = false;
        }
    }
    
    point_list_node* placed_piece_positions = to_point_list(pieces, num_pieces);
    point_list_node* cur_piece = placed_piece_positions;
    while (cur_piece != NULL) {
        int cur_piece_id = *(cur_piece->payload);
        point_list_node* neighbors = get_neighbors(*(cur_piece->point));
        
        point_list_node* cur_neighbor = neighbors;
        while (cur_neighbor != NULL) {
            // TODO find first may not be appropriate since there may be more adjacent pieces because of stacking, need to decide how to handle that...
            point_list_node* piece_at_neighbor = find_first(cur_piece->next, *(cur_neighbor->point));
            if (piece_at_neighbor != NULL) {
                int other_piece_id = *(piece_at_neighbor->payload);
                adjacency_matrix[cur_piece_id][other_piece_id] = true;
                adjacency_matrix[other_piece_id][cur_piece_id] = true;
            }
            cur_neighbor = cur_neighbor->next;
        }
        
        free(neighbors);
        cur_piece = cur_piece->next;
    }
    
    free(placed_piece_positions);
    return adjacency_matrix;
}

int get_num_adjacent(bool** adjacency_matrix, int piece_id) {
    int count = 0;
    for (int i = 0; i < get_num_pieces(); i++) {
        if (adjacency_matrix[piece_id][i]) {
            count++;
        }
    }
    return count;
}

void cut_pieces_helper(int cur_piece_id, int cur_discovery_time, bool** adjacency_matrix, bool** visited, int **low, int **discovery_time, int **parent, bool** cut_pieces) {
    (*visited)[cur_piece_id] = true;
    (*discovery_time)[cur_piece_id] = cur_discovery_time;
    (*low)[cur_piece_id] = cur_discovery_time;

    int num_children = 0;
    int min_child_low = INT_MAX;
    for (int i = 0; i < get_num_pieces(); i++) {
        if (adjacency_matrix[cur_piece_id][i]) {
            int adjacent_piece_id = i;
            bool is_parent = !(*visited)[adjacent_piece_id];
            
            if (is_parent) {
                (*parent)[adjacent_piece_id] = cur_piece_id;
                num_children++;
                cut_pieces_helper(adjacent_piece_id, cur_discovery_time + 1, adjacency_matrix, visited, low, discovery_time, parent, cut_pieces);
            }
            
            int adjacent_discovery_time = (*discovery_time)[adjacent_piece_id];
            int adjacent_low = (*low)[adjacent_piece_id];
            int cur_low = (*low)[cur_piece_id];
            int cur_parent = (*parent)[cur_piece_id];
            
            if (is_parent) {
                min_child_low = adjacent_low < min_child_low ? adjacent_low : min_child_low;
            }
            
            if (cur_discovery_time < adjacent_discovery_time) {
                // forward edge
                (*low)[cur_piece_id] = adjacent_low < cur_low ? adjacent_low : cur_low;
            } else if (cur_parent != adjacent_piece_id) {
                // back edge
                (*low)[cur_piece_id] = adjacent_discovery_time < cur_low ? adjacent_discovery_time : cur_low;
            }
        }
    }
    
    if (cur_discovery_time == 0) {
        (*cut_pieces)[cur_piece_id] = num_children > 1;
    } else {
        (*cut_pieces)[cur_piece_id] = min_child_low != INT_MAX && min_child_low > cur_discovery_time;
    }    
}

// returns array of length num_placed_pieces,
bool* get_hive_cut_pieces(piece* pieces) {
    int num_pieces = get_num_pieces();
    
    bool* cut_pieces = malloc(num_pieces*sizeof(bool));
    bool** adjacency_matrix = to_adjacency_matrix(pieces);
    bool* visited = malloc(num_pieces*sizeof(bool));
    int* parent = malloc(num_pieces*sizeof(int));
    int* low = malloc(num_pieces*sizeof(int));
    int* discovery_time = malloc(num_pieces*sizeof(int));
    for (int i = 0; i < num_pieces; i++) {
        cut_pieces[i] = false;
        visited[i] = false;
        parent[i] = -1;
        low[i] = -1;
        discovery_time[i] = -1;
    }
    
    for (int i = 0; i < num_pieces; i++) {
        if (pieces[i].point == NULL) {
            continue;
        }
        if (!visited[i]) {
            cut_pieces_helper(i, 0, adjacency_matrix, &visited, &low, &discovery_time, &parent, &cut_pieces);
        }
    }
    
    free(visited);
    free(parent);
    free(low);
    free(discovery_time);
    for (int i = 0; i < num_pieces; i++) {
        free(adjacency_matrix[i]);
    }
    free(adjacency_matrix);
    return cut_pieces;
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
        case ANT:
            return get_possible_ant_moves(piece_to_move_pos, all_piece_positions);
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
