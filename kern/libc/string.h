
#pragma once

#include "../stdint.h"

const char* strchr(const char* str, char chr);
char* strcpy(char* dst, const char* src);
char *strncpy(char *s1, const char *s2, size_t n);  // copied from a scrapped version of PekOS
unsigned strlen(const char* str);
int strcmp(char *str1, char *str2);
