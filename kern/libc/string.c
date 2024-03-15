
#include "string.h"

const char* strchr(const char* str, char chr)
{
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}

char* strcpy(char* dst, const char* src)
{
    char* origDst = dst;

    if (dst == NULL)
        return NULL;

    if (src == NULL)
    {
        *dst = '\0';
        return dst;
    }

    while (*src)
    {
        *dst = *src;
        ++src;
        ++dst;
    }
    
    *dst = '\0';
    return origDst;
}

char *strncpy(char *s1, const char *s2, size_t n)
{
	unsigned int extern_iter = 0;

	unsigned int iterator = 0;
	for (iterator = 0; iterator < n; iterator++)
	{
		if (s2[iterator] != '\0')
			s1[iterator] = s2[iterator];
		else
		{
			s1[iterator] = s2[iterator];
			extern_iter = iterator + 1;
			break;
		}
	}

	while (extern_iter < n)
	{
		s1[extern_iter] = '\0';
		extern_iter++;
	}

	return s1;
}

unsigned strlen(const char* str)
{
    unsigned len = 0;
    while (*str)
    {
        ++len;
        ++str;
    }

    return len;
}

int strcmp(char *str1, char *str2)
{
    while (*str1 && *str2 && (*str1++ = *str2++));

    if (*str1 == '\0' && *str2 == '\0')
        return 0;

    if (*str1 == '\0')
        return -1;

    else return 1;
}
