//
//  point_list.c
//  c-hive
//
//  Created by Bryan Williams on 7/5/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#include "point_list.h"
#include "piece.h"
#include <stdlib.h>

point* create_point(point p) {
    point* p_ptr = malloc(sizeof(point));
    p_ptr->x = p.x;
    p_ptr->y = p.y;
    return p_ptr;
}

point_list_node* create_list(point* p) {
    point_list_node* new_head = malloc(sizeof(point_list_node));
    new_head->point = p;
    new_head->next = NULL;
    new_head->payload = NULL;
    return new_head;
}

point_list_node* create_list_and_point(point p) {
    return create_list(create_point(p));
}

void add_payload(point_list_node* node, int payload) {
    int* payload_ptr = malloc(sizeof(int));
    *payload_ptr = payload;
    node->payload = payload_ptr;
}

point_list_node* create_list_with_payload_and_point(point p, int payload) {
    point_list_node* node = create_list_and_point(p);
    add_payload(node, payload);
    return node;
}

void destroy_node_and_point(point_list_node* node) {
    if (node != NULL) {
        point* p = node->point;
        if (p != NULL) {
            free(p);
            p = NULL;
        }
        int* payload = node->payload;
        if (payload != NULL) {
            free(payload);
            payload = NULL;
        }
        free(node);
        node = NULL;
    }
}

void destroy_list(point_list_node* head) {
    if (head == NULL) {
        return;
    }
    destroy_list(head->next);
    destroy_node_and_point(head);
}

// returns new head
point_list_node* prepend_point_to_list(point_list_node* head, point* p) {
    point_list_node* new_head = create_list(p);
    new_head->next = head == NULL ? NULL : head;
    return new_head;
}

void append_node_to_list(point_list_node** head, point_list_node* new_tail) {
    if (*head == NULL) {
        *head = new_tail;
    } else {
        point_list_node* old_tail = get_tail(*head);
        old_tail->next = new_tail;
    }
}

point_list_node* append_list(point_list_node* list_1, point_list_node* list_2) {
    if (list_2 == NULL) {
        return list_1;
    }
    if (list_1 == NULL) {
        return list_2;
    }
    
    point_list_node* list_1_tail = get_tail(list_1);
    list_1_tail->next = list_2;
    return list_1;
}

point_list_node* get_tail(point_list_node* head) {
    point_list_node* cur_node = head;
    if (cur_node == NULL) {
        return NULL;
    }
    while (cur_node->next != NULL) {
        cur_node = cur_node->next;
    }
    return cur_node;
}

bool is_equal(point p_1, point p_2) {
    return p_1.x == p_2.x && p_1.y == p_2.y;
}

bool contains_point(point_list_node* head, point p) {
    point_list_node* cur_node = head;
    while (cur_node != NULL) {
        if (is_equal(*(cur_node->point), p)) {
            return true;
        }
        cur_node = cur_node->next;
    }
    return false;
}

bool contains_point_and_payload(point_list_node* head, point p, int payload) {
    point_list_node* cur_node = head;
    while (cur_node != NULL) {
        if (is_equal(*(cur_node->point), p) && payload == *(cur_node->payload)) {
            return true;
        }
        cur_node = cur_node->next;
    }
    return false;
}

// returns true if the two lists have a non-empty intersection (have at least one element in common)
bool intersect(point_list_node* list_1, point_list_node* list_2) {
    if (list_1 == NULL || list_2 == NULL) {
        return false;
    }
    point_list_node* cur_list_1_node = list_1;
    while (cur_list_1_node != NULL) {
        if (contains_point(list_2, *(cur_list_1_node->point))) {
            return true;
        }
        cur_list_1_node = cur_list_1_node->next;
    }
    return false;
}

point_list_node* intersect_lists(point_list_node* list_1, point_list_node* list_2) {
    point_list_node* new_head = NULL;
    
    while (list_1 != NULL) {
        point cur_point = *(list_1->point);
        if (contains_point(list_2, cur_point)) {
            new_head = prepend_point_to_list(new_head, create_point(cur_point));
        }
        list_1 = list_1->next;
    }
    
    return new_head;
}

point_list_node* find_first(point_list_node* list, point p) {
    while (list != NULL) {
        if (is_equal(*(list->point), p)) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

// returns the next node after cur_node
point_list_node* remove_node_and_get_next(point_list_node* prev_node, point_list_node* cur_node) {
    if (prev_node == NULL) {
        point_list_node* next_node = cur_node->next;
        destroy_node_and_point(cur_node);
        return next_node;
    } else {
        prev_node->next = cur_node->next;
        destroy_node_and_point(cur_node);
        return prev_node->next;
    }
}

// returns new list
point_list_node* exclude_nodes_with_point(point_list_node* head, point p) {
    point_list_node* new_head = NULL;
    point_list_node* cur_node = head;
    while (cur_node != NULL) {
        point cur_point = *(cur_node->point);
        if (!is_equal(cur_point, p)) {
            new_head = prepend_point_to_list(new_head, create_point(cur_point));
        }
        cur_node = cur_node->next;
    }
    return new_head;
}

int size(point_list_node* list) {
    int size = 0;
    while(list != NULL) {
        size++;
        list = list->next;
    }
    return size;
}

bool is_empty(point_list_node* list) {
    return list == NULL;
}

// optimization: could maintain this list over time if we wanted rather than recalculating it
point_list_node* to_point_list(piece* pieces, int num_pieces) {
    point_list_node* point_list = NULL;
    for (int i = 0; i < num_pieces; i++) {
        piece cur_piece = *(pieces + i);
        point* cur_point = cur_piece.point;
        if (cur_point != NULL) {
            point_list = prepend_point_to_list(point_list, cur_point);
            add_payload(point_list, cur_piece.piece_id);
        }
    }
    return point_list;
}
