#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

void quick_sort(void* data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*));
int swap (void* first_element, void* second_element, size_t size_of_value);
int compare_from_min_to_max (const void* value_a, const void* value_b);
int my_strcmp_for_onegin (const char* string1, const char* string2);
char* string_to_word (const char* string, size_t len_of_string);
size_t my_strlen (const char* string);

int main () {
    const char* time[100] = {"Afg, jj,sk", "Tgh[jjajj]", "BSDuah ahhkj 5%hsbb", "Truwhdgawhbdbsd", "Af, gjh"};
    size_t size_of_data = 50;
    size_t size_of_data_element = sizeof(char);

    const char* s1 = "Abb, fff31;";
    const char* s2 = "Abbk4kd lsk . ..";
    int i = my_strcmp_for_onegin (s1, s2);
    printf ("strcmp result = %d", i);
}

void quick_sort(void* data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*)) {
    size_t max_arr_index = size_of_data - 1;

    if (size_of_data > 1)
    {
        char pivot = *(char*)((uintptr_t)data + (size_of_data - 1) * size_of_data_element);
        int i = 0;

        for (size_t j = 0; j < max_arr_index; j++)
        {
            if (compare_func ((void*)((uintptr_t)data + j * size_of_data_element), &pivot) > 0)
            {
                swap ((void*)((uintptr_t)data + i * size_of_data_element), (void*)((uintptr_t)data + j * size_of_data_element),
                    size_of_data_element);
                i++;
            }
        }

        swap ((void*)((uintptr_t)data + i * size_of_data_element), (void*)((uintptr_t)data + max_arr_index * size_of_data_element),
            size_of_data_element);

        void* start_of_first_massive = data;
        void* start_of_second_massive = (void*)((uintptr_t)data + (i + 1) * size_of_data_element);

        quick_sort(start_of_first_massive,                     i, size_of_data_element, compare_func);
        quick_sort(start_of_second_massive, size_of_data - i - 1, size_of_data_element, compare_func);
    }
}

int swap (void* first_element, void* second_element, size_t size_of_value) {
    if (first_element == NULL || second_element == NULL || size_of_value == 0) {
        return -1;
    }

    unsigned char* a_ptr = (unsigned char*)first_element;
    unsigned char* b_ptr = (unsigned char*)second_element;

    while (size_of_value >= sizeof(uint64_t)) {
        uint64_t temp = *(uint64_t*)a_ptr;
        *(uint64_t*)a_ptr = *(uint64_t*)b_ptr;
        *(uint64_t*)b_ptr = temp;
        a_ptr += sizeof(uint64_t);
        b_ptr += sizeof(uint64_t);
        size_of_value -= sizeof(uint64_t);
    }

    if (size_of_value >= sizeof(uint32_t)) {
        uint32_t temp = *(uint32_t*)a_ptr;
        *(uint32_t*)a_ptr = *(uint32_t*)b_ptr;
        *(uint32_t*)b_ptr = temp;
        a_ptr += sizeof(uint32_t);
        b_ptr += sizeof(uint32_t);
        size_of_value -= sizeof(uint32_t);
    }

    if (size_of_value >= sizeof(uint16_t)) {
        uint16_t temp = *(uint16_t*)a_ptr;
        *(uint16_t*)a_ptr = *(uint16_t*)b_ptr;
        *(uint16_t*)b_ptr = temp;
        a_ptr += sizeof(uint16_t);
        b_ptr += sizeof(uint16_t);
        size_of_value -= sizeof(uint16_t);
    }

    while (size_of_value > 0) {
        unsigned char temp = *a_ptr;
        *a_ptr = *b_ptr;
        *b_ptr = temp;
        a_ptr++;
        b_ptr++;
        size_of_value--;
    }

    return 0;
}

int compare_from_min_to_max (const void* value_a, const void* value_b) {
    int a = *(const int*)value_a;
    int b = *(const int*)value_b;
    return (b - a);
}

int my_strcmp_for_onegin (const char* string1, const char* string2) {
    int i = 0;
    size_t len_of_string1 = my_strlen (string1);
    size_t len_of_string2 = my_strlen (string2);

    char* glued_string1 = string_to_word (string1, len_of_string1);
    char* glued_string2 = string_to_word (string2, len_of_string2);

    printf ("\n1 string: %s\n", glued_string1);
    printf ("2 string: %s\n\n", glued_string2);

    while (glued_string1[i] == glued_string2[i])
    {
        i++;
    }

    if ((glued_string1[i] > glued_string2[i]) || (glued_string1[i] < glued_string2[i]))
    {
        return glued_string1[i] - glued_string2[i];
    }

    else
    {
        return 777;
    }
}

char* string_to_word (const char* string, size_t len_of_string) {
    char* new_string = (char*)calloc(len_of_string, string[0]);
    if (new_string == NULL)
    {
        return NULL;
    }

    int index_of_letter = 0;

    for (size_t i = 0; i < len_of_string; i++)
    {
        if (isalpha(string[i]))
        {
            new_string[index_of_letter] = string[i];
            index_of_letter++;
        }
    }

    new_string[index_of_letter] = '\0';

    return new_string;
}

size_t my_strlen (const char* string) {
    assert (string != NULL);

    size_t my_len = 0;

    while (string[my_len])
    {
        my_len++;
    }
    return my_len;
}
