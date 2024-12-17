#ifndef STACK
#define STACK

struct stack_t
{
    void* ptr;
    size_t size;
    size_t capacity;
    size_t size_elem;
};

enum ErrorStack
{
    STACK_ERROR_NO         = 0,
    STACK_ERROR_ALLOCATION = 1,
    STACK_ERROR_NULL_PTR   = 2
};

ErrorStack create_stack(stack_t *stack_t, int capacity, size_t size_elem);
ErrorStack push_stack(stack_t *stack_t, const void *value);
ErrorStack dtor_stack(stack_t *stack_t);
ErrorStack pop_stack(stack_t *stack_t, void *value);

#endif
