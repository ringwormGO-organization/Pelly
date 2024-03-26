#pragma once

#include "../stdint.h"

/**
 * Struct holding calculator runtime info
 * @param first_number first number entered in calculator
 * @param op operator
 * @param second_number second number entered in calculator
 * 
 * @param level what is user entering: first number, operator or second number
*/
typedef struct
{
    uint16_t first_number;
    char op;
    uint16_t second_number;

    bool is_first_number;
} Calculator;

typedef struct
{
    int index;

    char line[60];
    char* history[5];
} Shell;

/**
 * Struct holding information which is passed to program's function
 * @param calculator data needed for calculator program
*/
typedef struct
{
    Calculator* calculator;
    Shell* shell;
} Argument;
