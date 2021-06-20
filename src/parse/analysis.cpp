#include <parse/analysis.h>

namespace Frost::Parse{


void Analyser::analyse(){
    visit(m_ast, {});
}

Optional<Type> Analyser::visit(AST* ast, AnalysisCtx ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
        }case Parse::AST::Type::BLOCK:{
            return visit(static_cast<Parse::BlockAST*>(ast), ctx);
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
        } case Parse::AST::Type::FN:{
            return visit(static_cast<Parse::FnAST*>(ast), ctx);
        } case Parse::AST::Type::STRUCT:{
            return visit(static_cast<Parse::StructAST*>(ast), ctx);
        }
    }
    return Optional<Type>();
}

Optional<Type> Analyser::visit(ProgramAST* program_ast, AnalysisCtx ctx){
    for(auto& statement : program_ast->statements())
        visit(statement, ctx);
    return Optional<Type>();
}
Optional<Type> Analyser::visit(BlockAST* ast, AnalysisCtx ctx){
    for(auto& statement : ast->statements())
       visit(statement, ctx);
    return Optional<Type>();
}

Optional<Type> Analyser::visit(DeclAST* decl_ast, AnalysisCtx ctx){
    // if we have a const fn then we need to essentially remove this decl...
    if(
        decl_ast->lit_type().type()==Frost::Type::Storage::FN 
        && decl_ast->lit_type().mut()==Frost::MutableType::CONST
        ){
            // if the decleration is initialised to a function,
            // update the functions mangled name
            if(decl_ast->initialised()){
                FnAST* fn = static_cast<FnAST*>(decl_ast->value());
            
                fn->mangled_identifier() = s(decl_ast->token().value());
            }
        }

    else if(
        decl_ast->lit_type().type()==Frost::Type::Storage::TYPE 
        && decl_ast->lit_type().mut()==Frost::MutableType::CONST
        ){
            // if the decleration is initialised to a function,
            // update the structs mangled name
            if(decl_ast->initialised()){
                StructAST* strct = static_cast<StructAST*>(decl_ast->value());
                strct->mangled_identifier() = s(decl_ast->token().value());
            }
        }

    else if(decl_ast->lit_type().type()==Frost::Type::Storage::UNKNOWN){
        // check if we are dealing with a struct
        auto type = m_sym_table.get(decl_ast->lit_type().token());
        if(!type.has()){
            dbg() << "unknown type!\n";


            return Optional<Type>();
        }
        

        decl_ast->lit_type()=type.data();

    }



    //if(decl_ast->requires_inference()){
    //    auto type = visit(decl_ast->value(), ctx);
    //    ASSERT(type.has());
    //    decl_ast->lit_type()=type.data();
    //}
    if(decl_ast->initialised())
        visit(decl_ast->value(), ctx);
    

    return Optional<Type>();
}


Optional<Type> Analyser::visit(BinOpAST* bin_op_ast, AnalysisCtx ctx){
    // we need to be able to cast the types to the higher precedence type
    // todo for now return the type of the lhs
    return visit(bin_op_ast->lhs(), ctx);
}


Optional<Type> Analyser::visit(VariableAST* variable_ast, AnalysisCtx){
    return Optional<Type>();
    //return m_sym_table.get(variable_ast->token().value().data());
}
Optional<Type> Analyser::visit(LiteralAST* literal_ast, AnalysisCtx){
     return Optional<Type>();
    //return literal_ast->lit_type();
}
Optional<Type> Analyser::visit(FnAST* ast, AnalysisCtx ctx){
    visit(ast->body(), ctx);
    return Optional<Type>();
    //return literal_ast->lit_type();
}
Optional<Type> Analyser::visit(StructAST* ast, AnalysisCtx ctx){

    auto type = Frost::Type(Frost::Type::Storage::STRUCT);
    type.set_token(ast->mangled_identifier());
    m_sym_table.put(ast->mangled_identifier(), type);

    return Optional<Type>();
    //return literal_ast->lit_type();
}

}