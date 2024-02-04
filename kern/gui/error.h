/**
 * @author Andrej Bartulin, Stjepan Bilić Matišić
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Header file containing error enum
*/

#pragma once

typedef enum error_T
{
    NO_ERROR,
    EMPTY,
    TITLE_BOUNDARY_EXCEEDED,
    X_BOUNDARY_EXCEEDED,
    Y_BOUNDARY_EXCEEDED,
    LEN_X_BOUNDARY_EXCEEDED,
    LEN_Y_BOUNDARY_EXCEEDED,
} Error;
