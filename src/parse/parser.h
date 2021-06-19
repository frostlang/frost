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

class ParseContext{
public:
    enum class Scope{
        GLOBAL,
        FN,
        BLOCK
    };
    Scope& scope(){
        return m_scope;
    }
    Frost::Type& expecting_type(){
        return m_expecting_type;
    }
private:
    Scope m_scope;
    Frost::Type m_expecting_type; // used for type inference
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
    u1 expecting_expr();
    AST* parse();
    AST* statement(ParseContext);
    AST* ret(ParseContext);
    AST* brk(ParseContext);
    AST* ctn(ParseContext);
    AST* decl(ParseContext); // decleration
    AST* ifstmt(ParseContext);
    AST* forloop(ParseContext);
    AST* expression_stmt(ParseContext);
    AST* assign(ParseContext);

    AST* expression(ParseContext);
    AST* identifier(ParseContext);

    AST* lor(ParseContext);
    AST* land(ParseContext);
    AST* bor(ParseContext);
    AST* band(ParseContext);
    AST* eq(ParseContext);
    AST* cmp(ParseContext);
    AST* shift(ParseContext);
    AST* pm(ParseContext); // + -
    AST* mdmr(ParseContext); // * / % /%
    AST* un(ParseContext);
    AST* cast(ParseContext);
    AST* call(ParseContext);

    AST* block(ParseContext);
    AST* fn(ParseContext);
    AST* single(ParseContext);
    AST* group(ParseContext);
    AST* var(ParseContext);
    AST* num(ParseContext);
    AST* string(ParseContext);
private:

    Unit* m_unit;
    TokenStream* m_tokens;
};

}