/**
 * @author Andrej Bartulin
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for programs' arguments
*/

#pragma once

#include "../stdint.h"

/* Constants for the LCG algorithm */
static uint16_t LCG_MULTIPLIER = 33676;
static uint16_t LCG_INCREMENT = 12345;
static uint16_t LCG_MODULUS = 65535; // 2^31

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

/**
 * Struct holding paint runtime info
 * @param background_color current background_color
*/
typedef struct
{
    uint16_t background_color;
} Paint;

/**
 * Struct holding random program runtime info
 * @param lcg_seed current seed
*/
typedef struct
{
    uint16_t lcg_seed;
} Random;

/**
 * Struct holding shell runtime info
 * @param index current index in `line` array
 * 
 * @param line content of current line
 * @param history shell's history
*/
typedef struct
{
    int index;

    char line[60];
    char* history[5];
} Shell;

/**
 * Struct holding information which is passed to program's function
 * @param calculator data needed for calculator program
 * @param paint data needed for paint program
 * @param random data needed for random program
 * @param shell data needed for shell program
*/
typedef struct
{
    Calculator* calculator;
    Paint* paint;
    Random* random;
    Shell* shell;
} Argument;
