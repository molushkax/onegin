#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>
#include "onegin.h"

void quick_sort (void* data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*));
int  compare_alphabet_order (const void* value_a, const void* value_b);
int  compare_alphabet_order_reverse (const void* value_a, const void* value_b);
int  compare_from_min_to_max (const void* value_a, const void* value_b);

#endif /* SORTING_H */
