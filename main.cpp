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
#include "sorting.h"
#include "strings_funcs.h"

int main () {

    struct info_for_working_with_file file_info = {};

    size_t memory_for_index = work_with_buffer_for_text (&file_info, __FUNCTION__);
    write_pointers_to_strings_in_index (&file_info, memory_for_index, __FUNCTION__);

    FILE* onegin_out = fopen("onegin_out.txt", "w");
    check_of_opening_file (onegin_out, __FUNCTION__);

    qsort (file_info.index, file_info.number_of_strings, sizeof(string_info), compare_alphabet_order);
    print_results_of_sorting_in_file(file_info.index, file_info.number_of_strings, onegin_out, "Alphabet sorting:");

    quick_sort (file_info.index, file_info.number_of_strings, sizeof(string_info), compare_alphabet_order_reverse);
    print_results_of_sorting_in_file(file_info.index, file_info.number_of_strings, onegin_out, "Alphabet reverse sorting:");

    qsort (file_info.index, file_info.number_of_strings, sizeof(string_info), compare_from_min_to_max);
    print_results_of_sorting_in_file(file_info.index, file_info.number_of_strings, onegin_out, "Original Onegin:");

    free_alloc_plus_ptr (file_info.index, file_info.buffer, file_info.bytes_for_onegin, file_info.number_of_strings);
    close (file_info.file_onegin);
    fclose (onegin_out);

    printf("Check file onegin_out.txt\n\n");
    return 0;
}
