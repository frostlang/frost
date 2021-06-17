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
        BuildContext ctx = {};
        visit(m_ast, ctx);

        for(auto& block : ctx.blocks())
            block.dump();
    }

    Optional<std::string> CASTGen::visit(Parse::AST* ast, C::BuildContext& ctx){
        switch(ast->type()){
            case Parse::AST::Type::PROGRAM:{
                return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
            }
            case Parse::AST::Type::BLOCK:{
                return visit(static_cast<Parse::BlockAST*>(ast), ctx);
            }
            case Parse::AST::Type::EXPR_STMT: {
                return visit(static_cast<Parse::ExprStmtAST*>(ast)->expr(), ctx);
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
        return Optional<std::string>();
    }

    Optional<std::string> CASTGen::visit(Parse::ProgramAST* ast, BuildContext& ctx){
        for(auto& stmt : ast->statements())
            visit(stmt, ctx);
        return Optional<std::string>();
    }

    Optional<std::string> CASTGen::visit(Parse::BlockAST* ast, BuildContext& ctx){
        for(auto& stmt : ast->statements())
            visit(stmt, ctx);
        return Optional<std::string>();
    }
    
    
    Optional<std::string> CASTGen::visit(Parse::IfAST* ast, BuildContext& ctx){
        ctx.emit("if(");
        visit(ast->if_cond(), ctx);
        ctx.emit(")");
        visit(ast->if_body(), ctx);
        ctx.emit("\n");
        return Optional<std::string>();
    }

    Optional<std::string> CASTGen::visit(Parse::DeclAST* ast, BuildContext& ctx){

        // if we have a const fn then we essentially ignore the decl
        // and just generate the function!
        if(
            ast->lit_type().type()==Frost::Type::Storage::FN 
            && ast->lit_type().mut()==Frost::MutableType::CONST
            ){
                // if the decleration is initialised to a function,
                // update the functions mangled name
                if(ast->initialised()){
                    Parse::FnAST* fn = static_cast<Parse::FnAST*>(ast->value());
                    return visit(fn, ctx);
                }
            }

        // if we are not a const fn then just generate the decl :)
        ctx.emit(type_to_c(ast->lit_type()));
        ctx.emit(" ");
        ctx.emit(s(ast->token().value()));
        if(ast->initialised()){
            ctx.emit(" = ");
            ctx.emit(visit(ast->value(), ctx).data());
        }
        ctx.emit(";\n");
        return Optional<std::string>();
    }

    Optional<std::string> CASTGen::visit(Parse::LiteralAST* ast, BuildContext& ctx){
        ctx.emit(s(ast->token().value()));
        return Optional<std::string>();
    }

    
    Optional<std::string> CASTGen::visit(Parse::FnAST* ast, BuildContext& ctx){

        auto* active_block = ctx.active();
        ctx.set_block(ctx.global_block());

        ctx.emit("void ");
        ctx.emit(ast->mangled_identifier());
        ctx.emit("()");
        ctx.emit("{\n");
        visit(ast->body(), ctx);
        ctx.emit("}\n");
        
        ctx.set_block(active_block);

        // return a pointer to this function
        std::stringstream ss;
        ss << "&" << ast->mangled_identifier();

        return ss.str();
    }
}