//
//  point_list.h
//  c-hive
//
//  Created by Bryan Williams on 7/5/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#ifndef point_list_h
#define point_list_h

#include <stdio.h>
#include "piece.h"
#include "stdbool.h"

typedef struct point_list_node {
    point* point;
    struct point_list_node* next;
    // optional associated data
    int* payload;
} point_list_node;

point* create_point(point p);

point_list_node* create_list(point* p);

point_list_node* create_list_and_point(point p);

point_list_node* create_list_with_payload_and_point(point p, int payload);

void destroy_node_and_point(point_list_node* node);

void destroy_list(point_list_node* head);

point_list_node* prepend_point_to_list(point_list_node* head, point* p);

void append_node_to_list(point_list_node** head, point_list_node* p);

point_list_node* append_list(point_list_node* list_1, point_list_node* list_2);

point_list_node* get_tail(point_list_node* head);

bool contains_point(point_list_node* head, point p);

bool contains_point_and_payload(point_list_node* head, point p, int payload);

bool intersect(point_list_node* list_1, point_list_node* list_2);

point_list_node* intersect_lists(point_list_node* list_1, point_list_node* list_2);

//point_list_node* remove_node_and_get_next(point_list_node* prev_node, point_list_node* cur_node);

point_list_node* exclude_nodes_with_point(point_list_node* head, point p);

int size(point_list_node* list);

bool is_empty(point_list_node* list);

point_list_node* to_point_list(piece* pieces, int num_pieces);

#endif /* point_list_h */
