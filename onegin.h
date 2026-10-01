#ifndef ONEGIN_H
#define ONEGIN_H

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>

#define NO_COLOR "\033[0m"
#define RED      "\033[1;31m"

struct error_map {
    int         error_code;
    const char* error_message;
};

struct string_info {
    char*  start_ptr;
    size_t length;
};

struct info_for_working_with_file {
    int           file_onegin;
    char*         buffer;
    size_t        bytes_for_onegin;
    ssize_t       number_of_read_info;
    string_info*  index;
    size_t        number_of_strings;
};

void   print_custom_error (int error_code, const char* function_name, int line);
size_t work_with_buffer_for_text (info_for_working_with_file* file_info, const char* caller);
int    get_size_of_file (const char* onegin, size_t* bytes_for_onegin, const char* caller);
int    read_file (int file_onegin, char** buffer, int bytes_for_onegin, const char* caller);
int    check_of_memory_allocation (const char* buffer, const char* caller);
int    check_of_opening_file (const int file_onegin, const char* caller);
int    check_of_reading_file (const int file_onegin, const ssize_t number_of_read_info, const char* caller);
int    check_of_memory_allocation_index (string_info* index, const char* caller);
int    check_of_opening_file (FILE* onegin_out, const char* caller);
size_t strings_number (char* buffer, size_t bytes);
int    write_pointers_to_strings_in_index (info_for_working_with_file* file_info, size_t memory_for_index, const char* caller);
void   print_results_of_sorting_in_file (string_info* index, size_t lines_to_read, FILE* onegin_out, const char* type_of_sorting);
size_t put_pointers_to_strings_in_index (char* buffer, string_info* index, size_t bytes_for_onegin);
int    free_alloc_plus_ptr (string_info* index, char* buffer, size_t bytes_for_onegin, size_t number_of_strings);

#endif /* ONEGIN_H */
