#pragma once

#include <exception>
#include <unit.h>
#include <parse/token.h>
#include <type.h>
#include <parse/ast.h>

namespace Frost::Parse{

enum class ParseError : u8{
    PANIC,    // we must immediately stop
    WARNING,  // we notify that there is a problem and attempt to fix it
    SILENT,   // we don't notify and silently fix it
};

/*
we need to decide on the approach to 
*/
class ParseException : public std::exception {
public:
    ParseException(std::string msg){

    }
private:
};

class Parser{
public:
    static Parser create(Unit* u, TokenStream* t){
        Parser p;
        p.m_unit = u;
        p.m_tokens = t;
        return p;
    }

    void panic(std::string msg){
        dbg() << "PANIC=" << msg << "\n";
        throw ParseException(msg);
    }

    Optional<Type> type();
    AST* parse();
    AST* statement();
    AST* decl(); // decleration
    AST* ifstmt();
    AST* forloop();
    AST* expression();


    AST* lor();
    AST* land();
    AST* bor();
    AST* band();
    AST* eq();
    AST* cmp();
    AST* shift();
    AST* pm(); // + -
    AST* mdmr(); // * / % /%
    AST* un();
    AST* cast();

    AST* block();
    AST* group();
    AST* fn();
    AST* single();
    AST* identifier();
    AST* num();
    AST* string();
private:

    Unit* m_unit;
    TokenStream* m_tokens;
};

}