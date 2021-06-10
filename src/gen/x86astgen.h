#include <vector>
#include <string>
#include <string_view>
#include <gen/generator.h>
#include <parse/ast.h>
#include <debug.h>
#include <gen/x86_64.h>
#include <symtable.h>

namespace Frost::Gen::X86_64{

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
    
    Optional<X86_64::Operand> visit(Parse::AST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::ProgramAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::BlockAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::ExprStmtAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::IfAST* if_ast, X86_64::BuildContext& ctx);
    Optional<X86_64::Operand> visit(Parse::ReturnAST* return_ast, X86_64::BuildContext& ctx);
    Optional<X86_64::Operand> visit(Parse::DeclAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::AssignAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::BinOpAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::VariableAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::LiteralAST*, X86_64::BuildContext&);
    Optional<X86_64::Operand> visit(Parse::FnAST*, X86_64::BuildContext&);

private:
    Parse::AST* m_ast;
    SymTable<X86_64::Operand> m_sym_table;

};

extern std::vector<X86_64::InstructionEncoding> instruction_lookup_table;

}