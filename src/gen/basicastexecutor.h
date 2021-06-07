#pragma once

#include <debug.h>
#include <type.h>
#include <gen/executor.h>

namespace Frost::Gen{

using namespace Parse;


// represents a basic value
class BasicValue{
public:
    static BasicValue create(){
        BasicValue b;
        b.m_type = Type::create(TypeType::U0);
        return b;
    } 
private:
    Type m_type;
};


class BasicGC{
public:

    // allocate a value
    BasicValue* value(Type type){
        return 0;
    }

    // perform gc
    void gc(){}

private:
    
};

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
    DEF_VISITOR_OVERRIDE_VISIT_AST(BinOpAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(VariableAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(LiteralAST*)
private:
    BasicGC m_gc;
    Parse::AST* m_ast;
};

}