#include <stdio.h>

#include "split_token.h"
#include "stack.h"
#include "create_bynar_tree.h"
#include "semantical_analys.h"
#include "dump.h"

void dump_var(const stack_t* stack);
void print_ptr(const stack_t* stack);

int main()
{
    stack_t stack = {};
    ErrorFrontEnd error = split_into_tokens(&stack, "comp.txt");
    
    if (error != FRONT_ERROR_NO)
        return error;

    StackForParsing main_stack = {stack, 0};
    BynarTree tree             = {};
    create_bynar_tree(&tree, sizeof(Token));

    error = main_parsing(&main_stack, &tree);
    if (error != FRONT_ERROR_NO)
    {
        printf("ERROR: %d\n", error);
        return error;
    }

    stack_t stack_var = {};
    stack_t stack_func = {};
    printf("ERROR: %d", semantycal_analys(&tree, &stack_var, &stack_func));
}
