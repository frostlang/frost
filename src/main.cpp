#include <debug.h>
#include <parse/token.h>
#include <parse/lexer.h>
#include <parse/parser.h>
#include <asserts.h>
#include <unit.h>
#include <type.h>
#include <gen/basicastexecutor.h>
#include <gen/x86astgen.h>

int main(){
    using namespace Frost;

    Type t = Type::create(TypeType::U8);
    dbg() << t << "\n";

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


    Gen::Instruction i = Gen::Instruction::create("add", 04,
        Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_8),
        Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
        Gen::OperandEncoding::create()
    );
        Gen::Instruction i = Gen::Instruction::create("add", 04,
        Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_8),
        Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
        Gen::OperandEncoding::create()
    );

    return 0;
}