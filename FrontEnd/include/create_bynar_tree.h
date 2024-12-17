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
ErrorFrontEnd expression_parsing(StackForParsing *stack, Node** node, int prioritet_operator);
ErrorFrontEnd command_parsing(StackForParsing *stack, Node** node);

#endif