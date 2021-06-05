#include <gen/generator.h>
#include <gen/frostir.h>
#include <parse/ast.h>

namespace Frost::Gen{

using namespace Parse;
//
// NOTE this class takes the AST and generates 
//
class X86ASTGenerator : ASTGenerator{

public:
    static X86ASTGenerator create(Parse::AST* ast){
        X86ASTGenerator x;
        x.m_ast = ast;
        return x;
    }
    
    void gen();

    void emit(const char*);

private:
    Parse::AST* m_ast;
};

}