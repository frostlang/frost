#include <gen/castgen.h>
#include <gen/c.h>

namespace Frost::Gen::C{


    std::string CASTGen::type_to_c(Frost::Type type){
        switch(type.type()){
            case Type::Storage::FN:
                return "void(*)()";
            case Type::Storage::U8:
            case Type::Storage::S8:
            case Type::Storage::U16:
            case Type::Storage::S16:
            case Type::Storage::U32:
            case Type::Storage::S32:
                return "int";
        }
        return "unknown";
    }


    void CASTGen::gen(){
        visit(m_ast, {});
    }

    void CASTGen::emit(std::string_view code){
        dbg() << s(code.data());
    }

    Optional<C::Expression> CASTGen::visit(Parse::AST* ast, C::BuildContext ctx){
        switch(ast->type()){
            case Parse::AST::Type::PROGRAM:{
                return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
            }
            case Parse::AST::Type::IF:{
                return visit(static_cast<Parse::IfAST*>(ast), ctx);
            }
            case Parse::AST::Type::DECL:{
                return visit(static_cast<Parse::DeclAST*>(ast), ctx);
            }
            case Parse::AST::Type::FN:{
                return visit(static_cast<Parse::FnAST*>(ast), ctx);
            }
            case Parse::AST::Type::LITERAL:{
                return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
            }
        }
        return Optional<Expression>();
    }

    Optional<Expression> CASTGen::visit(Parse::ProgramAST* ast, BuildContext ctx){
        for(auto& stmt : ast->statements())
            visit(stmt, ctx);
        return Optional<Expression>();
    }
    
    Optional<Expression> CASTGen::visit(Parse::IfAST* ast, BuildContext ctx){
        emit("if(");
        visit(ast->if_cond(), ctx);
        emit(")");
        visit(ast->if_body(), ctx);
        emit("\n");
        return Optional<Expression>();
    }

    Optional<Expression> CASTGen::visit(Parse::DeclAST* ast, BuildContext ctx){
        emit(type_to_c(ast->lit_type()));
        emit(" ");
        emit(s(ast->token().value()));
        if(ast->initialised()){
            emit(" = ");
            visit(ast->value(), ctx);
        }
        emit(";\n");
        return Optional<Expression>();
    }

    Optional<Expression> CASTGen::visit(Parse::LiteralAST* ast, BuildContext ctx){
        emit(s(ast->token().value()));
        return Optional<Expression>();
    }

    
    Optional<Expression> CASTGen::visit(Parse::FnAST* ast, BuildContext ctx){
        emit("void ");
        emit(ast->mangled_identifier());
        emit("()");
        emit("{\n");
        emit("}\n");
        return Optional<Expression>();
    }
}