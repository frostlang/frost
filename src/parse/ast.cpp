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
DEF_AST_VISIT(LOrAST)
DEF_AST_VISIT(LAndAST)
DEF_AST_VISIT(VariableAST)
DEF_AST_VISIT(LiteralAST)



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

AST* CleanupVisitor::visit(LOrAST* lor_ast){

    lor_ast->lhs()->visit(*this);
    lor_ast->rhs()->visit(*this);
    dbg() << "delete LOrAST\n";

    delete lor_ast;
    return 0;

}

AST* CleanupVisitor::visit(LAndAST* land_ast){

    land_ast->lhs()->visit(*this);
    land_ast->rhs()->visit(*this);
    dbg() << "delete LAndAST\n";
    delete land_ast;
    return 0;

}

AST* CleanupVisitor::visit(VariableAST* variable_ast){

    dbg() << "delete VariableAST\n";

    delete variable_ast;
    return 0;

}

AST* CleanupVisitor::visit(LiteralAST* literal_ast){

    dbg() << "delete LiteralAST\n";

    delete literal_ast;
    return 0;

}
}