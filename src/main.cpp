#include <debug.h>
#include <parse/token.h>
#include <parse/lexer.h>
#include <parse/parser.h>
#include <asserts.h>
#include <unit.h>
#include <type.h>
#include <gen/basicastexecutor.h>

int main(){
    using namespace Frost;


    Unit u = Unit::create().from_file("c:/frost/test/frost/1.frost");

    dbg() << u << "\n";

    Parse::Lexer l = Parse::Lexer::create(&u);
    Parse::TokenStream& tokens = l.lex();

    dbg() << tokens << "\n";

    Parse::Parser p = Parse::Parser::create(&u, &tokens);
    Parse::AST* ast = p.parse();


    Gen::BasicASTExecutor executor = Gen::BasicASTExecutor::create(ast);
    executor.exec();


    Parse::CleanupVisitor cleanup;
    ast->visit(cleanup);

    return 0;
}