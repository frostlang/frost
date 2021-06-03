#pragma once

#include <unit.h>
#include <parse/token.h>

namespace Frost::Parse{

class Lexer{

public:

    Lexer(Unit& u) : m_unit(u){}

    static Lexer create(Unit& u){
        Lexer l(u);
        return l;
    }

    TokenStream& lex();


private:
    TokenStream m_tokens;
    const Unit& m_unit;
};

}