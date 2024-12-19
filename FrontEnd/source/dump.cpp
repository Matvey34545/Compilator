#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dump.h"

const char *DOT_FILE = "dump/dump.dot";
const char *PNG_FILE = "img/dot.png";

static void create_dot_top(const Node *top, FILE *fp);
static void create_communication(const Node *top, FILE *fp);
static ErrorFrontEnd create_dot_file(const char *filename, const BynarTree *tree);
static ErrorFrontEnd create_png_file(const char *filename, const char *png_file);

static void print_node(FILE* fp, const Token* token);
static const char* print_enum(KeyWords key_word);
static void my_print_str(FILE* fp, const Token* token);

ErrorFrontEnd dump(const BynarTree *tree, const char *filename, const char *png_file)
{
    ErrorFrontEnd error = create_dot_file(filename, tree);
    if (error != FRONT_ERROR_NO)
        return error;
        
    return create_png_file(filename, png_file); 
}

static ErrorFrontEnd create_png_file(const char *filename, const char *png_file)
{
    if (png_file == NULL)
        png_file = PNG_FILE;

    if (filename == NULL)
        filename = DOT_FILE;

    int size_str = strlen("dot -Tpng ") + strlen(" -o ") + strlen(filename) + strlen(png_file) + 1;

    char* str = (char*)calloc(size_str, sizeof(char));
    char* ptr = str;

    strcpy(ptr, "dot -Tpng ");
    ptr += strlen("dot -Tpng ");

    strcpy(ptr, filename);
    ptr += strlen(filename);

    ptr = strcpy(ptr, " -o ");
    ptr += strlen(" -o ");

    ptr = strcpy(ptr, png_file);
    ptr += strlen(png_file);

    int res = system(str);
    if (res != 0)
    {
        free(str);
        return FRONT_ERROR_GRAPHIS;
    }
    free(str);

    return FRONT_ERROR_NO;
}

static ErrorFrontEnd create_dot_file(const char *filename, const BynarTree *tree)
{
    if (filename == NULL)
        filename = DOT_FILE;

    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
        return FRONT_ERROR_OPEN_FILE;

    fprintf(fp, "digraph list{\nrankdir = HR\n");

    create_dot_top(tree->root, fp);

    fprintf(fp, "edge[color=black]\n");

    if (tree->root != NULL)
        create_communication(tree->root, fp);

    fprintf(fp, "}");
    fclose(fp);

    return FRONT_ERROR_NO;
}

static void create_dot_top(const Node *node, FILE *fp)
{
    if (node == NULL)
        return;

    fprintf(fp, "node%p [shape=Mrecord, style=\"filled\",  label = \"", node);
    print_node(fp, (Token*)node->elem);
    fprintf(fp, "\"]\n");

    create_dot_top(node->left, fp);
    create_dot_top(node->right, fp);
}

static void create_communication(const Node *node, FILE *fp)
{
    if (node->left != NULL)
    {
        fprintf(fp, "node%p->node%p\n", node, node->left);
        create_communication(node->left, fp);
    }
    if (node->right != NULL)
    {
        fprintf(fp, "node%p->node%p\n", node, node->right);
        create_communication(node->right, fp);
    }
}

static void print_node(FILE* fp, const Token* token)
{
    switch (token->type_key_word)
    {
        case CONST:
            fprintf(fp, "%d", token->cnst);
            break;
        
        case VARIABLE:
            my_print_str(fp, token);
            break;
        
        case FUNC:
            my_print_str(fp, token);
            fprintf(fp, "| is_decl_func: %d | is_return: %d | is_decl_const: %d", token->is_declaration_func, token->is_return_func, token->is_declaration_const);
            break;
        
        case OPERATOR:
        case SEPARATOR:
        case CD_OPERATOR:
        case DECLARATOR:
        case FUNC_DECLARATOR:
        case END_FUNC:
        case DEFOLT_FUNC:
        case MAIN_FUNC:
            fprintf(fp, "is_decl_const: %d | %s", token->is_declaration_const, print_enum(token->number_key_words));
            break;
        
        case VOID_NODE:
            fprintf(fp, "VOID_NODE");
            break;
    }
}

static void my_print_str(FILE* fp, const Token* token)
{
    for (int i = 0; i < (token->var).size_name; i++)
        fputc(*((token->var).name + i), fp);
}

static const char* print_enum(KeyWords key_word)
{
    #define DESCR_(e)     \
        case e: return #e;\
            break    
    
    switch (key_word)
    {
        DESCR_(IF);
        DESCR_(ELSE);
        DESCR_(WHILE);
        DESCR_(RETURN);

        DESCR_(INT);
        DESCR_(VOID);

        DESCR_(MAIN);
        DESCR_(PRINT);
        DESCR_(SCAN);

        DESCR_(ADD);
        DESCR_(SUB);
        DESCR_(MUL);
        DESCR_(DIV);
        DESCR_(MORE);
        DESCR_(LESS);
        DESCR_(EQUALLY);
        DESCR_(MORE_EQUALLY);
        DESCR_(LESS_EQUALLY);
        DESCR_(LOGICAL_AND);
        DESCR_(LOGICAL_OR);
        DESCR_(LOGICAL_NO);
        DESCR_(ASSIGN);

        DESCR_(SEP_LINE);
        DESCR_(SEP_PARAM_FUNC);
        DESCR_(SEP_ZONE);
        DESCR_(BACK_SEP_ZONE);
        DESCR_(SEP_EXP);
        DESCR_(BACK_SEP_EXP);

        DESCR_(DECL_FUNC);
    }
    return NULL;

    #undef DESCR_
}




