#include <debug.h>
#include <parse/token.h>
#include <parse/lexer.h>
#include <parse/parser.h>
#include <asserts.h>
#include <unit.h>
#include <type.h>
#include <gen/x86astgen.h> // crash if we include this for some reason...
#include <gen/generator.h>
#include <symtable.h>
#include <parse/analysis.h>
#include <map>
#include <iostream>
#include <string>
int main(){


    Frost::Unit u = Frost::Unit::create().from_file("c:/frost/test/frost/1.frost");

    Frost::dbg() << u << "\n";

    Frost::Parse::Lexer l = Frost::Parse::Lexer::create(&u);
    Frost::Parse::TokenStream& tokens = l.lex();

    Frost::dbg() << tokens << "\n";

    Frost::Parse::Parser p = Frost::Parse::Parser::create(&u, &tokens);
    Frost::Parse::AST* ast = p.parse();
    Frost::dbg() << "done parsing\n";

    Frost::dbg() << "ast=" << ast << "\n";
    //Frost::Parse::Analyser a = Frost::Parse::Analyser::create(&u, ast);
    //a.analyse();

    //Frost::Gen::X86::X86ASTGenerator x = Frost::Gen::X86::X86ASTGenerator::create(ast);
    //x.gen();



    Frost::Parse::CleanupVisitor cleanup;
    ast->visit(cleanup);

    return 0;
}