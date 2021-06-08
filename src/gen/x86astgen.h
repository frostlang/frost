#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <gen/generator.h>
#include <gen/frostir.h>
#include <parse/ast.h>
#include <variant>
#include <debug.h>
#include <gen/x86.h>

namespace Frost::Gen{

using namespace Parse;
using namespace X86;
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
    
    Optional<Operand> visit(Parse::AST*, BuildContext&);
    Optional<Operand> visit(Parse::ProgramAST*, BuildContext&);
    Optional<Operand> visit(Parse::BinOpAST*, BuildContext&);
    Optional<Operand> visit(Parse::VariableAST*, BuildContext&);
    Optional<Operand> visit(Parse::LiteralAST*, BuildContext&);

private:
    Parse::AST* m_ast;
};

extern std::vector<InstructionEncoding> instruction_lookup_table;
}