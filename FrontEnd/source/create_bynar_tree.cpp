#include <stdio.h>

#include "create_bynar_tree.h"
#include "syntax.h"

const int MAX_BYNARY_PRIORITET = 6;

static ErrorFrontEnd declaration_var_parsing(StackForParsing *stack, Node** node, TypeKeyWords type);

static ErrorFrontEnd unary_parsing(StackForParsing *stack, Node** node);
static ErrorFrontEnd const_parsing(StackForParsing *stack, Node** node);

static ErrorFrontEnd func_parsing(StackForParsing *stack, Node** node);
static ErrorFrontEnd func_parametr_parsing(StackForParsing *stack, Node** node);

static ErrorFrontEnd declaration_func_parsing(StackForParsing *stack, Node** node);
static ErrorFrontEnd declaration_func_param_parsing(StackForParsing *stack, Node** node);
static ErrorFrontEnd body_func_parsing(StackForParsing *stack, Node** node);
static ErrorFrontEnd cd_operator_parsing(StackForParsing *stack, Node** node);

static ErrorFrontEnd check_type_and_create_node(StackForParsing *stack, Node** node, TypeKeyWords type);
static ErrorFrontEnd create_new_top(StackForParsing *stack, Node** node);


ErrorFrontEnd main_parsing(StackForParsing *stack, BynarTree* tree)
{
    if (tree == NULL)
        return FRONT_ERROR_NULL_PTR;

    ErrorFrontEnd error = declaration_var_parsing(stack, &tree->root, DECLARATOR);
    if (error != FRONT_ERROR_NO)
        return error;

    if ( ((Token*)(stack->stack).ptr + stack->counter)->type_key_word != END_CODE)
        return FRONT_ERROR_SYNTAX;

    return FRONT_ERROR_NO;
}

static ErrorFrontEnd declaration_var_parsing(StackForParsing *stack, Node** node, TypeKeyWords type)
{
    ErrorFrontEnd error = check_type_and_create_node(stack, node, type);
    if (error != FRONT_ERROR_NO)
        return error;

    error = expression_parsing(stack, &(*node)->left, MAX_BYNARY_PRIORITET);
    if (error != FRONT_ERROR_NO)
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != SEP_LINE)
        return FRONT_ERROR_SYNTAX;

    (stack->counter)++;
    return FRONT_ERROR_NO;
}

ErrorFrontEnd expression_parsing(StackForParsing *stack, Node** node, int prioritet_operator)
{
    if (prioritet_operator == 0)
        return unary_parsing(stack, node);

    Node* left_subtree  = NULL;
    Node* right_subtree = NULL;

    ErrorFrontEnd error = expression_parsing(stack, &left_subtree, prioritet_operator - 1); 
    if (error != FRONT_ERROR_NO) 
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->type_key_word == OPERATOR && 
        (((Token*)(stack->stack).ptr + stack->counter)->operator_t).byn_prioritet == prioritet_operator) 
    {
        error = create_new_top(stack, node);
        if (error != FRONT_ERROR_NO) 
            return error;
        
        error = expression_parsing(stack, &right_subtree, prioritet_operator);
        if (error != FRONT_ERROR_NO) 
            return error;

        (*node)->left  = left_subtree;
        (*node)->right = right_subtree;
    }
    else
        *node = left_subtree;
    
    return FRONT_ERROR_NO;        
}

static ErrorFrontEnd unary_parsing(StackForParsing *stack, Node** node)
{  
    if (((Token*)(stack->stack).ptr + stack->counter)->type_key_word == OPERATOR &&
        ( (((Token*)(stack->stack).ptr + stack->counter)->operator_t).type_operator == UNARY ||
          (((Token*)(stack->stack).ptr + stack->counter)->operator_t).type_operator == MIXED) )
    {
        ErrorFrontEnd error = create_new_top(stack, node);
        if (error != FRONT_ERROR_NO)
            return error;

        return unary_parsing(stack, &(*node)->left);   
    }

    return func_parsing(stack, node);
}

static ErrorFrontEnd func_parsing(StackForParsing *stack, Node** node)
{
    if (((Token*)(stack->stack).ptr + stack->counter)->type_key_word != FUNC)
        return const_parsing(stack, node);

    ErrorFrontEnd error = create_new_top(stack, node);
    if (error != FRONT_ERROR_NO)
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != SEP_EXP)
        return FRONT_ERROR_SYNTAX;

    (stack->counter)++;
    return func_parametr_parsing(stack, &(*node)->left);
}

static ErrorFrontEnd func_parametr_parsing(StackForParsing *stack, Node** node)
{
    ErrorBynarTree error_tree = NewNode(VOID_NODE, 0, node);
    if (error_tree != BYNAR_ERROR_MO)
        return FRONT_ERROR_TREE;

    ErrorFrontEnd error = expression_parsing(stack, &(*node)->left, MAX_BYNARY_PRIORITET);
    if (error != FRONT_ERROR_NO)
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words == SEP_PARAM_FUNC)
    {
        (stack->counter)++;
        return func_parametr_parsing(stack, &(*node)->right);
    }
    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != BACK_SEP_EXP)
        return FRONT_EXPECTED_BACK_SEP_EXP;
    
    (stack->counter)++;
    return FRONT_ERROR_NO;
}

static ErrorFrontEnd const_parsing(StackForParsing *stack, Node** node)
{
    if (((Token*)(stack->stack).ptr + stack->counter)->type_key_word == CONST ||
        ((Token*)(stack->stack).ptr + stack->counter)->type_key_word == VARIABLE )
        return create_new_top(stack, node);
        
    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words == SEP_EXP)
    {
        (stack->counter)++;
        ErrorFrontEnd error = expression_parsing(stack, node, MAX_BYNARY_PRIORITET);
        if (error != FRONT_ERROR_NO)
            return error;

        if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != BACK_SEP_EXP)
            return FRONT_EXPECTED_BACK_SEP_EXP;

        (stack->counter)++;
        return FRONT_ERROR_NO;
    }
    return FRONT_ERROR_SYNTAX;
}

static ErrorFrontEnd declaration_func_parsing(StackForParsing *stack, Node** node)
{
    ErrorFrontEnd error = check_type_and_create_node(stack, node, FUNC_DECLARATOR);
    if (error != FRONT_ERROR_NO)
        return error;

    error = check_type_and_create_node(stack, &(*node)->left, DECLARATOR);
    if (error != FRONT_ERROR_NO)
        return error;

    error = check_type_and_create_node(stack, &(*node)->left->left, FUNC);
    if (error != FRONT_ERROR_NO)
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != SEP_EXP)
        return FRONT_ERROR_SYNTAX;

    (stack->counter)++;

    error = declaration_func_param_parsing(stack, &(*node)->left->left->left);
    if (error != FRONT_ERROR_NO)
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words == SEP_ZONE)
        return body_func_parsing(stack, &(*node)->left->left->right);

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != SEP_LINE)
        return FRONT_ERROR_SYNTAX;

    (stack->counter)++;
    return FRONT_ERROR_NO;
}

static ErrorFrontEnd declaration_func_param_parsing(StackForParsing *stack, Node** node)
{
    ErrorBynarTree error_tree = NewNode(VOID_NODE, 0, node);
    if (error_tree != BYNAR_ERROR_MO)
        return FRONT_ERROR_TREE;

    ErrorFrontEnd error = check_type_and_create_node(stack, &(*node)->left, DECLARATOR);
    if (error != FRONT_ERROR_NO)
        return error;
    
    error = check_type_and_create_node(stack, &(*node)->left->left, VARIABLE);
    if (error != FRONT_ERROR_NO)
        return error;
    
    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words == SEP_PARAM_FUNC)
    {
        (stack->counter)++;
        return declaration_func_param_parsing(stack, &(*node)->right);
    }
    
    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != BACK_SEP_EXP)
        return FRONT_EXPECTED_BACK_SEP_EXP;
    
    (stack->counter)++;
    return FRONT_ERROR_NO;
}

static ErrorFrontEnd body_func_parsing(StackForParsing *stack, Node** node)
{
    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != SEP_ZONE)
        return FRONT_ERROR_SYNTAX;
    
    (stack->counter)++;

    ErrorFrontEnd error = command_parsing(stack, node);
    if (error != FRONT_ERROR_NO)
        return error;

    if (((Token*)(stack->stack).ptr + stack->counter)->number_key_words != BACK_SEP_ZONE)
        return FRONT_ERROR_SYNTAX;
    
    (stack->counter)++;
    return FRONT_ERROR_NO;  
}

static ErrorFrontEnd cd_operator_parsing(StackForParsing *stack, Node** node)
{
    ErrorFrontEnd error = check_type_and_create_node(stack, node, CD_OPERATOR);
    if (error != FRONT_ERROR_NO)
        return error;

    error = expression_parsing(stack, &(*node)->left, MAX_BYNARY_PRIORITET);
    if (error != FRONT_ERROR_NO)
        return error;

    return body_func_parsing(stack, &(*node)->right);
}

ErrorFrontEnd command_parsing(StackForParsing *stack, Node** node)
{
    ErrorBynarTree error_tree = NewNode(VOID_NODE, 0, node);
    if (error_tree != BYNAR_ERROR_MO)
        return FRONT_ERROR_TREE;

    ErrorFrontEnd error = FRONT_ERROR_NO;
    bool is_read = false;

    switch (((Token*)(stack->stack).ptr + stack->counter)->type_key_word)
    {
        case DECLARATOR:
        case END_FUNC:
            error = declaration_var_parsing(stack, &(*node)->left, ((Token*)(stack->stack).ptr + stack->counter)->type_key_word);
            if (error != FRONT_ERROR_NO)
                return error;
            is_read = true;
            break;
        
        case FUNC_DECLARATOR:
            error = declaration_func_parsing(stack, &(*node)->left);
            if (error != FRONT_ERROR_NO)
                return error;
            is_read = true;
            break;
        
        case CD_OPERATOR:
            error = cd_operator_parsing(stack, &(*node)->left);
            if (error != FRONT_ERROR_NO)
                return error;
            is_read = true;
            break; 
    }
    if (is_read)
        return command_parsing(stack, &(*node)->right);

    return FRONT_ERROR_NO;
}

static ErrorFrontEnd check_type_and_create_node(StackForParsing *stack, Node** node, TypeKeyWords type)
{
    if (((Token*)(stack->stack).ptr + stack->counter)->type_key_word != type)
        return FRONT_ERROR_SYNTAX;

    return create_new_top(stack, node);
}

static ErrorFrontEnd create_new_top(StackForParsing *stack, Node** node)
{
    ErrorBynarTree error_tree = create_bynar_top((Token*)(stack->stack).ptr + stack->counter, sizeof(Token), node);
    if (error_tree != BYNAR_ERROR_MO)
        return FRONT_ERROR_TREE;

    (stack->counter)++;
    return FRONT_ERROR_NO;    
}







