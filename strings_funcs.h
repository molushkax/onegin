#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include <stddef.h>
#include "onegin.h"

char*  my_strdup (const char* string);
void*  my_memcpy (void* where, const void* from, size_t n);
size_t my_strlen (const char* string);
int    swap (void* first_element, void* second_element, size_t size_of_value);
int    my_strcmp_for_onegin (const char* first_string, const char* second_string);
int    my_strcmp_for_onegin_reverse (const string_info* first_string, const string_info* second_string);
int    skip_not_alpha (const char* string, int* i);
int    skip_not_alpha_reverse (const char* string, int* len);
bool   isalpha_rus (unsigned char alpha);
unsigned char tolower_rus (unsigned char c);

#endif /* STRING_FUNCS_H */
