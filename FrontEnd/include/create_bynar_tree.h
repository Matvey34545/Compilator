#ifndef CREATE_BYNAR_TREE
#define CREATE_BYNAR_TREE

#include "split_token.h"
#include "bynar_tree.h"

struct StackForParsing
{
    const stack_t stack;
    int counter;
};

ErrorFrontEnd main_parsing(StackForParsing *stack, BynarTree* tree);

#endif