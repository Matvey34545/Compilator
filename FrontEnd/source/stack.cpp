#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "stack.h"

const int MIN_CAPACITY = 100;

static ErrorStack reallocation_stack(stack_t *stack_t);
static ErrorStack check_on_null_ptr(const stack_t *stack_t, const void *value);

ErrorStack create_stack(stack_t *stack_t, int capacity, size_t size_elem)
{
    if (stack_t == NULL)
        return STACK_ERROR_NULL_PTR;

    stack_t->size_elem = size_elem;
    stack_t->size = 0;
    stack_t->capacity = capacity;

    stack_t->ptr = calloc(capacity, size_elem);
    if (stack_t->ptr == NULL)
        return STACK_ERROR_ALLOCATION;

    return STACK_ERROR_NO;
}

ErrorStack dtor_stack(stack_t *stack_t)
{
    if (stack_t == NULL)
        return STACK_ERROR_NULL_PTR;

    if (stack_t->ptr == NULL)
        return STACK_ERROR_NULL_PTR;

    free(stack_t->ptr);
    return STACK_ERROR_NO;
}

ErrorStack push_stack(stack_t *stack_t, const void *value)
{
    ErrorStack  error = check_on_null_ptr(stack_t, value);
    if (error != STACK_ERROR_NO)
        return error;

    stack_t->size += stack_t->size_elem;

    error = reallocation_stack(stack_t);
    if (error != STACK_ERROR_NO)
        return error;

    memcpy((char*)stack_t->ptr + stack_t->size - stack_t->size_elem, value, stack_t->size_elem);
    return STACK_ERROR_NO;
}

ErrorStack pop_stack(stack_t *stack_t, void *value)
{
    ErrorStack  error = check_on_null_ptr(stack_t, value);
    if (error != STACK_ERROR_NO)
        return error;

    stack_t->size -= stack_t->size_elem;

    error = reallocation_stack(stack_t);
    if (error != STACK_ERROR_NO)
        return error;

    memcpy(value, (char*)stack_t->ptr + stack_t->size, stack_t->size_elem);
    return STACK_ERROR_NO;
}

static ErrorStack reallocation_stack(stack_t *stack_t)
{
    const int SIZE_REALLOC = 2;
    void *temp = NULL;

    if (stack_t->size >= stack_t->capacity - stack_t->size_elem || stack_t->size >= stack_t->capacity)
    {
        temp = realloc(stack_t->ptr, stack_t->capacity * SIZE_REALLOC);
        if (temp == NULL)
            return STACK_ERROR_ALLOCATION;
        
        stack_t->capacity *= SIZE_REALLOC;
        stack_t->ptr = temp;
    }

    if (stack_t->size < stack_t->capacity / SIZE_REALLOC / SIZE_REALLOC && stack_t->size > MIN_CAPACITY)
    {
        temp = realloc(stack_t->ptr, stack_t->capacity / SIZE_REALLOC);
        if (temp == NULL)
            return STACK_ERROR_ALLOCATION;

        stack_t->capacity /= SIZE_REALLOC;
        stack_t->ptr = temp;
    }
    return STACK_ERROR_NO;
}

static ErrorStack check_on_null_ptr(const stack_t *stack_t, const void *value)
{
    if (stack_t == NULL)
        return STACK_ERROR_NULL_PTR;

    if (stack_t->ptr == NULL)
        return STACK_ERROR_NULL_PTR;

    if (value == NULL)
        return STACK_ERROR_NULL_PTR;

    return STACK_ERROR_NO;
}






