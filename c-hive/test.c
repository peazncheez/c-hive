//
//  test.c
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#include <stdio.h>
#include "piece.h"
#include "board.h"
#include "util.h"
#include "point_list.h"
#include <stdlib.h>
#include "stdbool.h"

void print_point_list(point_list_node* list) {
    while (list != NULL) {
        printf("point: %d %d\n", list->point->x, list->point->y);
        list = list->next;
    }
}

bool is_equal_ignore_order(point_list_node* list_1, point_list_node* list_2) {
    if (list_1 == NULL) {
        return list_2 == NULL;
    }
    if (size(list_1) != size(list_2)) {
        return false;
    }
    while (list_1 != NULL) {
        if (!contains_point(list_2, *(list_1->point))) {
            return false;
        }
        list_1 = list_1->next;
    }
    return true;
}

bool test_equal_ignore_order(point_list_node *expected, point_list_node *actual, char* fail_message) {
    if (!is_equal_ignore_order(expected, actual)) {
        printf("%s - returned: ", fail_message);
        print_point_list(actual);
        return false;
    }
    return true;
}

bool test_get_neighbors() {
    printf("Testing get neighbors\n");
    point_list_node* neighbors = get_neighbors((point) {.x = 1, .y = 1});
    point_list_node* expected_neighbors = & (point_list_node){
        .point = & (point) {.x =  2, .y = 2},
        .next = & (point_list_node){
            .point = & (point) {.x =  1, .y = 2},
            .next = & (point_list_node){
                .point = & (point) {.x =  2, .y = 0},
                .next = & (point_list_node){
                    .point = & (point) {.x =  1, .y = 0},
                    .next = & (point_list_node){
                        .point = & (point) {.x =  2, .y = 1},
                        .next = & (point_list_node){
                            .point = & (point) {.x =  0, .y = 1},
                            .next = NULL
                        },
                    },
                },
            },
        },
    };
    
    if (!test_equal_ignore_order(expected_neighbors, neighbors, "Failed neighbors for 1,1")) {
        destroy_list(neighbors);
        return false;
    }

    destroy_list(neighbors);
    
    neighbors = get_neighbors((point) {.x = 1, .y = 2});
    expected_neighbors = & (point_list_node){
        .point = & (point) {.x =  1, .y = 3},
        .next = & (point_list_node){
            .point = & (point) {.x =  0, .y = 3},
            .next = & (point_list_node){
                .point = & (point) {.x =  1, .y = 1},
                .next = & (point_list_node){
                    .point = & (point) {.x =  0, .y = 1},
                    .next = & (point_list_node){
                        .point = & (point) {.x =  2, .y = 2},
                        .next = & (point_list_node){
                            .point = & (point) {.x =  0, .y = 2},
                            .next = NULL
                        },
                    },
                },
            },
        },
    };
    
    if (!test_equal_ignore_order(expected_neighbors, neighbors, "Failed neighbors for 1,2")) {
        destroy_list(neighbors);
        return false;
    }
    
    destroy_list(neighbors);
    return true;
}

bool test_grasshopper_moves() {
    printf("Testing ghop moves \n");
    
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
    
    point_list_node* expected_moves = & (point_list_node){
        .point = & (point) {.x =  3, .y = 5},
        .next = & (point_list_node){
            .point = & (point) {.x =  3, .y = 0},
            .next = NULL
        },
    };

    point_list_node* grasshopper_moves = get_possible_grasshopper_moves((point) {.x = 2, .y = 2}, other_piece_positions);
    if (!test_equal_ignore_order(expected_moves, grasshopper_moves, "Failed ghop moves for 2,2")) {
        destroy_list(grasshopper_moves);
        return false;
    }
    
    destroy_list(grasshopper_moves);
    return true;
}

bool test_spider_moves() {
    printf("Testing spider moves \n");
    
    // https://www.fgbradleys.com/rules/rules3/HiveRules.pdf
    point_list_node* other_piece_positions = & (point_list_node){
        // spider
        .point = & (point) {.x =  3, .y = 2},
        .next = & (point_list_node){
            // bee
            .point = & (point) {.x =  3, .y = 3},
            .next = & (point_list_node){
                // beetle
                .point = & (point) {.x =  3, .y = 4},
                .next = & (point_list_node){
                    // ant
                    .point = & (point) {.x =  2, .y = 5},
                    .next = & (point_list_node){
                        // grasshopper
                        .point = & (point) {.x =  1, .y = 5},
                        .next = & (point_list_node){
                            // grasshopper
                            .point = & (point) {.x =  0, .y = 5},
                            .next = & (point_list_node){
                                // bee
                                .point = & (point) {.x =  0, .y = 4},
                                .next = & (point_list_node){
                                    // ant
                                    .point = & (point) {.x =  0, .y = 3},
                                    .next = & (point_list_node){
                                        // beetle
                                        .point = & (point) {.x =  1, .y = 2},
                                        .next = NULL,
                                    },
                                },
                            },
                        },
                    },
                },
            },
        },
    };
    
    point spider_pos = {.x = 2, .y = 1};
    point_list_node* expected_moves = & (point_list_node){
        .point = & (point) {.x =  1, .y = 4},
        .next = & (point_list_node){
            .point = & (point) {.x =  2, .y = 4},
            .next = & (point_list_node){
                .point = & (point) {.x =  0, .y = 1},
                .next = & (point_list_node){
                    .point = & (point) {.x =  4, .y = 3},
                    .next = NULL
                },

            },
        },
    };

    point_list_node* spider_moves = get_possible_spider_moves(spider_pos, other_piece_positions);
    if (!test_equal_ignore_order(expected_moves, spider_moves, "Failed spider moves for 2,1")) {
        destroy_list(spider_moves);
        return false;
    }
    destroy_list(spider_moves);
    
    // left of the white queen
    spider_pos = (point) {.x = 2, .y = 3};
    expected_moves = & (point_list_node){
        .point = & (point) {.x =  1, .y = 3},
        .next = & (point_list_node){
            .point = & (point) {.x =  0, .y = 1},
            .next = & (point_list_node){
                .point = & (point) {.x =  3, .y = 1},
                .next = & (point_list_node){
                    .point = & (point) {.x =  1, .y = 4},
                    .next = NULL

                },
            },
        },
    };

    spider_moves = get_possible_spider_moves(spider_pos, other_piece_positions);
    if (!test_equal_ignore_order(expected_moves, spider_moves, "Failed spider moves for 2,3")) {
        destroy_list(spider_moves);
        return false;
    }
    destroy_list(spider_moves);
    
    // test that no dupes are produced
    other_piece_positions = & (point_list_node){
        // spider
        .point = & (point) {.x =  3, .y = 2},
        .next = & (point_list_node){
            // bee
            .point = & (point) {.x =  3, .y = 3},
            .next = & (point_list_node){
                // beetle
                .point = & (point) {.x =  3, .y = 4},
                .next = & (point_list_node){
                    // ant
                    .point = & (point) {.x =  2, .y = 5},
                    .next = & (point_list_node){
                        // grasshopper
                        .point = & (point) {.x =  1, .y = 5},
                        .next = & (point_list_node){
                            // grasshopper
                            .point = & (point) {.x =  0, .y = 5},
                            .next = & (point_list_node){
                                // bee
                                .point = & (point) {.x =  0, .y = 4},
                                .next = & (point_list_node){
                                    // ant
                                    .point = & (point) {.x =  0, .y = 3},
                                    .next = & (point_list_node){
                                        // beetle
                                        .point = & (point) {.x =  1, .y = 2},
                                        .next = & (point_list_node){
                                            // made up for test
                                            .point = & (point) {.x =  1, .y = 4},
                                            .next = NULL,
                                        },
                                    },
                                },
                            },
                        },
                    },
                },
            },
        },
    };
    
    spider_pos = (point) {.x = 2, .y = 1};
    // if dupe logic weren't present, would get 2 (2, 4)s
    expected_moves = & (point_list_node){
            .point = & (point) {.x =  2, .y = 4},
            .next = & (point_list_node){
                .point = & (point) {.x =  0, .y = 1},
                .next = & (point_list_node){
                    .point = & (point) {.x =  4, .y = 3},
                    .next = NULL
                },

            },
    };

    spider_moves = get_possible_spider_moves(spider_pos, other_piece_positions);
    if (!test_equal_ignore_order(expected_moves, spider_moves, "Failed spider moves for 2,1")) {
        destroy_list(spider_moves);
        return false;
    }
    destroy_list(spider_moves);
    
    return true;
}

bool test_ant_moves() {
    printf("Testing ant moves \n");
    
    // https://www.fgbradleys.com/rules/rules3/HiveRules.pdf
    point_list_node* other_piece_positions = & (point_list_node){
        // spider
        .point = & (point) {.x =  3, .y = 3},
        .next = & (point_list_node){
            // bee
            .point = & (point) {.x =  4, .y = 3},
            .next = & (point_list_node){
                // beetle
                .point = & (point) {.x =  3, .y = 2},
                .next = & (point_list_node){
                    // ant
                    .point = & (point) {.x =  3, .y = 1},
                    .next = & (point_list_node){
                        // grasshopper
                        .point = & (point) {.x =  5, .y = 2},
                        .next = NULL,
                    },
                },
            },
        },
    };
    
    point ant_pos = {.x = 4, .y = 4};
    point_list_node* expected_moves = & (point_list_node){
        .point = & (point) {.x =  3, .y = 4},
        .next = & (point_list_node){
            .point = & (point) {.x =  2, .y = 3},
            .next = & (point_list_node){
                .point = & (point) {.x =  2, .y = 2},
                .next = & (point_list_node){
                    .point = & (point) {.x =  2, .y = 1},
                    .next = & (point_list_node){
                        .point = & (point) {.x =  3, .y = 0},
                        .next = & (point_list_node){
                            .point = & (point) {.x =  4, .y = 0},
                            .next = & (point_list_node){
                                .point = & (point) {.x =  4, .y = 1},
                                .next = & (point_list_node){
                                    .point = & (point) {.x =  5, .y = 1},
                                    .next = & (point_list_node){
                                        .point = & (point) {.x =  6, .y = 2},
                                        .next = & (point_list_node){
                                            .point = & (point) {.x =  5, .y = 3},
                                            .next = & (point_list_node){
                                                .point = & (point) {.x =  5, .y = 4},
                                                .next = & (point_list_node){
                                                    // this is illegal, need to filter out with gates
                                                    .point = & (point) {.x =  4, .y = 2},
                                                    .next = NULL
                                                },
                                            },
                                        },

                                    },
                                },
                            },

                        },
                    },
                },

            },
        },
    };

    point_list_node* ant_moves = get_possible_ant_moves(ant_pos, other_piece_positions);
    if (!test_equal_ignore_order(expected_moves, ant_moves, "Failed ant moves for 4,4")) {
        destroy_list(ant_moves);
        return false;
    }
    destroy_list(ant_moves);
    return true;
}

int main(int argc, const char * argv[]) {
    printf("Starting tests\n");
    point_list_node* a = NULL;
    printf("%d", &a);
    piece* initial_pieces = get_initial_pieces();
    int num_pieces = get_starting_piece_count_per_color()*NUM_COLORS;
    
    int loop;
    for (loop = 0; loop < get_starting_piece_count_per_color()*NUM_COLORS; loop++) {
        printf("piece: %d %d %d\n", initial_pieces[loop].color, initial_pieces[loop].point, initial_pieces[loop].type);
    }

    print_board(initial_pieces, num_pieces);

    initial_pieces[0].point = & (point) {.x =  2, .y = 2};
    initial_pieces[1].point = & (point) {.x =  3, .y = 3};
    initial_pieces[15].point = & (point) {.x =  -5, .y = -4};
    print_board(initial_pieces, num_pieces);

    // using a single free since it was allocated with a single malloc
    // also, not freeing the embedded point* since all points were not created with malloc
    free(initial_pieces);

    if (!test_get_neighbors()) {
        return 1;
    }

    if(!test_grasshopper_moves()) {
        return 1;
    }

    if(!test_spider_moves()) {
        return 1;
    }
    
    if(!test_ant_moves()) {
        return 1;
    }
    
    return 0;
}
