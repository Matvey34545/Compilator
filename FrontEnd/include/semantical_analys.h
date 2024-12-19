#ifndef SEMANTICAL_ANALYS
#define SEMANTICAL_ANALYS

#include "bynar_tree.h"
#include "stack.h"
#include "split_token.h"
#include "syntax.h"

struct UnitStackVar
{
    stack_t* parent_stack;
    bool is_ptr_stack;
    union
    {
        stack_t* ptr_stack;
        Variable var;
    };
};

struct UnitStackFunc
{
    Variable func;
    KeyWords type_return;
    int number_arg;
    int number_definition;
    stack_t stack_type_arg;
};

ErrorFrontEnd semantycal_analys(BynarTree* tree, stack_t* stack_var, stack_t* stack_func);

#endif