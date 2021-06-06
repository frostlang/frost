#pragma once

#include <debug.h>
#include <gen/executor.h>

namespace Frost::Gen{

using namespace Parse;
/*
A basic Tree-Walking interpreter
*/
class BasicASTExecutor : public ASTExecutor{
public:


    static BasicASTExecutor create(Parse::AST* ast){
        BasicASTExecutor b;
        b.m_ast = ast;
        return b;
    }

    void exec() override {
        dbg() << "BasicASTExecutor exec!\n";

        m_ast->visit(*this);
    }

    DEF_VISITOR_OVERRIDE_VISIT_AST(ErrorAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ProgramAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(IfAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ForAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BlockAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BreakAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ContinueAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(LOrAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(LAndAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BOrAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BAndAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(VariableAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(LiteralAST*)
private:
    Parse::AST* m_ast;
};

}