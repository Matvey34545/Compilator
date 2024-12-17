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


const InfoToken info_words[] =    {{"if",        sizeof("if")       - 1, true,  true,  {CD_OPERATOR,     IF,                                     }},
                                   {"else",      sizeof("else")     - 1, true,  true,  {CD_OPERATOR,     ELSE,                                   }},
                                   {"while",     sizeof("while")    - 1, true,  true,  {CD_OPERATOR,     WHILE,                                  }},

                                   {"return",    sizeof("return")   - 1, true,  true,  {END_FUNC,        RETURN,                                 }}, 

                                   {"int",       sizeof("int")      - 1, true,  true,  {DECLARATOR,      INT,                                    }},
                                   {"void",      sizeof("void")     - 1, true,  true,  {DECLARATOR,      VOID,                                   }},
                                   {"const",     sizeof("const")    - 1, true,  true,  {DECLARATOR,      CONST_VAR,                              }},

                                   {"main",      sizeof("main")     - 1, true,  false, {FUNC,            MAIN,                                   }},
                                   {"print",     sizeof("print")    - 1, true,  false, {FUNC,            PRINT,                                  }},
                                   {"scan",      sizeof("scan")     - 1, true,  false, {FUNC,            SCAN,                                   }},

                                   {"+",         sizeof("+")        - 1, false, false, {OPERATOR,        ADD,                 {{BYNARY,   2   }} }},
                                   {"-",         sizeof("-")        - 1, false, false, {OPERATOR,        SUB,                 {{MIXED,    2   }} }},
                                   {"*",         sizeof("-")        - 1, false, false, {OPERATOR,        MUL,                 {{BYNARY,   1   }} }},
                                   {"/",         sizeof("/")        - 1, false, false, {OPERATOR,        DIV,                 {{BYNARY,   1   }} }},
                                   {"==",        sizeof("==")       - 1, false, false, {OPERATOR,        EQUALLY,             {{BYNARY,   3   }} }},
                                   {">=",        sizeof(">=")       - 1, false, false, {OPERATOR,        MORE_EQUALLY,        {{BYNARY,   3   }} }},
                                   {"<=",        sizeof("<=")       - 1, false, false, {OPERATOR,        LESS_EQUALLY,        {{BYNARY,   3   }} }},
                                   {">",         sizeof(">")        - 1, false, false, {OPERATOR,        MORE,                {{BYNARY,   3   }} }},
                                   {"<",         sizeof("<")        - 1, false, false, {OPERATOR,        LESS,                {{BYNARY,   3   }} }},
                                   {"&&",        sizeof("&&")       - 1, false, false, {OPERATOR,        LOGICAL_AND,         {{BYNARY,   4   }} }},
                                   {"||",        sizeof("||")       - 1, false, false, {OPERATOR,        LOGICAL_OR,          {{BYNARY,   5   }} }},
                                   {"!",         sizeof("!")        - 1, false, false, {OPERATOR,        LOGICAL_NO,          {{UNARY,    0   }} }},
                                   {"=",         sizeof("=")        - 1, false, false, {OPERATOR,        ASSIGN,              {{BYNARY,   6   }} }},

                                   {";",         sizeof(";")        - 1, false, false, {SEPARATOR,       SEP_LINE,                               }},
                                   {",",         sizeof(",")        - 1, false, false, {SEPARATOR,       SEP_PARAM_FUNC,                         }},
                                   {"{",         sizeof("{")        - 1, false, false, {SEPARATOR,       SEP_ZONE,                               }},
                                   {"}",         sizeof("}")        - 1, false, false, {SEPARATOR,       BACK_SEP_ZONE,                          }},
                                   {"(",         sizeof("(")        - 1, false, false, {SEPARATOR,       SEP_EXP,                                }},
                                   {")",         sizeof(")")        - 1, false, false, {SEPARATOR,       BACK_SEP_EXP,                           }},

                                   {"func",      sizeof("func")     - 1, true,  true, {FUNC_DECLARATOR, DECL_FUNC,                               }}
};

const size_t NUMBER_KEY_WORDS = sizeof(info_words) / sizeof(info_words[0]);

#endif