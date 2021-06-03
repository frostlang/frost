#pragma once

#include <unit.h>
#include <parse/token.h>
#include <parse/ast.h>

namespace Frost::Parse{

class Parser{
public:
    Parser(Unit& u, TokenStream& t) : m_unit(u), m_tokens(t){}
    static Parser create(Unit& u, TokenStream& t){
        Parser p(u, t);
        return p;
    }

    AST* parse();

private:

    const Unit& m_unit;
    const TokenStream& m_tokens;
};

}