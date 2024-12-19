#include <stdio.h>
#include <string.h>

#include "semantical_analys.h"

const int PARAMETR_FUNC      = 4;
const int MIN_SIZE_STACK_VAR = 100;

static ErrorFrontEnd fills_information_func(const Node* node, UnitStackFunc* info_func);
static ErrorFrontEnd fills_information_param_func(const Node* node, UnitStackFunc* info_func);

static ErrorFrontEnd create_func_stack(const Node* node, stack_t* stack);
static bool comparison_func_and_var(const Variable* func_1, const Variable* func_2);
static UnitStackFunc* is_have_this_func(const Variable* func_1, const stack_t* stack);
static bool comparison_type_param_func(const UnitStackFunc* func_1, const UnitStackFunc* func_2);  
static ErrorFrontEnd check_void_used_arifmethics(const Node* node, bool is_last_node_operator);
static ErrorFrontEnd checking_definition_functions_inside_functions(const Node* node, bool is_inside_function);

static ErrorFrontEnd check_func_no_definition(const stack_t* stack);
static ErrorFrontEnd check_func_with_decl_func(const stack_t* stack, const Node* node);

static ErrorFrontEnd check_left_operator_assigment(const Node* node);
static ErrorFrontEnd check_conditional_assigment(const Node* node);
static ErrorFrontEnd create_stack_var(const Node* node, stack_t* stack, stack_t* parent_stack);
static ErrorFrontEnd check_end_function(const Node* node, int is_return_function);

static ErrorFrontEnd create_and_check_func_stack(const Node* node, stack_t* stack);

static int count_param_func(const Node* node);
static bool check_number_key_subtree(const Node* node, KeyWords key);
static ErrorFrontEnd create_my_stack(stack_t* stack, stack_t* ptr_stack);

static ErrorFrontEnd give_variable(const Node* node, Variable* var);
static ErrorFrontEnd create_new_var(const Node* node, stack_t* stack, stack_t* parent_stack, bool is_var);
static bool is_have_var(const stack_t* stack, const Variable* var);

ErrorFrontEnd semantycal_analys(BynarTree* tree, stack_t* stack_var, stack_t* stack_func)
{
    if (tree == NULL || stack_var == NULL || stack_func == NULL)
        return FRONT_ERROR_NULL_PTR;

    ErrorFrontEnd error = check_left_operator_assigment(tree->root);
    if (error != FRONT_ERROR_NO)
        return error;

    ErrorStack error_stack = create_stack(stack_func, MIN_SIZE_STACK_VAR, sizeof(UnitStackFunc));
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    error = create_and_check_func_stack(tree->root, stack_func);
    if (error != FRONT_ERROR_NO)
        return error;

    error = check_conditional_assigment(tree->root);
    if (error != FRONT_ERROR_NO)
        return error;

    error = check_end_function(tree->root, false);
    if (error != FRONT_ERROR_NO)
        return error;

    error = create_my_stack(stack_var, NULL);
    if (error != FRONT_ERROR_NO)
        return error;
    
    return create_stack_var(tree->root, stack_var, NULL);
}

static ErrorFrontEnd create_stack_var(const Node* node, stack_t* stack, stack_t* parent_stack)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    ErrorFrontEnd error = FRONT_ERROR_NO;
    stack_t* new_stack = stack;
    stack_t* old_stack = parent_stack;

    if (((Token*)node->elem)->type_key_word == VARIABLE)
    {
        if (!is_have_var(stack, &((Token*)node->elem)->var))
            return FRONT_VARIABLE_NOT_DECLARATED;
    }

    if (((Token*)node->elem)->type_key_word == FUNC && ((Token*)node->elem)->is_declaration_func)
        return create_stack_var(node, stack, parent_stack);

    if (((Token*)node->elem)->type_key_word == DECLARATOR && ((Token*)node->elem)->is_declaration_const)
    {
        Variable var = {};
        error = give_variable(node->left, &var);
        if (error != FRONT_ERROR_NO)
            return error;
        
        if (is_have_var(stack, &var))
            return FRONT_DOUBLE_DECLARATION_VAR;

        error = create_new_var(node->left, stack, parent_stack, true);
        if (error != FRONT_ERROR_NO)
            return error;
    }

    if (((Token*)node->elem)->type_key_word == FUNC && !((Token*)node->elem)->is_declaration_func ||
        ((Token*)node->elem)->type_key_word == MAIN_FUNC || ((Token*)node->elem)->type_key_word == CD_OPERATOR)
    {
        error = create_new_var(node->left, stack, parent_stack, false);
        if (error != FRONT_ERROR_NO)
            return error;
        
        new_stack = ((UnitStackVar*)((char*)stack->ptr + stack->size - sizeof(UnitStackVar)))->ptr_stack;
        old_stack = stack;
    }
    
    error = create_stack_var(node->left, new_stack, old_stack);
    if (error != FRONT_ERROR_NO)
        return error;

    return create_stack_var(node->right, new_stack, old_stack);     
}

static ErrorFrontEnd create_new_var(const Node* node, stack_t* stack, stack_t* parent_stack, bool is_var)
{
    UnitStackVar var = {};
    if (is_var)
    {
        ErrorFrontEnd error = give_variable(node, &var.var);
        if (error != FRONT_ERROR_NO)
            return error;
    }

    var.is_ptr_stack = !is_var;
    var.parent_stack = parent_stack;

    ErrorStack error_stack = STACK_ERROR_NO;
    
    if (!is_var)
    {
        var.ptr_stack = (stack_t*)calloc(1, sizeof(UnitStackVar));
        if (var.ptr_stack == NULL)
            return FRONT_ERROR_ALLOCATION;

        ErrorFrontEnd error = create_my_stack(var.ptr_stack, stack);
        if (error != FRONT_ERROR_NO)
            return error; 
    }

    error_stack = push_stack(stack, &var);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    return FRONT_ERROR_NO;
}

static ErrorFrontEnd create_my_stack(stack_t* stack, stack_t* ptr_stack)
{
    UnitStackVar temp = {ptr_stack, false};
    ErrorStack error_stack = create_stack(stack, MIN_SIZE_STACK_VAR, sizeof(UnitStackVar));
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    error_stack = push_stack(stack, &temp);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    return FRONT_ERROR_NO;
}

static bool is_have_var(const stack_t* stack, const Variable* var)
{
    if (stack == NULL)
        return false;

    if (stack->ptr == NULL)
        return false;

    for (int i = 1; i < stack->size / sizeof(UnitStackVar); i++)
    {
        if (!((UnitStackVar*)stack->ptr + i)->is_ptr_stack)
        {
            if (comparison_func_and_var(&((UnitStackVar*)stack->ptr + i)->var, var))
                return true;
        }
    }
    return is_have_var(((UnitStackVar*)stack->ptr)->parent_stack, var);
}

static ErrorFrontEnd give_variable(const Node* node, Variable* var)
{
    if (node == NULL)
        return FRONT_ERROR_SYNTAX;

    if (((Token*)node->elem)->type_key_word == VARIABLE)
    {
        memcpy(var, &((Token*)node->elem)->var, sizeof(Variable));
        return FRONT_ERROR_NO;
    }
    if (((Token*)node->elem)->number_key_words != ASSIGN)
        return FRONT_EXPECTED_ASSIGN;

    if (node->left == NULL)
        return FRONT_ERROR_SYNTAX;

    if (((Token*)node->left->elem)->type_key_word == VARIABLE)
    {
        memcpy(var, &((Token*)node->left->elem)->var, sizeof(Variable));
        if (check_number_key_subtree(node->right, ASSIGN))
            return FRONT_ERROR_SYNTAX;

        return FRONT_ERROR_NO;
    }
    return FRONT_ERROR_SYNTAX;
}

static ErrorFrontEnd check_end_function(const Node* node, int is_return_function)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (((Token*)node->elem)->type_key_word == END_FUNC)
    {
        if (is_return_function && node->left == NULL)
            return FRONT_RETURN_NON_VOID_FUNCTION;

        if (!is_return_function && node->left != NULL)
            return FRONT_HAVE_RETURN_VOID_FUNCTION;
    }
    if (((Token*)node->elem)->type_key_word == FUNC && ((Token*)node->elem)->is_return_func)
        is_return_function = true;

    ErrorFrontEnd error = check_end_function(node->left, is_return_function);
    if (error != FRONT_ERROR_NO)
        return error;

    return check_end_function(node->right, is_return_function);
}

static ErrorFrontEnd check_left_operator_assigment(const Node* node)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (((Token*)node->elem)->number_key_words == ASSIGN)
    {
        if (node->left == NULL)
            return FRONT_ERROR_TREE;

        if (((Token*)node->left->elem)->type_key_word != VARIABLE)
            return FRONT_LEFT_OPERAND_ASSIGN;
    }

    ErrorFrontEnd error = check_left_operator_assigment(node->left);
    if (error != FRONT_ERROR_NO)
        return error;

    return check_left_operator_assigment(node->right);
}

static ErrorFrontEnd check_conditional_assigment(const Node* node)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (((Token*)node->elem)->type_key_word == CD_OPERATOR || ((Token*)node->elem)->type_key_word == END_FUNC)
    {
        if (check_number_key_subtree(node->left, ASSIGN))
            return ERROR_ASSIGN_CONDITIONAL;
    }
    ErrorFrontEnd error = check_conditional_assigment(node->left);
    if (error != FRONT_ERROR_NO)
        return error;

    return check_conditional_assigment(node->right);
}

static bool check_number_key_subtree(const Node* node, KeyWords key)
{
    if (node == NULL)
        return false;

    if (((Token*)node->elem)->number_key_words == key)
        return true;

    bool is_find = check_number_key_subtree(node->left, key);
    if (is_find)
        return true;

    return check_number_key_subtree(node->right, key);
}

static ErrorFrontEnd checking_definition_functions_inside_functions(const Node* node, bool is_inside_function)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (((Token*)node->elem)->type_key_word == FUNC_DECLARATOR || ((Token*)node->elem)->type_key_word == MAIN_FUNC)
    {
        if (is_inside_function)
            return ERROR_DEFINITION_FUNC_INSIDE_FUNC;

        is_inside_function = true;
    }
    ErrorFrontEnd error = checking_definition_functions_inside_functions(node->left, is_inside_function);
    if (error != FRONT_ERROR_NO)
        return error;

    return checking_definition_functions_inside_functions(node->right, is_inside_function);
}

static ErrorFrontEnd create_and_check_func_stack(const Node* node, stack_t* stack)
{
    ErrorFrontEnd error = checking_definition_functions_inside_functions(node, false);
    if (error != FRONT_ERROR_NO)
        return error;
    
    error = create_func_stack(node, stack);
    if (error != FRONT_ERROR_NO)
        return error;

    error = check_func_no_definition(stack);
    if (error != FRONT_ERROR_NO)
        return error;

    error = check_func_with_decl_func(stack, node);
    if (error != FRONT_ERROR_NO)
        return error;

    return check_void_used_arifmethics(node, false);
}

static ErrorFrontEnd check_void_used_arifmethics(const Node* node, bool is_last_node_operator)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (is_last_node_operator)
    {
        if (((Token*)node->elem)->type_key_word == FUNC && !((Token*)node->elem)->is_return_func)
            return ERROR_VOID_USED_ARIPHMETICS;
    }

    if (((Token*)node->elem)->type_key_word == OPERATOR)
        is_last_node_operator = true;

    ErrorFrontEnd error = check_void_used_arifmethics(node->left, is_last_node_operator);
    if (error != FRONT_ERROR_NO)
        return error;

    return check_void_used_arifmethics(node->right, is_last_node_operator);
}

static ErrorFrontEnd create_func_stack(const Node* node, stack_t* stack)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    ErrorFrontEnd error = FRONT_ERROR_NO;

    if (((Token*)node->elem)->type_key_word == FUNC_DECLARATOR)
    {
        UnitStackFunc temp_func = {};
        error = fills_information_func(node->left, &temp_func);
        if (error != FRONT_ERROR_NO)
            return error;
        
        UnitStackFunc* func_find = is_have_this_func(&temp_func.func, stack); 
        if (func_find != NULL)
        {
            if (!((Token*)node->left->left->elem)->is_declaration_func)
            {
                if (!comparison_type_param_func(&temp_func, func_find))
                    return FRONT_DEFIN_DEFFERENT_DECL;

                if (func_find->number_definition > 0)
                    return FRONT_DOUBLE_DEFENITION_FUNC;

                func_find->number_definition += 1;
                return FRONT_ERROR_NO;
            }
            return ERROR_DOUBLE_DECLARATION_FUNC;
        }

        if (!((Token*)node->left->left->elem)->is_declaration_func)
            temp_func.number_definition = 1;

        ErrorStack error_stack = push_stack(stack, &temp_func);
        if (error_stack != STACK_ERROR_NO)
            return FRONT_ERROR_STACK;

        return FRONT_ERROR_NO;    
    }
    error =  create_func_stack(node->left, stack);
    if (error != FRONT_ERROR_NO)
        return error;

    return create_func_stack(node->right, stack);
}

static ErrorFrontEnd check_func_with_decl_func(const stack_t* stack, const Node* node)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (((Token*)node->elem)->type_key_word == FUNC)
    {
        UnitStackFunc* temp_func = is_have_this_func(&((Token*)node->elem)->var, stack);
        if (temp_func == NULL)
            return FRONT_FUNC_NO_DECLARATED;

        int num_param = count_param_func(node->left);
        if (num_param != temp_func->number_arg)
            return FRONT_NUMBER_ARG;

        if (temp_func->type_return != VOID)
            ((Token*)node->elem)->is_return_func = true;    
    }
    ErrorFrontEnd error = check_func_with_decl_func(stack, node->left);
    if (error != FRONT_ERROR_NO)
        return error;

    return check_func_with_decl_func(stack, node->right);
}

static int count_param_func(const Node* node)
{
    if (node == NULL)
        return 0;

    return count_param_func(node->right) + 1;
}

static ErrorFrontEnd check_func_no_definition(const stack_t* stack)
{
    for (int i = 0; i < stack->size / sizeof(UnitStackFunc); i++)
    {
        if (((UnitStackFunc*)stack->ptr + i)->number_definition == 0)
            return FRONT_FUNC_WITHOUT_DEFFINITION;
    }
    return FRONT_ERROR_NO;    
}

static UnitStackFunc* is_have_this_func(const Variable* func_1, const stack_t* stack)
{
    for (int i = 0; i < stack->size / sizeof(UnitStackFunc); i++)
    {
        if (comparison_func_and_var(func_1, &((UnitStackFunc*)stack->ptr + i)->func))
            return (UnitStackFunc*)stack->ptr + i;
    }
    return NULL;
}

static bool comparison_func_and_var(const Variable* func_1, const Variable* func_2)
{
    if (func_1->size_name != func_2->size_name)
        return false;

    if (strncmp(func_1->name, func_2->name, func_2->size_name) == 0)
        return true;

    return false;
}

static bool comparison_type_param_func(const UnitStackFunc* func_1, const UnitStackFunc* func_2)
{
    if (func_1->type_return != func_2->type_return)
        return false;

    if (func_1->number_arg != func_2->number_arg)
        return false;

    for (int i = 0; i < func_1->number_arg; i++)
    {
        if ( *((KeyWords*)(func_1->stack_type_arg).ptr + i) != *((KeyWords*)(func_2->stack_type_arg).ptr + i))
            return false;    
    }
    return true;
}

static ErrorFrontEnd fills_information_func(const Node* node, UnitStackFunc* info_func)
{
    memcpy(&info_func->func, &((Token*)node->left->elem)->var, sizeof(Variable));
    info_func->type_return = ((Token*)node->elem)->number_key_words;

    ErrorStack error_stack = create_stack(&info_func->stack_type_arg, PARAMETR_FUNC, sizeof(KeyWords));
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    ErrorFrontEnd error =  fills_information_param_func(node->left->left, info_func);
    if (error != FRONT_ERROR_NO)
        return error;

    info_func->number_arg = (info_func->stack_type_arg).size / sizeof(KeyWords);
    return FRONT_ERROR_NO;
}

static ErrorFrontEnd fills_information_param_func(const Node* node, UnitStackFunc* info_func)
{
    if (node == NULL)
        return FRONT_ERROR_NO;

    if (node->left == NULL)
        return FRONT_ERROR_NO;

    ErrorStack error_stack = push_stack(&info_func->stack_type_arg, &((Token*)node->left->elem)->number_key_words);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    return fills_information_param_func(node->right, info_func);  
}