#pragma once

#include <unit.h>
#include <parse/token.h>

namespace Frost::Parse{

class Lexer{

public:

    Lexer(Unit* u) : m_unit(u){
        m_index = 0;
    }

    static Lexer create(Unit* u){
        Lexer l(u);
        return l;
    }

    TokenStream& lex();
    char current();
    char peek(u32 ahead = 0);
    char next();


private:
    TokenStream m_tokens;
    Unit* m_unit;
    u32 m_index = 0;
};

}