#include <parse/ast.h>
#include <debug.h>

namespace Frost::Parse{

#define DEF_AST_VISIT(ast_class)                \
    AST* ast_class::visit(ASTVisitor& visitor){ \
        return visitor.visit(this);             \
    }                                           

DEF_AST_VISIT(ErrorAST)
DEF_AST_VISIT(ProgramAST)
DEF_AST_VISIT(IfAST)
DEF_AST_VISIT(ForAST)
DEF_AST_VISIT(BlockAST)
DEF_AST_VISIT(BreakAST)
DEF_AST_VISIT(ContinueAST)



AST* CleanupVisitor::visit(ProgramAST* program_ast){

    for(auto& ast : program_ast->statements())
        ast->visit(*this);

    dbg() << "delete ProgramAST\n";

    delete program_ast;
    return 0;

}

AST* CleanupVisitor::visit(IfAST* if_ast){

    dbg() << "delete IfAST\n";

    delete if_ast;
    return 0;

}

AST* CleanupVisitor::visit(ForAST* for_ast){

    dbg() << "delete ForAST\n";

    delete for_ast;
    return 0;

}

AST* CleanupVisitor::visit(BlockAST* block_ast){

    for(auto& ast : block_ast->statements())
        ast->visit(*this);

    dbg() << "delete BlockAST\n";

    delete block_ast;
    return 0;

}

AST* CleanupVisitor::visit(ErrorAST* error_ast){

    dbg() << "delete ErrorAST\n";

    delete error_ast;
    return 0;

}

AST* CleanupVisitor::visit(BreakAST* break_ast){

    dbg() << "delete BreakAST\n";

    delete break_ast;
    return 0;

}

AST* CleanupVisitor::visit(ContinueAST* continue_ast){

    dbg() << "delete ContinueAST\n";

    delete continue_ast;
    return 0;

}

}