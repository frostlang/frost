#include <parse/token.h>

namespace Frost::Parse{

const char* token_debug[]={
    "END",
    "UNKNOWN",
    "UNDERSCORE",
    "IDENTIFIER",

    "STRING",
    "MULTILINE_STRING",

    "NEWLINE",

    "ASM",

    "COLON",
    "SEMICOLON",
    "COMMA",
    "DOT",

    "LCURLY",
    "RCURLY",
    "LPAREN",
    "RPAREN",
    "LBRACKET",
    "RBRACKET",

    "QUOTE",
    "APOSTROPHE",
    "NUMBER",
    "TRUE",
    "FALSE",

    "THIS",

    "ANY",
    "U0",
    "U1",
    "U8",
    "S8",
    "U16",
    "S16",
    "U32",
    "S32",
    "F32",
    "U64",
    "S64",
    "F64",
    "FN",
    "TYPE",
    "MODULE",

    "ASSIGN",

    "PLUS",
    "MINUS",
    "STAR",
    "DIV",
    "MODULO",
    "REMAINDER",

    "INCREMENT",
    "DECREMENT",

    "LAND", // logial and
    "LOR",
    "LNOT",
    "BOR",
    "BAND",
    "BNOT",

    "EQUALS",
    "NEQUALS",

    "ARROW",
    "AMPERSAND",

    "BREAK",
    "CONTINUE",
    "RETURN",
    "IF",
    "ELSE",
    "FOR",
    "MATCH",

    "STRUCT",
    "INTERFACE",

    "PUB",
    "PRIV",
    "MUT",
    "CONST"
};
}