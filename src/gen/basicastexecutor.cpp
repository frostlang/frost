/*#include <gen/basicastexecutor.h>

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

void* BasicASTExecutor::visit(DeclAST* decl_ast){

    dbg() << "exec DeclAST\n";

    return 0;

}

void* BasicASTExecutor::visit(BinOpAST* bin_op_ast){

    bin_op_ast->lhs()->visit(*this);
    bin_op_ast->rhs()->visit(*this);
    dbg() << "exec BinOpAST\n";

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


}*/