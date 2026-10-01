#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>

#include "strings_funcs.h"

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
    unsigned char* from_ptr  = (unsigned char*)from;

    while (n >= sizeof(uint64_t))
    {
        *(uint64_t*)where_ptr = *(const uint64_t*)from_ptr;
        where_ptr += sizeof(uint64_t);
        from_ptr  += sizeof(uint64_t);
        n -= sizeof(uint64_t);
    }

    if (n >= sizeof(uint32_t))
    {
        *(uint32_t*)where_ptr = *(const uint32_t*)from_ptr;
        where_ptr += sizeof(uint32_t);
        from_ptr  += sizeof(uint32_t);
        n -= sizeof(uint32_t);
    }

    if (n >= sizeof(uint16_t))
    {
        *(uint16_t*)where_ptr = *(const uint16_t*)from_ptr;
        where_ptr += sizeof(uint16_t);
        from_ptr  += sizeof(uint16_t);
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

unsigned char tolower_rus (unsigned char c) {

    if (c >= 0xC0 && c <= 0xDF) return c + 0x20;
    if (c == 0xA8) return 0xB8;
    return c;
}

int my_strcmp_for_onegin (const char* first_string, const char* second_string) {

    assert(first_string  != NULL);
    assert(second_string != NULL);

    int i = 0, j = 0;
    while (first_string != NULL && second_string != NULL)
    {
        skip_not_alpha (first_string, &i);
        skip_not_alpha (second_string, &j);

        if (first_string[i]  == '\0' && second_string[j] == '\0')
            return 0;
        if (first_string[i]  == '\0')
            return -1;
        if (second_string[j] == '\0')
            return 1;

        int difference = tolower_rus((unsigned char)first_string[i]) - tolower_rus((unsigned char)second_string[j]);
        if (difference != 0)
            return difference;

        i++;
        j++;
    }
    return 0;
}

int skip_not_alpha (const char* string, int* i) {

    assert (string != NULL);
    assert (i != NULL);

    while (string[*i] != '\0' && !isalpha((unsigned char)string[*i]) && !isalpha_rus ((unsigned char)string[*i]))
    {
        (*i)++;
    }

    return 0;
}

bool isalpha_rus (unsigned char alpha) {

    if (alpha >= 192 || alpha == 168 || alpha == 184) return true;
    else return false;
}

int my_strcmp_for_onegin_reverse (const string_info* first_string, const string_info* second_string) {

    assert(first_string != NULL);
    assert(second_string != NULL);

    int first_len  = (int)first_string->length  - 1;
    int second_len = (int)second_string->length - 1;

    const char* str1 = first_string->start_ptr;
    const char* str2 = second_string->start_ptr;

    while (str1 != NULL && str2 != NULL)
    {
        skip_not_alpha_reverse (str1, &first_len);
        skip_not_alpha_reverse (str2, &second_len);

        if (first_len  < 0 && second_len < 0)
            return 0;
        if (first_len  < 0)
            return -1;
        if (second_len < 0)
            return 1;

        int difference  = tolower_rus((unsigned char)str1[first_len]) - tolower_rus((unsigned char)str2[second_len]);
        if (difference != 0)
            return difference;

        first_len--;
        second_len--;
    }
    return 0;
}

int skip_not_alpha_reverse (const char* string, int* len) {

    assert (string != NULL);
    assert (len    != NULL);

    while (*len >= 0 && !isalpha((unsigned char)string[*len]) && !isalpha_rus ((unsigned char)string[*len]))
    {
        (*len)--;
    }

    return 0;
}
