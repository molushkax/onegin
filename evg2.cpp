#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>   // Для read, close
#include <sys/stat.h>
#include <math.h>
#include <assert.h>

//------------------------------------------------------------------------------------------------------------------------------

size_t read_text_from_file (const char* filename, char** buffer);
void   quick_sort(void** data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*));
int    compare_alphabet_order (const void* value_a, const void* value_b);
int    compare_alphabet_order_reverse (const void* value_a, const void* value_b);
void   printf_results_of_first_sorting_in_file (char** index, size_t lines_to_read, FILE* out);
void   printf_results_of_second_sorting_in_file (char** index, size_t lines_to_read, FILE* out);
void   put_pointers_to_strings_in_index (char* buffer, char** index, size_t bytes_for_onegin);
size_t strings_number (char* buffer, size_t bytes);

//-------- My library --------

char*  my_strdup (const char* string);
void*  my_memcpy (void* where, const void* from, size_t n);
size_t my_strlen (const char* string);
int    swap (void* first_element, void* second_element, size_t size_of_value);
int    my_strcmp_for_onegin (const char* first_string, const char* second_string);
int    my_strcmp_for_onegin_reverse (const char* first_string, const char* second_string);
void   skip_not_alpha (const char* string, int* i);
void   skip_not_alpha_reverse (const char* string, int* len);

//-------------------------------------------------------------------------------------------------------------------------------

int main () {

    const char* onegin = "onegin.txt";
    struct stat file_stat;
    size_t bytes_for_onegin = 0;

    // 1. Получаем размер файла
    if (stat (onegin, &file_stat) == 0)
    {
        bytes_for_onegin = file_stat.st_size;
    }
    else
    {
        fprintf(stderr, "Cannot open file %s\n", onegin);
        return -1;
    }
    printf ("\nNumber of bytes for onegin: %zu\n\n", bytes_for_onegin);

    // 2. Выделяем память под весь файл (плюс 1 байт для '\0')
    char* buffer = (char*)malloc (bytes_for_onegin + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    // 3. Читаем файл
    int file_onegin = open (onegin, O_RDONLY);
    if (file_onegin == -1) {
        fprintf(stderr, "Cannot open file descriptor\n");
        free(buffer);
        return -1;
    }
    printf ("Descriptor of onegin: %d\n\n", file_onegin);

    ssize_t number_of_read_info = read (file_onegin, buffer, (int)bytes_for_onegin);
    if (number_of_read_info == -1) {
        fprintf(stderr, "Read error\n");
        close(file_onegin);
        free(buffer);
        return -1;
    }
    buffer[number_of_read_info] = '\0';
    printf ("Result of read function: %zd\n\n", number_of_read_info);

    close(file_onegin); // Закрываем дескриптор сразу после чтения

    // 4. Считаем количество строк (непустых)
    size_t number_of_strings = strings_number (buffer, number_of_read_info);
    printf ("Number of lines in Onegin: %zu\n\n", number_of_strings);

    // 5. Задаем память массив и заполняем его указателями
    char** index = (char**)calloc (number_of_strings + 1, sizeof(char*));
    if (index == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(buffer);
        return -1;
    }

    put_pointers_to_strings_in_index (buffer, index, number_of_read_info);

    FILE* out = fopen ("out.txt", "w");
    if (out == NULL)
    {
        fprintf (stderr, "Problems with file\n");
        free(buffer);
        free(index);
        return -1;
    }

    // 6. Сортировка по алфавиту
    qsort(index, number_of_strings, sizeof(char*), compare_alphabet_order);
    printf_results_of_first_sorting_in_file (index, number_of_strings, out);

    // 7. Сортировка с конца
    quick_sort((void**)index, number_of_strings, sizeof(char*), compare_alphabet_order_reverse);
    printf_results_of_second_sorting_in_file (index, number_of_strings, out);

    // 8. Освобождение памяти
    free (buffer);
    free (index);
    fclose (out);

    printf("Done! Check file out.txt\n\n");
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------

// Считает количество непустых строк в буфере
size_t strings_number (char* buffer, size_t bytes) {

    assert(buffer != NULL);

    int num = 1;

    while (bytes > 0)
    {
        if (buffer[bytes-1] == '\n')
            num++;
        if (buffer[bytes-1] == '\r')
            num--;
        bytes--;
    }

    return (num - 1) / 2;
}

// Заменяет \n на \0 и заполняет index указателями на начало строк
/*size_t put_pointers_to_strings_in_index (char* buffer, char** index, size_t bytes_for_onegin) {

    assert(index != NULL);
    assert(buffer != NULL);

    if (bytes_for_onegin == 0) return 0;

    size_t lines_count = 0;
    int has_chars = 0;
    size_t start_of_line = 0;

    for (size_t i = 0; i < bytes_for_onegin; i++) {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            if (i > 0 && buffer[i-1] == '\r')
            {
                buffer[i-1] = '\0';
            }

            // Если в строке были символы, добавляем её в индекс
            if (has_chars)
            {
                index[lines_count] = &buffer[start_of_line];
                lines_count++;
            }
            has_chars = 0;
            start_of_line = i + 1;
        } else if (buffer[i] != '\r' && buffer[i] != ' ' && buffer[i] != '\t')
        {
            if (!has_chars)
            {
                // Начало новой непустой строки
                has_chars = 1;
                start_of_line = i;
            }
        }
    }

    if (has_chars)
    {
        index[lines_count] = &buffer[start_of_line];
        lines_count++;
    }
    return lines_count;
}*/

void put_pointers_to_strings_in_index (char* buffer, char** index, size_t bytes_for_onegin) {

    assert(index != NULL);

    index[0] = buffer;
    size_t i = 0;
    int j = 1;

    while (i < bytes_for_onegin - 1)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';

            if (i > 0 && buffer[i - 1] == 'r')
            {
                buffer[i - 1] = '\0';
            }

            if (i + 1 < bytes_for_onegin)
            {
                index[j] = &buffer[i] + 1;
                j++;
            }
        }

        i++;
    }

    buffer[bytes_for_onegin - 1] = '\0';
}

//--------------------------------------------------------------------------------------------------------------------------------

// Быстрая сортировка с исправленной логикой
void quick_sort(void** data, size_t size_of_data, size_t size_of_data_element, int (*compare_func)(const void*, const void*)) {
    if (size_of_data <= 1) return;

    size_t max_arr_index = size_of_data - 1;
    size_t i = 0;

    for (size_t j = 0; j < max_arr_index; j++)
    {
        if (compare_func((void*)((uintptr_t)data + j * size_of_data_element),
           (void*)((uintptr_t)data + max_arr_index * size_of_data_element)) < 0)
        {
            swap((void*)((uintptr_t)data + i * size_of_data_element),
                 (void*)((uintptr_t)data + j * size_of_data_element),
                 size_of_data_element);
            i++;
        }
    }

    swap((void*)((uintptr_t)data + i * size_of_data_element),
         (void*)((uintptr_t)data + max_arr_index * size_of_data_element),
         size_of_data_element);

    quick_sort(data, i, size_of_data_element, compare_func);
    quick_sort((void**)((uintptr_t)data + (i + 1) * size_of_data_element),
               size_of_data - i - 1, size_of_data_element, compare_func);
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
    for (size_t i = 0; i < lines_to_read; i++) {
        if (index[i] != NULL) {
            fprintf(out, "%s\n", index[i]);
        }
    }
}

void printf_results_of_second_sorting_in_file (char** index, size_t lines_to_read, FILE* out) {

    fprintf (out, "\nSorted reverse: \n\n");
    for (size_t i = 0; i < lines_to_read; i++)
    {
        if (index[i] != NULL)
        {
            fprintf(out, "%s\n", index[i]);
        }
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

// Пропускает не буквы
void skip_not_alpha (const char* string, int* i) {

    assert (string != NULL);
    assert (i != NULL);

    while (string[*i] != '\0' && !isalpha((unsigned char)string[*i]))
    {
        (*i)++;
    }
}

// Основная функция сравнения (игнорирует все кроме букв)
int my_strcmp_for_onegin (const char* first_string, const char* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    int i = 0, j = 0;

    while (1)
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
}

// Пропускает не буквы с конца
void skip_not_alpha_reverse (const char* string, int* len) {

    assert (string != NULL);
    assert (len != NULL);

    while (*len >= 0 && !isalpha((unsigned char)string[*len]))
    {
        (*len)--;
    }
}

// Функция сравнения с конца
int my_strcmp_for_onegin_reverse (const char* first_string, const char* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    int first_len = (int)my_strlen(first_string) - 1;
    int second_len = (int)my_strlen(second_string) - 1;

    while (1)
    {
        skip_not_alpha_reverse(first_string, &first_len);
        skip_not_alpha_reverse(second_string, &second_len);

        if (first_len < 0 && second_len < 0) return 0;
        if (first_len < 0) return -1;
        if (second_len < 0) return 1;

        int difference = tolower((unsigned char)first_string[first_len]) -
                   tolower((unsigned char)second_string[second_len]);
        if (difference != 0) return difference;

        first_len--;
        second_len--;
    }
}
