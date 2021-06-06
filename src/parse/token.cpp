#include <parse/token.h>

namespace Frost::Parse{

const char* token_debug[]={
    "UNKNOWN",
    "UNDERSCORE",
    "IDENTIFIER",

    "COLON",
    "SEMICOLON",

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

    "ANY",
    "U0",
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
    "BOR",
    "BAND",

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