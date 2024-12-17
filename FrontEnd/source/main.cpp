#include <stdio.h>

#include "split_token.h"
#include "stack.h"
#include "create_bynar_tree.h"
#include "dump.h"

int main()
{
    stack_t stack = {};
    ErrorFrontEnd error = split_into_tokens(&stack, "comp.txt");
    
    if (error != FRONT_ERROR_NO)
        return error;

    // for (int i = 0; i < stack.size / sizeof(Token); i++)
    //     printf("Type: %d\nNumber: %d\n\n", ((Token*)stack.ptr + i)->type_key_word, ((Token*)stack.ptr + i)->number_key_words);

    StackForParsing main_stack = {stack, 0};
    BynarTree tree             = {};
    create_bynar_tree(&tree, sizeof(Token));

    printf("ERROR: %d\n", command_parsing(&main_stack, &tree.root));

    dump(&tree, NULL, NULL);
}