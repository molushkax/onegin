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
#include <locale.h>

#include "onegin.h"

const struct error_map errors[] = {
    {ENOMEM, "Memory allocation failed"},
    {EACCES, "Permission denied"},
    {ENOENT, "File not found"},
    {EIO,    "Input/output error"}
};

void print_custom_error (int error_code, const char* function_name, int line) {

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

    fprintf (stderr, RED "ERROR %d %s in function <<%s>> in line %d\n" NO_COLOR,
             error_code, message, function_name, line);
}

size_t work_with_buffer_for_text (info_for_working_with_file* file_info, const char* caller) {

    assert(file_info != NULL);

    const char* onegin = "onegin_rus.txt";

    get_size_of_file (onegin, &(file_info->bytes_for_onegin), caller);

    file_info->file_onegin = open (onegin, O_RDONLY);
    check_of_opening_file (file_info->file_onegin, caller);

    file_info->buffer = (char*)malloc(file_info->bytes_for_onegin + 1);
    check_of_memory_allocation (file_info->buffer, caller);

    file_info->number_of_read_info = read_file (file_info->file_onegin, &(file_info->buffer), (int)(file_info->bytes_for_onegin), caller);

    size_t number_of_strings = strings_number (file_info->buffer, file_info->number_of_read_info);

    return number_of_strings;
}

int get_size_of_file (const char* onegin, size_t* bytes_for_onegin, const char* caller) {

    assert(onegin != NULL);
    assert(bytes_for_onegin != NULL);

    struct stat file_stat = {};

    if (stat (onegin, &file_stat) == 0) {
        *bytes_for_onegin = file_stat.st_size;
        printf ("\nNumber of bytes for onegin: %zu\n\n", *bytes_for_onegin);
        return 0;
    }
    else
    {
        print_custom_error (ENOENT, caller, __LINE__);
        return -1;
    }
}

int read_file (int file_onegin, char** buffer, int bytes_for_onegin, const char* caller) {

    ssize_t read_info = read (file_onegin, *buffer, (int)bytes_for_onegin);
    check_of_reading_file (file_onegin, read_info, caller);
    (*buffer)[read_info] = '\0';

    return (int)read_info;
}

int check_of_memory_allocation (const char* buffer, const char* caller) {

    if (buffer == NULL)
    {
        print_custom_error(ENOMEM, caller, __LINE__);
        return -1;
    }
    else return 0;
}

int check_of_opening_file (const int file_onegin, const char* caller) {

    if (file_onegin == -1)
    {
        print_custom_error(ENOENT, caller, __LINE__);
        return -1;
    }
    else
    {
        printf ("Descriptor of onegin: %d\n\n", file_onegin);
        return 0;
    }
}

int check_of_reading_file (const int file_onegin, const ssize_t number_of_read_info, const char* caller) {

    if (number_of_read_info == -1)
    {
        print_custom_error(EIO, caller, __LINE__);
        close (file_onegin);
        return -1;
    }
    else
    {
        printf ("Result of read function: %zd\n\n", number_of_read_info);
        return 0;
    }
}

int check_of_opening_file (FILE* onegin_out, const char* caller) {

    if (onegin_out == NULL)
    {
        print_custom_error(ENOENT, caller, __LINE__);
        return -1;
    }
    else return 0;
}

int write_pointers_to_strings_in_index (info_for_working_with_file* file_info, size_t memory_for_index, const char* caller) {

    file_info->index = (string_info*)calloc(memory_for_index + 1, sizeof(string_info));
    check_of_memory_allocation_index (file_info->index, caller);

    file_info->number_of_strings = put_pointers_to_strings_in_index (file_info->buffer, file_info->index, file_info->number_of_read_info);

    return 0;
}

int check_of_memory_allocation_index (string_info* index, const char* caller) {

    if (index == NULL)
    {
        print_custom_error(ENOMEM, caller, __LINE__);
        return -1;
    }
    else return 0;
}

void print_results_of_sorting_in_file (string_info* index, size_t lines_to_read, FILE* onegin_out, const char* type_of_sorting) {

    assert(index != NULL);
    assert(onegin_out != NULL);

    fprintf (onegin_out, "\n%s \n\n", type_of_sorting);
    for (size_t i = 0; i < lines_to_read; i++)
    {
        if (index[i].start_ptr != NULL && index[i].start_ptr[0] != '\0' &&
        !(index[i].start_ptr[6] == ' ' && index[i].start_ptr[3] == ' ' && index[i].start_ptr[8] == ' '  && index[i].start_ptr[10] == ' ')
        && index[i].length > 5)
        {
            fprintf(onegin_out, "%s\n", index[i].start_ptr);
        }
    }
}

size_t put_pointers_to_strings_in_index (char* buffer, string_info* index, size_t bytes_for_onegin) {

    assert(index  != NULL);
    assert(buffer != NULL);

    index[0].start_ptr = buffer;
    index[0].length = 0;
    size_t i = 0;
    size_t j = 1;

    for (i = 0; i < bytes_for_onegin; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';

            index[j - 1].length = (size_t)(&buffer[i] - index[j - 1].start_ptr);

            if (i + 1 < bytes_for_onegin)
            {
                index[j].start_ptr = &buffer[i] + 1;
                index[j].length = 0;
                j++;
            }
        }
    }

    if (j > 0 && index[j - 1].length == 0)
    {
        index[j - 1].length = (size_t)(&buffer[bytes_for_onegin] - index[j - 1].start_ptr);
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

int free_alloc_plus_ptr (string_info* index, char* buffer, size_t bytes_for_onegin, size_t number_of_strings) {

    memset(index,  0, (number_of_strings + 1) * sizeof(string_info));
    memset(buffer, 0, bytes_for_onegin + 1);

    free(buffer);
    free(index);

    buffer = NULL;
    index = NULL;

    return 0;
}
