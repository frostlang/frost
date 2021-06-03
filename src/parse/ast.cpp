#include <parse/ast.h>
#include <debug.h>

namespace Frost::Parse{

#define DEF_AST_VISIT(ast_class)                \
    AST* ast_class::visit(ASTVisitor& visitor){ \
        return visitor.visit(this);             \
    }                                           

DEF_AST_VISIT(ProgramAST)



AST* CleanupVisitor::visit(ProgramAST* program_ast){

    dbg() << "delete ProgramAST\n";

    delete program_ast;
    return 0;

}


}