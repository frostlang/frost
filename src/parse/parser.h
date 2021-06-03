#pragma once

#include <unit.h>
#include <parse/token.h>
#include <parse/ast.h>

namespace Frost::Parse{

class Parser{
public:
    static Parser create(Unit* u, TokenStream* t){
        Parser p;
        p.m_unit = u;
        p.m_tokens = t;
        return p;
    }

    AST* parse();
    AST* identifier();
    AST* block();
private:

    Unit* m_unit;
    TokenStream* m_tokens;
};

}