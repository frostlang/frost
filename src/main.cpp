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

    Type t = Type::create(Type::Storage::U8);
    dbg() << t << "\n";

    Unit u = Unit::create().from_file("c:/frost/test/frost/1.frost");

    dbg() << u << "\n";

    Parse::Lexer l = Parse::Lexer::create(&u);
    Parse::TokenStream& tokens = l.lex();

    dbg() << tokens << "\n";

    Parse::Parser p = Parse::Parser::create(&u, &tokens);
    Parse::AST* ast = p.parse();


    Gen::X86ASTGenerator x = Gen::X86ASTGenerator::create(ast);


    Parse::CleanupVisitor cleanup;
    ast->visit(cleanup);


    auto add_encoding = Gen::InstructionEncoding::create("add", 04,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        );
    auto op0 = Gen::Operand::create(Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_8), Gen::Register(Gen::Register::Type::AH));
    auto op1 = Gen::Operand::create(Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8), 4);
    auto op2 = Gen::Operand::create();

    dbg() << Gen::Instruction::create("add", op0, op1, op2);

    return 0;
}