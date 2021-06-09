#include <vector>
#include <string>
#include <string_view>
#include <gen/generator.h>
#include <parse/ast.h>
#include <debug.h>
#include <gen/x86.h>

namespace Frost::Gen::X86{

//
// NOTE this class takes the AST and generates 
//
class X86ASTGenerator{// : ASTGenerator{
public:
    static X86ASTGenerator create(Parse::AST* ast){
        X86ASTGenerator x;
        x.m_ast = ast;
        return x;
    }
    
    void gen();

    void emit(const char*);
    
    Optional<X86::Operand> visit(Parse::AST*, X86::BuildContext&);
    Optional<X86::Operand> visit(Parse::ProgramAST*, X86::BuildContext&);
    Optional<X86::Operand> visit(Parse::IfAST* if_ast, X86::BuildContext& ctx);
    Optional<X86::Operand> visit(Parse::ReturnAST* return_ast, X86::BuildContext& ctx);
    Optional<X86::Operand> visit(Parse::DeclAST*, X86::BuildContext&);
    Optional<X86::Operand> visit(Parse::BinOpAST*, X86::BuildContext&);
    Optional<X86::Operand> visit(Parse::VariableAST*, X86::BuildContext&);
    Optional<X86::Operand> visit(Parse::LiteralAST*, X86::BuildContext&);

private:
    Parse::AST* m_ast;
};

extern std::vector<X86::InstructionEncoding> instruction_lookup_table;

}