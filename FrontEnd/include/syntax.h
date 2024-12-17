#ifndef SYNTAX
#define SYNTAX

#include <stdlib.h>

enum KeyWords
{
    IF             = 0,
    ELSE           = 1,
    WHILE          = 2,
    RETURN         = 3,

    INT            = 4,
    VOID           = 5,
    CONST_VAR      = 6,

    MAIN           = 7,
    PRINT          = 8,
    SCAN           = 9,

    ADD            = 10,
    SUB            = 11,
    MUL            = 12,
    DIV            = 13,
    EQUALLY        = 14,
    MORE_EQUALLY   = 15,
    LESS_EQUALLY   = 16,
    MORE           = 17,
    LESS           = 18,
    LOGICAL_AND    = 19,
    LOGICAL_OR     = 20,
    LOGICAL_NO     = 21,
    ASSIGN         = 22,

    SEP_LINE       = 23,
    SEP_PARAM_FUNC = 24,
    SEP_ZONE       = 25,
    BACK_SEP_ZONE  = 26,
    SEP_EXP        = 27,
    BACK_SEP_EXP   = 28,

    DECL_FUNC      = 29
};

enum TypeKeyWords
{
    OPERATOR        = 0,
    DECLARATOR      = 1,
    CD_OPERATOR     = 2,
    SEPARATOR       = 3,
    FUNC            = 5,
    CONST           = 6,
    VARIABLE        = 7,
    UNIDENTIFIED    = 8,
    END_CODE        = 9,
    VOID_NODE       = 10,
    END_FUNC        = 11,
    FUNC_DECLARATOR = 12
};

struct Variable 
{
    char* name;
    size_t size_name;
};

enum TypeOperator
{
    UNARY   = 0,
    BYNARY  = 1,
    MIXED   = 2
};

struct Operator_t 
{
    TypeOperator type_operator;
    int byn_prioritet; 
};

struct Token
{
    TypeKeyWords type_key_word;
    KeyWords number_key_words;
    int prioritet_declarator;
    union 
    {
        Operator_t operator_t;
        int cnst;
        Variable var;
        bool is_declaration_func;
    };
};

struct InfoToken
{
    const char* name;
    size_t size_name;
    bool is_sep_spaces_left;
    bool is_sep_spaces_right;
    Token token;    
};      


const InfoToken info_words[] =    {{"if",        sizeof("if")       - 1, true,  true,  {CD_OPERATOR,     IF,             0                        }},
                                   {"else",      sizeof("else")     - 1, true,  true,  {CD_OPERATOR,     ELSE,           0                        }},
                                   {"while",     sizeof("while")    - 1, true,  true,  {CD_OPERATOR,     WHILE,          0                        }},

                                   {"return",    sizeof("return")   - 1, true,  true,  {END_FUNC,        RETURN,         0                        }}, 

                                   {"int",       sizeof("int")      - 1, true,  true,  {DECLARATOR,      INT,            1                        }},
                                   {"void",      sizeof("void")     - 1, true,  true,  {DECLARATOR,      VOID,           1                        }},
                                   {"const",     sizeof("const")    - 1, true,  true,  {DECLARATOR,      CONST_VAR,      2                        }},

                                   {"main",      sizeof("main")     - 1, true,  false, {FUNC,            MAIN,           0                        }},
                                   {"print",     sizeof("print")    - 1, true,  false, {FUNC,            PRINT,          0                        }},
                                   {"scan",      sizeof("scan")     - 1, true,  false, {FUNC,            SCAN,           0                        }},

                                   {"+",         sizeof("+")        - 1, false, false, {OPERATOR,        ADD,            0,    {{BYNARY,   2   }} }},
                                   {"-",         sizeof("-")        - 1, false, false, {OPERATOR,        SUB,            0,    {{MIXED,    2   }} }},
                                   {"*",         sizeof("-")        - 1, false, false, {OPERATOR,        MUL,            0,    {{BYNARY,   1   }} }},
                                   {"/",         sizeof("/")        - 1, false, false, {OPERATOR,        DIV,            0,    {{BYNARY,   1   }} }},
                                   {"==",        sizeof("==")       - 1, false, false, {OPERATOR,        EQUALLY,        0,    {{BYNARY,   3   }} }},
                                   {">=",        sizeof(">=")       - 1, false, false, {OPERATOR,        MORE_EQUALLY,   0,    {{BYNARY,   3   }} }},
                                   {"<=",        sizeof("<=")       - 1, false, false, {OPERATOR,        LESS_EQUALLY,   0,    {{BYNARY,   3   }} }},
                                   {">",         sizeof(">")        - 1, false, false, {OPERATOR,        MORE,           0,    {{BYNARY,   3   }} }},
                                   {"<",         sizeof("<")        - 1, false, false, {OPERATOR,        LESS,           0,    {{BYNARY,   3   }} }},
                                   {"&&",        sizeof("&&")       - 1, false, false, {OPERATOR,        LOGICAL_AND,    0,    {{BYNARY,   4   }} }},
                                   {"||",        sizeof("||")       - 1, false, false, {OPERATOR,        LOGICAL_OR,     0,    {{BYNARY,   5   }} }},
                                   {"!",         sizeof("!")        - 1, false, false, {OPERATOR,        LOGICAL_NO,     0,    {{UNARY,    0   }} }},
                                   {"=",         sizeof("=")        - 1, false, false, {OPERATOR,        ASSIGN,         0,    {{BYNARY,   6   }} }},

                                   {";",         sizeof(";")        - 1, false, false, {SEPARATOR,       SEP_LINE,       0                        }},
                                   {",",         sizeof(",")        - 1, false, false, {SEPARATOR,       SEP_PARAM_FUNC, 0                        }},
                                   {"{",         sizeof("{")        - 1, false, false, {SEPARATOR,       SEP_ZONE,       0                        }},
                                   {"}",         sizeof("}")        - 1, false, false, {SEPARATOR,       BACK_SEP_ZONE,  0                        }},
                                   {"(",         sizeof("(")        - 1, false, false, {SEPARATOR,       SEP_EXP,        0                        }},
                                   {")",         sizeof(")")        - 1, false, false, {SEPARATOR,       BACK_SEP_EXP,   0                        }},

                                   {"func",      sizeof("func")     - 1, true,  true, {FUNC_DECLARATOR, DECL_FUNC,      0                         }}
};

const size_t NUMBER_KEY_WORDS = sizeof(info_words) / sizeof(info_words[0]);

#endif