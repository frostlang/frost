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
    void skip_whitespace();
    char current();
    char peek(u32 ahead = 0);
    char next(u32 ahead = 1);
    void number();
    void alpha();
    u1 match(const char* keyword);


private:
    TokenStream m_tokens;
    Unit* m_unit;
    u32 m_index = 0;
};

}