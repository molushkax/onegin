#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

struct error_map {

    int error_code;
    const char* error_message;
};

struct info_for_working_with_file {

    int     file_onegin;
    char*   buffer;
    size_t  bytes_for_onegin;
    ssize_t number_of_read_info;
    char**  index;
    size_t  number_of_strings;
};

//--------------------------------------------------------------------------------------------------------------------------------------

size_t work_with_buffer_for_text (info_for_working_with_file* file_info);
int    get_size_of_file (const char* onegin, size_t* bytes_for_onegin);
int    read_file (int file_onegin, char** buffer, int bytes_for_onegin);
int    check_of_memory_allocation (const char* buffer);
int    check_of_opening_file (const int file_onegin);
int    check_of_reading_file (const int file_onegin, const ssize_t number_of_read_info);
int    check_of_memory_allocation_index (char** index);
int    check_of_opening_file (FILE* onegin_out);
size_t strings_number (char* buffer, size_t bytes);
int    write_pointers_to_strings_in_index (info_for_working_with_file* file_info, size_t memory_for_index);
void   quick_sort (void* data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*));
int    compare_alphabet_order (const void* value_a, const void* value_b);
void   print_results_of_sorting_in_file (char** index, size_t lines_to_read, FILE* onegin_out, const char* type_of_sorting);
int    compare_alphabet_order_reverse (const void* value_a, const void* value_b);
size_t put_pointers_to_strings_in_index (char* buffer, char** index, size_t bytes_for_onegin);
int    compare_from_min_to_max (const void* value_a, const void* value_b);
void   print_custom_error (const char* function_name, int error_code);

//------------ My library -------------
char*  my_strdup (const char* string);
void*  my_memcpy (void* where, const void* from, size_t n);
size_t my_strlen (const char* string);
int    swap (void* first_element, void* second_element, size_t size_of_value);
int    my_strcmp_for_onegin (const char* first_string, const char* second_string);
int    my_strcmp_for_onegin_reverse (const char* first_string, const char* second_string);
int    skip_not_alpha (const char* string, int* i);
int    skip_not_alpha_reverse (const char* string, int* len);
int    free_alloc_plus_ptr (char** index, char* buffer, size_t bytes_for_onegin, size_t number_of_strings);

//--------------------------------------------------------------------------------------------------------------------------------------

int main () {

    struct info_for_working_with_file file_info = {};

    size_t memory_for_index = work_with_buffer_for_text (&file_info);
    write_pointers_to_strings_in_index (&file_info, memory_for_index);

    FILE* onegin_out = fopen("onegin_out.txt", "w");
    check_of_opening_file (onegin_out);

    qsort (file_info.index, file_info.number_of_strings, sizeof(char*), compare_alphabet_order);
    print_results_of_sorting_in_file(file_info.index, file_info.number_of_strings, onegin_out, "Alphabet sorting:");

    quick_sort (file_info.index, file_info.number_of_strings, sizeof(char*), compare_alphabet_order_reverse);
    print_results_of_sorting_in_file(file_info.index, file_info.number_of_strings, onegin_out, "Alphabet reverse sorting:");

    qsort (file_info.index, file_info.number_of_strings, sizeof(char*), compare_from_min_to_max);
    print_results_of_sorting_in_file(file_info.index, file_info.number_of_strings, onegin_out, "Original Onegin:");

    free_alloc_plus_ptr (file_info.index, file_info.buffer, file_info.bytes_for_onegin, file_info.number_of_strings);
    close(file_info.file_onegin);
    fclose(onegin_out);

    printf("Check file onegin_out.txt\n\n");
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------

size_t work_with_buffer_for_text (info_for_working_with_file* file_info) {

    assert(file_info != NULL);

    const char* onegin = "onegin.txt";

    get_size_of_file (onegin, &(file_info->bytes_for_onegin));

    file_info->file_onegin = open(onegin, O_RDONLY);
    check_of_opening_file (file_info->file_onegin);

    file_info->buffer = (char*)malloc(file_info->bytes_for_onegin + 1);
    check_of_memory_allocation (file_info->buffer);

    file_info->number_of_read_info = read_file (file_info->file_onegin, &(file_info->buffer), (int)(file_info->bytes_for_onegin));

    size_t number_of_strings = strings_number(file_info->buffer, file_info->number_of_read_info);

    return number_of_strings;
}

int get_size_of_file (const char* onegin, size_t* bytes_for_onegin) {

    assert(onegin != NULL);
    assert(bytes_for_onegin != NULL);

    struct stat file_stat = {};

    if (stat(onegin, &file_stat) == 0) {
        *bytes_for_onegin = file_stat.st_size;
        printf("\nNumber of bytes for onegin: %zu\n\n", *bytes_for_onegin);
        return 0;
    }

    else
    {
        print_custom_error("get_size_of_file", ENOENT);
        return -1;
    }
}

int read_file (int file_onegin, char** buffer, int bytes_for_onegin) {

    ssize_t read_info = read(file_onegin, *buffer, (int)bytes_for_onegin);
    check_of_reading_file (file_onegin, read_info);
    (*buffer)[read_info] = '\0';
    return (int)read_info;
}

int check_of_memory_allocation (const char* buffer) {

    if (buffer == NULL)
    {
        print_custom_error("check_of_memory_allocation", ENOMEM);
        return -1;
    }

    else return 0;
}

int check_of_opening_file (const int file_onegin) {

    if (file_onegin == -1)
    {
        print_custom_error("check_of_opening_file (fd)", ENOENT);
        return -1;
    }

    else
    {
        printf("Descriptor of onegin: %d\n\n", file_onegin);
        return 0;
    }
}

int check_of_reading_file (const int file_onegin, const ssize_t number_of_read_info) {

    if (number_of_read_info == -1)
    {
        print_custom_error("check_of_reading_file", EIO);
        close(file_onegin);
        return -1;
    }

    else
    {
        printf ("Result of read function: %zd\n\n", number_of_read_info);
        return 0;
    }
}

int check_of_memory_allocation_index (char** index) {

    if (index == NULL)
    {
        print_custom_error("check_of_memory_allocation_index", ENOMEM);
        return -1;
    }

    else return 0;
}

int check_of_opening_file (FILE* onegin_out) {

    if (onegin_out == NULL)
    {
        print_custom_error("check_of_opening_file (FILE*)", ENOENT);
        return -1;
    }

    else return 0;
}

int write_pointers_to_strings_in_index (info_for_working_with_file* file_info, size_t memory_for_index) {

    file_info->index = (char**)calloc(memory_for_index + 1, sizeof(char*));
    check_of_memory_allocation_index (file_info->index);

    file_info->number_of_strings = put_pointers_to_strings_in_index(file_info->buffer, file_info->index, file_info->number_of_read_info);

    return 0;
}

void quick_sort (void* data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*)) {

    assert(data != NULL);

    if (size_of_data <= 1) return;

    size_t max_arr_index = size_of_data - 1;
    size_t i = 0;

    for (size_t j = 0; j < max_arr_index; j++)
    {
        if (compare_func((void*)((uintptr_t)data + j * size_of_data_element),
                         (void*)((uintptr_t)data + max_arr_index * size_of_data_element)) < 0)
        {
            swap((void*)((uintptr_t)data + j * size_of_data_element),
                 (void*)((uintptr_t)data + i * size_of_data_element),
                 size_of_data_element);
            i++;
        }
    }

    swap((void*)((uintptr_t)data + i * size_of_data_element),
         (void*)((uintptr_t)data + max_arr_index * size_of_data_element),
         size_of_data_element);

    quick_sort(data, i, size_of_data_element, compare_func);
    quick_sort((void*)((uintptr_t)data + (i + 1) * size_of_data_element), size_of_data - i - 1, size_of_data_element, compare_func);
}

int compare_alphabet_order (const void* first_string, const void* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    const char* str1 = *(const char**)first_string;
    const char* str2 = *(const char**)second_string;
    return my_strcmp_for_onegin(str1, str2);
}

void print_results_of_sorting_in_file (char** index, size_t lines_to_read, FILE* onegin_out, const char* type_of_sorting) {

    assert(index != NULL);
    assert(onegin_out != NULL);

    fprintf (onegin_out, "\n%s \n\n", type_of_sorting);
    for (size_t i = 0; i < lines_to_read; i++)
    {
        if (index[i] != NULL and index[i][0] != '\0' && index[i][0] != '\n')
        {
            fprintf(onegin_out, "%s\n", index[i]);
        }
    }
}

int compare_alphabet_order_reverse (const void* first_string, const void* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    return my_strcmp_for_onegin_reverse(*(const char**)first_string, *(const char**)second_string);
}

size_t put_pointers_to_strings_in_index (char* buffer, char** index, size_t bytes_for_onegin) {

    assert(index != NULL);
    assert(buffer != NULL);

    index[0] = buffer;
    size_t i = 0;
    size_t j = 1;

    for (i = 0; i < bytes_for_onegin; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';

            if (i + 1 < bytes_for_onegin)
            {
                index[j] = &buffer[i] + 1;
                j++;
            }
        }
    }

    return j;
}

size_t strings_number (char* buffer, size_t bytes) {

    assert(buffer != NULL);

    size_t num = 1;
    while (bytes > 0)
    {
        if (buffer[bytes - 1] == '\n') num++;
        bytes--;
    }

    return num;
}

int compare_from_min_to_max (const void* value_a, const void* value_b) {

    assert(value_a != NULL);
    assert(value_b != NULL);

    int a = *(const int*)value_a;
    int b = *(const int*)value_b;
    return (a - b);
}

void print_custom_error (const char* function_name, int error_code) {

    const struct error_map errors[] =   {
                                            {ENOMEM, "Memory allocation failed"},
                                            {EACCES, "Permission denied"},
                                            {ENOENT, "File not found"},
                                            {EIO,    "Input/output error"}
                                        };

    const char* message = "Error";
    size_t number_of_errors = sizeof(errors) / sizeof(errors[0]);

    for (size_t i = 0; i < number_of_errors; i++)
    {
        if (errors[i].error_code == error_code)
        {
            message = errors[i].error_message;
            break;
        }
    }

    fprintf(stderr, "In function <<%s>> error %d: %s\n", function_name, error_code, message);
}

//------- My library -------

char* my_strdup (const char* string) {

    assert (string != NULL);

    size_t len_local = my_strlen (string) + 1;

    char* str = (char*)malloc(len_local * sizeof (char));

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

    if (where == NULL || from == NULL)
    {
        return NULL;
    }

    unsigned char* where_ptr = (unsigned char*)where;
    unsigned char* from_ptr = (unsigned char*)from;

    while (n >= sizeof(uint64_t))
    {
        *(uint64_t*)where_ptr = *(const uint64_t*)from_ptr;
        where_ptr += sizeof(uint64_t);
        from_ptr += sizeof(uint64_t);
        n -= sizeof(uint64_t);
    }

    if (n >= sizeof(uint32_t))
    {
        *(uint32_t*)where_ptr = *(const uint32_t*)from_ptr;
        where_ptr += sizeof(uint32_t);
        from_ptr += sizeof(uint32_t);
        n -= sizeof(uint32_t);
    }

    if (n >= sizeof(uint16_t))
    {
        *(uint16_t*)where_ptr = *(const uint16_t*)from_ptr;
        where_ptr += sizeof(uint16_t);
        from_ptr += sizeof(uint16_t);
        n -= sizeof(uint16_t);
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

    if (first_element == NULL || second_element == NULL || size_of_value == 0)
    {
        return -1;
    }

    unsigned char* a_ptr = (unsigned char*)first_element;
    unsigned char* b_ptr = (unsigned char*)second_element;

    while (size_of_value >= sizeof(uint64_t))
    {
        uint64_t temp = *(uint64_t*)a_ptr;
        *(uint64_t*)a_ptr = *(uint64_t*)b_ptr;
        *(uint64_t*)b_ptr = temp;
        a_ptr += sizeof(uint64_t);
        b_ptr += sizeof(uint64_t);
        size_of_value -= sizeof(uint64_t);
    }

    if (size_of_value >= sizeof(uint32_t))
    {
        uint32_t temp = *(uint32_t*)a_ptr;
        *(uint32_t*)a_ptr = *(uint32_t*)b_ptr;
        *(uint32_t*)b_ptr = temp;
        a_ptr += sizeof(uint32_t);
        b_ptr += sizeof(uint32_t);
        size_of_value -= sizeof(uint32_t);
    }

    if (size_of_value >= sizeof(uint16_t))
    {
        uint16_t temp = *(uint16_t*)a_ptr;
        *(uint16_t*)a_ptr = *(uint16_t*)b_ptr;
        *(uint16_t*)b_ptr = temp;
        a_ptr += sizeof(uint16_t);
        b_ptr += sizeof(uint16_t);
        size_of_value -= sizeof(uint16_t);
    }

    while (size_of_value > 0)
    {
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

    int i = 0, j = 0;
    while (first_string != NULL && second_string != NULL)
    {
        skip_not_alpha(first_string, &i);
        skip_not_alpha(second_string, &j);

        if (first_string[i] == '\0' && second_string[j] == '\0') return 0;
        if (first_string[i] == '\0') return -1;
        if (second_string[j] == '\0') return 1;

        int difference = tolower((unsigned char)first_string[i]) - tolower((unsigned char)second_string[j]);
        if (difference != 0) return difference;
        i++;
        j++;
    }
    return 0;
}

int skip_not_alpha (const char* string, int* i) {

    assert (string != NULL);
    assert (i != NULL);

    while (string[*i] != '\0' && !isalpha((unsigned char)string[*i]))
    {
        (*i)++;
    }

    return 0;
}

int my_strcmp_for_onegin_reverse (const char* first_string, const char* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    int first_len = (int)my_strlen(first_string) - 1;
    int second_len = (int)my_strlen(second_string) - 1;

    while (first_string != NULL && second_string != NULL)
    {
        skip_not_alpha_reverse(first_string, &first_len);
        skip_not_alpha_reverse(second_string, &second_len);

        if (first_len < 0 && second_len < 0) return 0;
        if (first_len < 0) return -1;
        if (second_len < 0) return 1;

        int difference = tolower((unsigned char)first_string[first_len]) - tolower((unsigned char)second_string[second_len]);
        if (difference != 0) return difference;
        first_len--;
        second_len--;
    }
    return 0;
}

int skip_not_alpha_reverse (const char* string, int* len) {

    assert (string != NULL);
    assert (len != NULL);

    while (*len >= 0 && !isalpha((unsigned char)string[*len]))
    {
        (*len)--;
    }

    return 0;
}

int free_alloc_plus_ptr (char** index, char* buffer, size_t bytes_for_onegin, size_t number_of_strings) {

    memset(index, 0, (number_of_strings + 1) * sizeof(char*));
    memset(buffer, 0, bytes_for_onegin + 1);

    free(buffer);
    free(index);

    buffer = NULL;
    index = NULL;

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------
