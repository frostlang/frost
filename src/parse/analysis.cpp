#include <parse/analysis.h>

namespace Frost::Parse{


void Analyser::analyse(){
    visit(m_ast, {});
}

Optional<Type> Analyser::visit(AST* ast, AnalysisContext ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
        }case Parse::AST::Type::DECL:{
            return visit(static_cast<Parse::DeclAST*>(ast), ctx);
        }case Parse::AST::Type::RETURN:{
            return visit(static_cast<Parse::ReturnAST*>(ast), ctx);
        } case Parse::AST::Type::BIN:{
            return visit(static_cast<Parse::BinOpAST*>(ast), ctx);
        } case Parse::AST::Type::VARIABLE:{
            return visit(static_cast<Parse::VariableAST*>(ast), ctx);
        } case Parse::AST::Type::LITERAL:{
            return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
        }
    }
    return Optional<Type>();
}

Optional<Type> Analyser::visit(ProgramAST* program_ast, AnalysisContext ctx){
    return Optional<Type>();   
    //for(auto& statement : program_ast->statements())
    //    visit(statement, ctx);
    //return Optional<Type>();
}


Optional<Type> Analyser::visit(DeclAST* decl_ast, AnalysisContext ctx){
    //if(decl_ast->requires_inference()){
    //    auto type = visit(decl_ast->value(), ctx);
    //    ASSERT(type.has());
    //    decl_ast->lit_type()=type.data();
    //}
    return Optional<Type>();
}


Optional<Type> Analyser::visit(BinOpAST* bin_op_ast, AnalysisContext ctx){
    // we need to be able to cast the types to the higher precedence type
    // todo for now return the type of the lhs
    return visit(bin_op_ast->lhs(), ctx);
}


Optional<Type> Analyser::visit(VariableAST* variable_ast, AnalysisContext){
    return Optional<Type>();
    //return m_sym_table.get(variable_ast->token().value().data());
}
Optional<Type> Analyser::visit(LiteralAST* literal_ast, AnalysisContext){
     return Optional<Type>();
    //return literal_ast->lit_type();
}

}