#ifndef SPLIT_TOKEN 
#define SPLIT_TOKEN

#include "syntax.h"
#include "stack.h"

const int ELEMENTARY_SIZE_STACK = 100;

enum ErrorFrontEnd
{
    FRONT_ERROR_NO                    = 0,
    FRONT_ERROR_ALLOCATION            = 1,
    FRONT_ERROR_NULL_PTR              = 2,
    FRONT_ERROR_OPEN_FILE             = 3,
    FRONT_ERROR_STACK                 = 4,
    FRONT_ERROR_SYNTAX                = 5,
    FRONT_ERROR_TREE                  = 6,
    FRONT_EXPECTED_DECLARATOR         = 7,
    FRONT_EXPECTED_VAR_OR_FUNC        = 8,
    FRONT_ERROR_DECLARATOR            = 9,
    FRONT_EXPECTED_BACK_SEP_EXP       = 10,
    FRONT_ERROR_GRAPHIS               = 11,
    FRONT_EXPECTED_FUNC               = 12,
    FRONT_EXPECTED_SEP_LINE           = 13,
    FRONT_LEFT_OPERAND_ASSIGN         = 14,
    ERROR_DEFINITION_FUNC_INSIDE_FUNC = 15,
    FRONT_DOUBLE_DEFENITION_FUNC      = 16,
    ERROR_DOUBLE_DECLARATION_FUNC     = 17,
    FRONT_FUNC_WITHOUT_DEFFINITION    = 19,
    FRONT_FUNC_NO_DECLARATED          = 20,
    FRONT_DEFIN_DEFFERENT_DECL        = 21,
    FRONT_NUMBER_ARG                  = 22,
    ERROR_VOID_USED_ARIPHMETICS       = 23,
    ERROR_ASSIGN_CONDITIONAL          = 24,
    FRONT_EXPECTED_ASSIGN             = 25,
    FRONT_INVALID_DECLARATOR          = 26,
    FRONT_VARIABLE_NOT_DECLARATED     = 27,
    FRONT_RETURN_NON_VOID_FUNCTION    = 28,
    FRONT_HAVE_RETURN_VOID_FUNCTION   = 29,
    FRONT_DOUBLE_DECLARATION_VAR      = 30
};

struct CompFile
{
    char* ptr;
    size_t size;
};

ErrorFrontEnd split_into_tokens(stack_t* array_tokens, const char *filename);

#endif