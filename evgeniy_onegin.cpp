#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
//TODO struct with началом и длиной cp1251 strrev
#define NUMBER_OF_LINES 6760

//------------------------------------------------------------------------------------------------------------------------------

size_t read_text_from_file (char** index, char** index_copy);
void   quick_sort(void** data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*));
int    compare_alphabet_order (const void* value_a, const void* value_b);
int    compare_alphabet_order_reverse (const void* value_a, const void* value_b);
void printf_results_of_first_sorting_in_file (char** index, size_t lines_to_read, FILE* out);
void printf_results_of_second_sorting_in_file (char** index, size_t lines_to_read, FILE* out);

//-------- My library --------

char*  my_strdup (const char* string);
void*  my_memcpy (void* where, const void* from, size_t n);
size_t my_strlen (const char* string);
int    swap (void* first_element, void* second_element, size_t size_of_value);
int    my_strcmp_for_onegin (const char* first_string, const char* second_string);
int    my_strcmp_for_onegin_reverse (const char* first_string, const char* second_string);
void   skip_not_alpha (const char* first_string, const char* second_string, int* i, int* j);
//void   skip_not_alpha_reverse (const char* first_string, const char* second_string, size_t* first_len, size_t* second_len);

//-------------------------------------------------------------------------------------------------------------------------------

int main () {

    char** index = (char**)calloc (NUMBER_OF_LINES, sizeof(char*));
    char** index_copy = (char**)calloc (NUMBER_OF_LINES, sizeof(char*));

    size_t lines_to_read = read_text_from_file (index, index_copy);

    FILE* out = fopen ("out.txt", "w");

    if (out == NULL)
    {
        fprintf (stderr, "Problems with file");
        return -1;
    }

    qsort(index, lines_to_read, sizeof(char*),compare_alphabet_order);
    printf_results_of_first_sorting_in_file (index, lines_to_read, out);

    quick_sort((void**)index, lines_to_read, sizeof(char*),compare_alphabet_order_reverse);
    printf_results_of_second_sorting_in_file (index, lines_to_read, out);

    fprintf (out, "\nOriginal Onegin\n\n");
    for (size_t i = 0; i < lines_to_read; i++)
    {
            fprintf (out, "%s", index_copy[i]);
    }

    for (size_t i = 0; i < lines_to_read; i++)
    {
        free (index[i]);
        free (index_copy[i]);

        lines_to_read--;
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

size_t read_text_from_file (char** index, char** index_copy) {

    FILE* text = fopen ("onegin.txt", "r");

    if (text == NULL)
    {
        fprintf (stderr, "Problems with file");
        return -1;
    }

    char buffer [NUMBER_OF_LINES] = {};
    size_t number_of_lines = 0;

    while (fgets(buffer, NUMBER_OF_LINES, text) != NULL && number_of_lines < 6780)
    {
        size_t buffer_len = my_strlen (buffer);
        if (buffer_len > 0 && buffer[buffer_len - 1] != '\n')
        {
            buffer[buffer_len] = '\n';
            buffer[buffer_len + 1] = '\0';
        }

        index[number_of_lines] = my_strdup (buffer);
        index_copy[number_of_lines] = my_strdup (buffer);

        if (index[number_of_lines] == NULL || index_copy[number_of_lines] == NULL)
        {
            fprintf (stderr, "Memory allocation failed\n");
            fclose (text);
            return -1;
        }

        number_of_lines++;
    }

    fclose (text);
    return number_of_lines;
}

void quick_sort(void** data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*)) {

    size_t max_arr_index = size_of_data - 1;

    if (size_of_data > 1)
    {
        char* pivot = *(char**)((uintptr_t)data + (size_of_data - 1) * size_of_data_element);
        int i = 0;

        for (size_t j = 0; j < max_arr_index; j++)
        {
            if (compare_func ((void*)((uintptr_t)data + j * size_of_data_element), &pivot) < 0)
            {
                swap ((void*)((uintptr_t)data + i * size_of_data_element), (void*)((uintptr_t)data + j * size_of_data_element),
                    size_of_data_element);
                i++;
            }
        }

        swap ((void*)((uintptr_t)data + i * size_of_data_element), (void*)((uintptr_t)data + max_arr_index * size_of_data_element),
            size_of_data_element);

        quick_sort(data,                            i, size_of_data_element, compare_func);
        quick_sort(data + i + 1, size_of_data - i - 1, size_of_data_element, compare_func);
    }
}

int compare_alphabet_order(const void* first_string, const void* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    const char* str1 = *(const char**)first_string;
    const char* str2 = *(const char**)second_string;
    return my_strcmp_for_onegin(str1, str2);
}

int compare_alphabet_order_reverse (const void* first_string, const void* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    return my_strcmp_for_onegin_reverse(*(const char**)first_string, *(const char**)second_string);
}

void printf_results_of_first_sorting_in_file (char** index, size_t lines_to_read, FILE* out) {

    fprintf (out, "\nSorted: \n\n");

    for (size_t i = 0; i < lines_to_read; i++)
    {
        if (index[i][0] != '\0' && index[i][0] != '\n')
            fprintf (out, "%s", index[i]);
    }
}

void printf_results_of_second_sorting_in_file (char** index, size_t lines_to_read, FILE* out) {

    fprintf (out, "\nSorted reverse: \n\n");

    for (size_t i = 0; i < lines_to_read; i++)
    {
        if (index[i][0] != '\0' && index[i][0] != '\n')
            fprintf (out, "%s", index[i]);
    }
}

//-------- My library --------

char* my_strdup (const char* string) {

    assert (string != NULL);

    size_t len_local = my_strlen (string) + 1;
    char* str = (char* )malloc(len_local * sizeof (char));

    if (str != NULL)
    {
        return ((char*)my_memcpy ((void*)str, (void*)string, len_local));
    }

    else
    {
        return NULL;
    }
}

void* my_memcpy (void* where, const void* from, size_t n) {

    if (where == NULL || from ==  NULL)
    {
        return NULL;
    }
    unsigned char* where_ptr = (unsigned char*)where;
    unsigned char* from_ptr = (unsigned char*)from;

    while (n >= sizeof(uint64_t))
    {
        *(uint64_t*)where_ptr = *(const uint64_t*)from_ptr;
        where_ptr += sizeof (uint64_t);
        from_ptr += sizeof (uint64_t);
        n -= sizeof (uint64_t);
    }

    if (n >= sizeof(uint32_t))
    {
        *(uint32_t*)where_ptr = *(const uint32_t*)from_ptr;
        where_ptr += sizeof (uint32_t);
        from_ptr += sizeof (uint32_t);
        n -= sizeof (uint32_t);
    }

    if (n >= sizeof(uint16_t))
    {
        *(uint16_t*)where_ptr = *(const uint16_t*)from_ptr;
        where_ptr += sizeof (uint16_t);
        from_ptr += sizeof (uint16_t);
        n -= sizeof (uint16_t);
    }

    while (n > 0)
    {
        *where_ptr = *from_ptr;
        where_ptr++;
        from_ptr++;
        n--;
    }

    return where;
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

int my_strcmp_for_onegin (const char* first_string, const char* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    int i = 0;
    int j = 0;

    skip_not_alpha (first_string, second_string, &i, &j);

    while (tolower(first_string[i]) == tolower(second_string[j]) &&
        tolower(first_string[i]) != '\0' && tolower(second_string[j]) != '\0')
    {
        skip_not_alpha (first_string, second_string, &i, &j);

        i++;
        j++;
    }

    skip_not_alpha (first_string, second_string, &i, &j);

    if ((tolower(first_string[i]) != tolower(second_string[j])))
    {
        return tolower(first_string[i]) - tolower(second_string[j]);
    }

    else
    {
        return 0;
    }
}

int my_strcmp_for_onegin_reverse (const char* first_string, const char* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    size_t first_len = my_strlen (first_string) - 1;
    size_t second_len = my_strlen (second_string) - 1;

    skip_not_alpha_reverse (first_string, second_string, &first_len, &second_len);

    while (tolower(first_string[first_len]) == tolower(second_string[second_len]) &&
        first_len > 0 && second_len > 0)
    {
        skip_not_alpha_reverse (first_string, second_string, &first_len, &second_len);

        first_len--;
        second_len--;
    }

    skip_not_alpha_reverse (first_string, second_string, &first_len, &second_len);

    if ((tolower(first_string[first_len]) != tolower(second_string[second_len])))
    {
        return tolower(first_string[first_len]) - tolower(second_string[second_len]);
    }

    else return 0;
}

void skip_not_alpha (const char* first_string, const char* second_string, int* i, int* j) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    while (!isalpha (first_string[*i])) (*i)++;
    while (!isalpha (second_string[*j])) (*j)++;
}

void skip_not_alpha_reverse (const char* first_string, const char* second_string, size_t* first_len, size_t* second_len) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    while (!isalpha (first_string[*first_len])) (*first_len)--;
    while (!isalpha (second_string[*second_len])) (*second_len)--;
}

//--------------------------------------------------------------------------------------------------------------------------------

