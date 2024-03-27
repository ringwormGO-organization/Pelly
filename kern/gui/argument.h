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

/* Constants */
#define EXPLORER_SIZE 5
#define HISTORY_SIZE 5
#define LINE_SIZE 60 /* len_x - 4 */
#define TEXT_SIZE 180 /* 36 (len_x - 4) * 5 (len_y - 11) */

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
 * Struct holding file explorer runtime info
 * @param index index of last written file
 * @param files BBFS v2 files (RAM); 
 *        TODO: make BBFS write file structure to RAM, 
 *              implement read function for that,
 *              and don't use char arrays
*/
typedef struct
{
    int index;
    char* files[EXPLORER_SIZE];
} FileExplorer;

/**
 * Struct holding notepad runtime info
 * @param index current index in `text` array
 * @param text content of current file
*/
typedef struct
{
    int index;
    char text[TEXT_SIZE];
} Notepad;

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

    char line[LINE_SIZE];
    char* history[HISTORY_SIZE];
} Shell;

/**
 * Struct holding information which is passed to program's function
 * @param calculator data needed for calculator program
 * @param file_explorer data needed for file explorer program
 * @param notepad data needed for notepad program
 * @param paint data needed for paint program
 * @param random data needed for random program
 * @param shell data needed for shell program
*/
typedef struct
{
    Calculator* calculator;
    FileExplorer* file_explorer;
    Notepad* notepad;
    Paint* paint;
    Random* random;
    Shell* shell;
} Argument;
