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

int main(int argc, const char * argv[]) {
    piece* initial_pieces = getInitialPieces();
    
//    printf("Hello world\n");
    int loop;
    for (loop = 0; loop < getStartingPieceCountPerColor()*NUM_COLORS; loop++) {
        printf("piece: %d %d %d %d\n", initial_pieces[loop].color, initial_pieces[loop].point.x, initial_pieces[loop].point.y, initial_pieces[loop].type);
    }
//    free(initial_pieces);
    
    print_board(initial_pieces, getStartingPieceCountPerColor()*NUM_COLORS);
    
    initial_pieces[0].point.x = 2;
    initial_pieces[0].point.y = 2;
    
    initial_pieces[1].point.x = 3;
    initial_pieces[1].point.y = 3;
    
    initial_pieces[2].point.x = -5;
    initial_pieces[2].point.y = -4;
//    initial_pieces[0].point.y = 2;
//    printf("%d", max(initial_pieces, sizeof(piece), getX));
    
    print_board(initial_pieces, getStartingPieceCountPerColor()*NUM_COLORS);
    
    free(initial_pieces);
    
    return 0;
}
