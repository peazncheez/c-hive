//
//  util.c
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#include "util.h"
#include "limits.h"

int sum(int array[], int size) {
    int total = 0, loop = 0;
    for (; loop < size; loop++) {
        total += array[loop];
    }
    return total;
}

int max(void* array, int num, int size, int (*to_int)(void*)) {
    int loop;
    int cur_max = INT_MIN;
    for (loop = 0; loop < size; loop++) {
        int cur_int = to_int((array + size*loop));
        cur_max = cur_int > cur_max ? cur_int : cur_max;
    }
    return cur_max;
}

int min(void* array, int num, int size, int (*to_int)(void*)) {
    int loop;
    int cur_min = INT_MAX;
    for (loop = 0; loop < size; loop++) {
        int cur_int = to_int((array + size*loop));
        cur_min = cur_int < cur_min ? cur_int : cur_min;
    }
    return cur_min;
}

//int max_reduce(int agg_value, void* cur_element, int (*to_int)(void*)) {
//    int cur_int = to_int(cur_element);
//    return agg_value > cur_int ? agg_value : cur_int;
//}
//
//int min_reduce(int agg_value, void* cur_element, int (*to_int)(void*)) {
//    int cur_int = to_int(cur_element);
//    return agg_value < cur_int ? agg_value : cur_int;
//}

//int reduce(void*array, int num, int size, int (*reduce)(int, void*), int initial_value) {
//    int loop;
//    int agg_value = initial_value;
//    for (loop = 0; loop < num; loop++) {
//        agg_value = reduce(agg_value, (array + size*loop));
//    }
//    return agg_value;
//}
