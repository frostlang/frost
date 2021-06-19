#pragma once
#include <type.h>
#include <types.h>
#include <gen/c.h>
#include <parse/ast.h>
// generate C from frost source

namespace Frost::Gen::C{

class CASTGen{
public:
    static CASTGen create(Parse::AST* ast){
        CASTGen x;
        x.m_ast = ast;
        return x;
    }
    
    std::string type_to_c(Frost::Type type);
    void gen();
    Optional<std::string> visit(Parse::AST* ast, C::BuildContext& ctx);
    Optional<std::string> visit(Parse::ProgramAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::BlockAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::IfAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::DeclAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::ExprStmtAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::BinOpAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::CallAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::VariableAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::StringAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::LiteralAST* ast, BuildContext& ctx);
    Optional<std::string> visit(Parse::FnAST* ast, BuildContext& ctx);
private:
    Parse::AST* m_ast;
};
}