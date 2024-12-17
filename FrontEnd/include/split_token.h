#ifndef SPLIT_TOKEN 
#define SPLIT_TOKEN

#include "syntax.h"
#include "stack.h"

const int ELEMENTARY_SIZE_STACK = 100;

enum ErrorFrontEnd
{
    FRONT_ERROR_NO              = 0,
    FRONT_ERROR_ALLOCATION      = 1,
    FRONT_ERROR_NULL_PTR        = 2,
    FRONT_ERROR_OPEN_FILE       = 3,
    FRONT_ERROR_STACK           = 4,
    FRONT_ERROR_SYNTAX          = 5,
    FRONT_ERROR_TREE            = 6,
    FRONT_EXPECTED_DECLARATOR   = 7,
    FRONT_EXPECTED_VAR_OR_FUNC  = 8,
    FRONT_ERROR_DECLARATOR      = 9,
    FRONT_EXPECTED_BACK_SEP_EXP = 10,
    FRONT_ERROR_GRAPHIS         = 11,
    FRONT_EXPECTED_FUNC         = 12,
    FRONT_EXPECTED_SEP_LINE     = 13
};

struct CompFile
{
    char* ptr;
    size_t size;
};

ErrorFrontEnd split_into_tokens(stack_t* array_tokens, const char *filename);

#endif