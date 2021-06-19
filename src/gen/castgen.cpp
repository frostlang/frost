#include <gen/castgen.h>
#include <gen/c.h>
#include <iostream>
#include <fstream>

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
        ctx.emit("#include <stdio.h>\n");
        visit(m_ast, ctx);
        for(auto& block : ctx.blocks())
            block.dump();



        std::ofstream c_src ("/frost/test/generated/c_src.c");
        if (c_src.is_open())
        {
            for(auto& block : ctx.blocks())
            c_src << block.src();
            c_src.close();
            system("gcc /frost/test/generated/c_src.c -o /frost/test/executables/c");
            dbg() << "---RUNNING EXECUTABLE---\n\n\n";
            system("/frost/test/executables/c.exe");
            dbg() << "\n\n\n---DONE RUNNING---";
        }
        else dbg() << "Unable to open file";
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
                return visit(static_cast<Parse::ExprStmtAST*>(ast), ctx);
            }
            case Parse::AST::Type::RETURN:{
                return visit(static_cast<Parse::ReturnAST*>(ast), ctx);
            }
            case Parse::AST::Type::IF:{
                return visit(static_cast<Parse::IfAST*>(ast), ctx);
            }
            case Parse::AST::Type::DECL:{
                return visit(static_cast<Parse::DeclAST*>(ast), ctx);
            }
            case Parse::AST::Type::BIN:{
                return visit(static_cast<Parse::BinOpAST*>(ast), ctx);
            }
            case Parse::AST::Type::CALL:{
                return visit(static_cast<Parse::CallAST*>(ast), ctx);
            }
            case Parse::AST::Type::VARIABLE:{
                return visit(static_cast<Parse::VariableAST*>(ast), ctx);
            }
            case Parse::AST::Type::FN:{
                return visit(static_cast<Parse::FnAST*>(ast), ctx);
            }
            case Parse::AST::Type::STRING:{
                return visit(static_cast<Parse::StringAST*>(ast), ctx);
            }
            case Parse::AST::Type::LITERAL:{
                return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
            }
            case Parse::AST::Type::STRUCT:{
                return visit(static_cast<Parse::StructAST*>(ast), ctx);
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
    
    Optional<std::string> CASTGen::visit(Parse::ReturnAST* ast, BuildContext& ctx){
        ctx.emit("return");
        if(ast->has_value()){
            ctx.emit(" ");
            auto value = visit(ast->value(), ctx);
            ASSERT(value.has());
            ctx.emit(value.data());
        }
        ctx.emit(";\n");
        return Optional<std::string>();
    }

    Optional<std::string> CASTGen::visit(Parse::IfAST* ast, BuildContext& ctx){
        ctx.emit("if(");
        ctx.emit(visit(ast->if_cond(), ctx).data());
        ctx.emit(")");
        ctx.emit(visit(ast->if_body(), ctx).data());
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
        else if(
            ast->lit_type().type()==Frost::Type::Storage::TYPE 
            && ast->lit_type().mut()==Frost::MutableType::CONST
            ){
                // if the decleration is initialised to a function,
                // update the functions mangled name
                if(ast->initialised()){
                    // todo this is a hack
                    Parse::StructAST* strct = static_cast<Parse::StructAST*>(ast->value());
                    return visit(strct, ctx);
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
    
    Optional<std::string> CASTGen::visit(Parse::ExprStmtAST* ast, BuildContext& ctx){
        dbg() << "doing exprstmt!\n";
        auto expr = visit(ast->expr(), ctx);
        ASSERT(expr.has());
        ctx.emit(expr.data());
        ctx.emit(";\n");
        return Optional<std::string>();
    }

    Optional<std::string> CASTGen::visit(Parse::BinOpAST* ast, BuildContext& ctx){
        const char* ops[]={
            "||",
            "&&",
            "|",
            "&",
            "==",
            "!=",
            ">",
            ">=",
            "<",
            "<=",
            "+",
            "-",
            "*",
            "/"
        };
        std::stringstream ss;
        ss << visit(ast->lhs(), ctx).data();
        ss << ops[(u32)ast->op()];
        ss << visit(ast->rhs(), ctx).data();
        return ss.str();
    }

    Optional<std::string> CASTGen::visit(Parse::CallAST* ast, BuildContext& ctx){
        std::stringstream ss;
        ss << visit(ast->callee(), ctx).data() << "(";
        u32 i = 0;
        for(auto& arg : ast->args()){
            auto a = visit(arg, ctx);
            ASSERT(a.has());
            ss << a.data();
            if(i<ast->args().size()-1)
                ss << ",";
            i++;
        }
        ss << ")";
        return ss.str();
    }
    
    Optional<std::string> CASTGen::visit(Parse::VariableAST* ast, BuildContext& ctx){
        return s(ast->token().value());
    }

    Optional<std::string> CASTGen::visit(Parse::StringAST* ast, BuildContext& ctx){
        std::stringstream ss;
        ss << "\"" << s(ast->token().value()) << "\"";
        return ss.str();
    }

    Optional<std::string> CASTGen::visit(Parse::LiteralAST* ast, BuildContext& ctx){
        return s(ast->token().value());
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

    Optional<std::string> CASTGen::visit(Parse::StructAST* ast, BuildContext& ctx){
        ctx.emit("typedef struct {\n");
        for(auto& decl : ast->decls()){
            visit(decl, ctx);
        }
        ctx.emit("} ");
        ctx.emit(ast->mangled_identifier());
        ctx.emit(";\n");
        return Optional<std::string>();
    }
}