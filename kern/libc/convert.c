#include "convert.h"

void num_to_string(int num, char* str) 
{
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0) 
    {
        isNegative = 1;
        num = -num;
    }

    // Convert each digit to a character
    do 
    {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add the negative sign if necessary
    if (isNegative) 
    {
        str[i++] = '-';
    }

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    int j = 0;
    int k = i - 1;
    while (j < k) 
    {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
        j++;
        k--;
    }
}
