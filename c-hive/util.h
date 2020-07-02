//
//  util.h
//  c-hive
//
//  Created by Bryan Williams on 7/1/20.
//  Copyright © 2020 Bryan Williams. All rights reserved.
//

#ifndef util_h
#define util_h

#include <stdio.h>

int sum(int array[], int size);

int max(void* array, int num, int size, int (*to_int)(void*));

int min(void* array, int num, int size, int (*to_int)(void*));

//int max_reduce(int agg_value, void* cur_element, int (*to_int)(void*));

//int min_reduce(int agg_value, void* cur_element, int (*to_int)(void*));

//int reduce(void*array, int num, int size, int (*reduce)(int, void*), int initial_value);

#endif /* util_h */
