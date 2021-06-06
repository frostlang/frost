#include <gen/basicastexecutor.h>

namespace Frost::Gen{

void* BasicASTExecutor::visit(ProgramAST* program_ast){

    for(auto& ast : program_ast->statements())
        ast->visit(*this);

    dbg() << "exec ProgramAST\n";

    return 0;

}

void* BasicASTExecutor::visit(IfAST* if_ast){

    dbg() << "exec IfAST\n";

    return 0;

}

void* BasicASTExecutor::visit(ForAST* for_ast){

    dbg() << "exec ForAST\n";

    return 0;

}

void* BasicASTExecutor::visit(BlockAST* block_ast){

    for(auto& ast : block_ast->statements())
        ast->visit(*this);

    dbg() << "exec BlockAST\n";

    return 0;

}

void* BasicASTExecutor::visit(ErrorAST* error_ast){

    dbg() << "exec ErrorAST\n";

    return 0;

}

void* BasicASTExecutor::visit(BreakAST* break_ast){

    dbg() << "exec BreakAST\n";

    return 0;

}

void* BasicASTExecutor::visit(ContinueAST* continue_ast){

    dbg() << "exec ContinueAST\n";

    return 0;

}

void* BasicASTExecutor::visit(LOrAST* lor_ast){

    lor_ast->lhs()->visit(*this);
    lor_ast->rhs()->visit(*this);
    dbg() << "exec LOrAST\n";

    return 0;

}

void* BasicASTExecutor::visit(LAndAST* land_ast){

    land_ast->lhs()->visit(*this);
    land_ast->rhs()->visit(*this);
    dbg() << "exec LAndAST\n";
    return 0;

}

void* BasicASTExecutor::visit(BOrAST* bor_ast){

    bor_ast->lhs()->visit(*this);
    bor_ast->rhs()->visit(*this);
    dbg() << "exec BOrAST\n";
    return 0;

}

void* BasicASTExecutor::visit(BAndAST* band_ast){

    band_ast->lhs()->visit(*this);
    band_ast->rhs()->visit(*this);
    dbg() << "exec BAndAST\n";
    return 0;

}

void* BasicASTExecutor::visit(VariableAST* variable_ast){

    dbg() << "exec VariableAST\n";

    return 0;

}

void* BasicASTExecutor::visit(LiteralAST* literal_ast){

    dbg() << "exec LiteralAST\n";

    return 0;

}


}