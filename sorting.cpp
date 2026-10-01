#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "sorting.h"
#include "strings_funcs.h"

void quick_sort (void* data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*)) {

    assert(data != NULL);

    if (size_of_data <= 1) return;

    size_t max_arr_index = size_of_data - 1;
    size_t i = 0;

    for (size_t j = 0; j < max_arr_index; j++)
    {
        if (compare_func ((void*)((uintptr_t)data +             j * size_of_data_element),
                          (void*)((uintptr_t)data + max_arr_index * size_of_data_element)) < 0)
        {
            swap ((void*)((uintptr_t)data + j * size_of_data_element),
                  (void*)((uintptr_t)data + i * size_of_data_element),
                 size_of_data_element);
            i++;
        }
    }

    swap ((void*)((uintptr_t)data +             i * size_of_data_element),
          (void*)((uintptr_t)data + max_arr_index * size_of_data_element),
         size_of_data_element);

    quick_sort (data,                                                                         i, size_of_data_element, compare_func);
    quick_sort ((void*)((uintptr_t)data + (i + 1) * size_of_data_element), size_of_data - i - 1, size_of_data_element, compare_func);
}

int compare_alphabet_order (const void* first_string, const void* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    const char* str1 = ((const string_info*)first_string)->start_ptr;
    const char* str2 = ((const string_info*)second_string)->start_ptr;
    return my_strcmp_for_onegin(str1, str2);
}

int compare_alphabet_order_reverse (const void* first_string, const void* second_string) {

    assert(first_string  != NULL);
    assert(second_string != NULL);

    return my_strcmp_for_onegin_reverse ((const string_info*)first_string, (const string_info*)second_string);
}

int compare_from_min_to_max (const void* value_a, const void* value_b) {

    assert(value_a != NULL);
    assert(value_b != NULL);

    const char* a = ((const string_info*)value_a)->start_ptr;
    const char* b = ((const string_info*)value_b)->start_ptr;

    return int(a - b);
}
