#include <debug.h>
#include <parse/token.h>
#include <parse/lexer.h>
#include <parse/parser.h>
#include <asserts.h>
#include <unit.h>

#include <type.h>
int main(){
    using namespace Frost;

    Unit u = Unit::create().from_source("test.frost", "false true 1 2 match");

    dbg() << u << "\n";

    Parse::Lexer l = Parse::Lexer::create(&u);
    Parse::TokenStream& tokens = l.lex();

    dbg() << tokens << "\n";

    Parse::Parser p = Parse::Parser::create(&u, &tokens);
    Parse::AST* ast = p.parse();

    Parse::CleanupVisitor cleanup;
    ast->visit(cleanup);

    return 0;
}