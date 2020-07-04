//
//  main.c
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#include <stdio.h>
#include "piece.h"
#include "board.h"
#include "util.h"
#include <stdlib.h>

void print_point_list(point_list_node* list) {
    while (list != NULL) {
        printf("point: %d %d\n", list->point->x, list->point->y);
        list = list->next;
    }
}

int main(int argc, const char * argv[]) {
    piece* initial_pieces = getInitialPieces();
    
//    printf("Hello world\n");
    int loop;
    for (loop = 0; loop < getStartingPieceCountPerColor()*NUM_COLORS; loop++) {
        printf("piece: %d %d %d\n", initial_pieces[loop].color, initial_pieces[loop].point, initial_pieces[loop].type);
    }
//    free(initial_pieces);
    
    print_board(initial_pieces, getStartingPieceCountPerColor()*NUM_COLORS);
    
    initial_pieces[0].point = & (point) {.x =  2, .y = 2};
    initial_pieces[1].point = & (point) {.x =  3, .y = 3};
    initial_pieces[15].point = & (point) {.x =  -5, .y = -4};
    
//    initial_pieces[0].point.y = 2;
//    printf("%d", max(initial_pieces, sizeof(piece), getX));
    
    print_board(initial_pieces, getStartingPieceCountPerColor()*NUM_COLORS);
    
    free(initial_pieces);
    
    point_list_node* neighbors = get_neighbors((point) {.x = 1, .y = 1});
    print_point_list(neighbors);
    
    printf("\n");
    
    neighbors = get_neighbors((point) {.x = 1, .y = 2});
    print_point_list(neighbors);
    
    printf("\n Testing ghop moves \n");

    
    point_list_node* other_piece_positions = & (point_list_node){
        .point = & (point) {.x =  2, .y = 1},
        .next = & (point_list_node){
            .point = & (point) {.x =  2, .y = 3},
            .next = & (point_list_node){
                .point = & (point) {.x =  3, .y = 4},
                .next = & (point_list_node){
                    .point = & (point) {.x =  -1, .y = -1},
                    .next = NULL
                },
            },
        },
    };
    point_list_node* grasshoper_moves = get_possible_grasshopper_moves((point) {.x = 2, .y = 2}, other_piece_positions);
    print_point_list(grasshoper_moves);
    
    free(neighbors);
    
    return 0;
}
