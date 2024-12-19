#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "split_token.h"

static ErrorFrontEnd copy_file_in_memory(CompFile* comp_file, const char *filename);
static ErrorFrontEnd split_tokens(stack_t* array_tokens, CompFile* comp_file);

static ErrorFrontEnd is_const(stack_t* array_tokens, CompFile* comp_file, bool* is_key);
static ErrorFrontEnd is_key_word(stack_t* array_tokens, CompFile* comp_file, bool* is_key);
static ErrorFrontEnd read_unidettified(stack_t* array_tokens, CompFile* comp_file);

static int skip_spaces(char **str);
static bool is_first_letter_key_words(char letter);
static bool is_standart_key_word(const CompFile* comp_file, int* number_key_word);

static void fills_type_unidentified(stack_t *array_tokens);

ErrorFrontEnd split_into_tokens(stack_t* array_tokens, const char *filename)
{
    CompFile comp_file = {};
    ErrorFrontEnd error = copy_file_in_memory(&comp_file, filename);
    if (error != FRONT_ERROR_NO)
        return error;

    ErrorStack error_stack = create_stack(array_tokens, ELEMENTARY_SIZE_STACK, sizeof(Token));
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;
    
    error = split_tokens(array_tokens, &comp_file);
    if (error != FRONT_ERROR_NO)
        return error;

    fills_type_unidentified(array_tokens);
    return FRONT_ERROR_NO;
}

static void fills_type_unidentified(stack_t *array_tokens)
{
    for (int i = 0; i < array_tokens->size / sizeof(Token); i++)
    {
        if ( ((Token*)array_tokens->ptr + i)->type_key_word == UNIDENTIFIED)
        {
            if ( ((Token*)array_tokens->ptr + i + 1)->number_key_words == SEP_EXP)
                ((Token*)array_tokens->ptr + i)->type_key_word = FUNC;

            else
                ((Token*)array_tokens->ptr + i)->type_key_word = VARIABLE;    
        }
    }
}

static ErrorFrontEnd copy_file_in_memory(CompFile* comp_file, const char *filename)
{
    if (filename == NULL)
        return FRONT_ERROR_NULL_PTR;

    struct stat st = {};
    stat(filename, &st);
    comp_file->size = st.st_size;

    comp_file->ptr = (char*) calloc(comp_file->size + 1, sizeof(char));
    if (comp_file->ptr == NULL)
        return FRONT_ERROR_ALLOCATION;

    
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL)
        return FRONT_ERROR_OPEN_FILE;
    
    fread(comp_file->ptr, sizeof(char), comp_file->size, fp);

    fclose(fp);

    return FRONT_ERROR_NO;
}

static ErrorFrontEnd split_tokens(stack_t* array_tokens, CompFile* comp_file)
{
    bool is_key  = false;
    bool is_cnst = false;

    while (comp_file->size > 0)
    {
        ErrorFrontEnd error = is_key_word(array_tokens, comp_file, &is_key);
        if (error != FRONT_ERROR_NO)
            return error;

        if (!is_key)
        {
            error = is_const(array_tokens, comp_file, &is_cnst);
            if (error != FRONT_ERROR_NO)
                return error;

            if (!is_cnst)
            {
                error = read_unidettified(array_tokens, comp_file);
                if (error != FRONT_ERROR_NO)
                    return error;
            }
        }
        comp_file->size -= skip_spaces(&comp_file->ptr);
    }

    Token temp_token = {};
    temp_token.type_key_word = END_CODE;
    ErrorStack error_stack = push_stack(array_tokens, &temp_token);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    return FRONT_ERROR_NO;
}

static int skip_spaces(char **str)
{
    int count = 0;
    while (isspace(**str))
    {
        (*str)++;
        count++;
    }
    return count;
}

static ErrorFrontEnd is_const(stack_t* array_tokens, CompFile* comp_file, bool* is_key)
{
    int read_symbol = 0;
    int temp_const  = 0;
    
    if (sscanf(comp_file->ptr, "%d%n", &temp_const, &read_symbol) == 0)
    {
        *is_key = false;
        return FRONT_ERROR_NO;
    }

    Token temp_token = {};
    temp_token.type_key_word = CONST;
    temp_token.cnst          = temp_const;

    ErrorStack error_stack = push_stack(array_tokens, &temp_token);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    comp_file->ptr  += read_symbol;
    comp_file->size -= read_symbol;
    *is_key = true;

    return FRONT_ERROR_NO;
}

static ErrorFrontEnd is_key_word(stack_t* array_tokens, CompFile* comp_file, bool* is_key)
{
    comp_file->size -= skip_spaces(&comp_file->ptr);

    int number_key_word = 0;

    if (!is_standart_key_word(comp_file, &number_key_word) || 
        !isspace(*(comp_file->ptr + info_words[number_key_word].size_name)) && info_words[number_key_word].is_sep_spaces_right)
    {
        *is_key = false;
        return FRONT_ERROR_NO;
    }

    *is_key = true;
    comp_file->ptr  += info_words[number_key_word].size_name;
    comp_file->size -= info_words[number_key_word].size_name;

    ErrorStack error_stack = push_stack(array_tokens, &(info_words + number_key_word)->token);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;
            
    return FRONT_ERROR_NO;
}

static bool is_standart_key_word(const CompFile* comp_file, int* number_key_word)
{
    for (int i = 0; i < NUMBER_KEY_WORDS; i++)
    {
        if (strncmp(info_words[i].name, comp_file->ptr, info_words[i].size_name) == 0)
        {
            *number_key_word = i;
            return true;    
        }
    }
    return false;    
}

static ErrorFrontEnd read_unidettified(stack_t* array_tokens, CompFile* comp_file)
{
    comp_file->size -= skip_spaces(&comp_file->ptr);

    char* name_unidentified = comp_file->ptr;
    int size_unidentified   = 0;
    int temp_number_key     = 0;

    bool condition = true;
    while (condition)
    {
        if (isspace(*(comp_file->ptr)) || *(comp_file->ptr) == 0)
        {
            condition = false;
            continue;    
        }
        if (is_first_letter_key_words(*(comp_file->ptr)))
        {
            if (is_standart_key_word(comp_file, &temp_number_key))
            {
                if (!info_words[temp_number_key].is_sep_spaces_left)
                {
                    condition = false;
                    continue;
                }
            }
        }
        size_unidentified += 1;
        comp_file->ptr += 1;
    }

    comp_file->size -= size_unidentified;

    Token temp_token = {};
    temp_token.var.name      = name_unidentified;
    temp_token.var.size_name = size_unidentified;
    temp_token.type_key_word = UNIDENTIFIED;

    ErrorStack error_stack = push_stack(array_tokens, &temp_token);
    if (error_stack != STACK_ERROR_NO)
        return FRONT_ERROR_STACK;

    return FRONT_ERROR_NO;
}

static bool is_first_letter_key_words(char letter)
{
    for (int i = 0; i < NUMBER_KEY_WORDS; i++)
    {
        if (letter == *(info_words[i].name))
            return true;
    }
    return false;
}

