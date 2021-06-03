#include <parse/ast.h>
#include <debug.h>

namespace Frost::Parse{

#define DEF_AST_VISIT(ast_class)                \
    AST* ast_class::visit(ASTVisitor& visitor){ \
        return visitor.visit(this);             \
    }                                           

DEF_AST_VISIT(ProgramAST)
DEF_AST_VISIT(BlockAST)



AST* CleanupVisitor::visit(ProgramAST* program_ast){

    for(auto& ast : program_ast->statements())
        ast->visit(*this);

    dbg() << "delete ProgramAST\n";

    delete program_ast;
    return 0;

}

AST* CleanupVisitor::visit(BlockAST* block_ast){

    for(auto& ast : block_ast->statements())
        ast->visit(*this);

    dbg() << "delete BlockAST\n";

    delete block_ast;
    return 0;

}

}