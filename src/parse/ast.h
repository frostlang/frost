#pragma once

#include <vector>

namespace Frost::Parse{

class ASTVisitor;

class AST {

public:
    virtual AST* visit(ASTVisitor& visitor) = 0;
private:

};


#define DEF_VISIT_INHERIT_AST AST* visit(ASTVisitor& visitor) override;

class ErrorAST : public AST {
public:
    static ErrorAST create(){
        return {};
    }
    
    DEF_VISIT_INHERIT_AST
private:
};

class ProgramAST : public AST {
public:
    static ProgramAST create(std::vector<AST*> statements){
        ProgramAST p;
        p.m_statements = statements;
        return p;
    }

    DEF_VISIT_INHERIT_AST

    auto statements(){
        return m_statements;
    }
private:
    std::vector<AST*> m_statements;
};


class IfAST : public AST{
public:
    static IfAST create(){
        return {};
    }

    DEF_VISIT_INHERIT_AST

    void set_if_cond(AST* if_cond){
        m_if_cond=if_cond;
    }
    void set_if_body(AST* if_body){
        m_if_body = if_body;
    }
    void set_else_body(AST* else_body){
        m_else_body=else_body;
    }
private:
    AST* m_if_cond;
    AST* m_if_body;
    AST* m_else_body;
};

class ForAST : public AST{
public:
    static ForAST create(){
        return {};
    }

    DEF_VISIT_INHERIT_AST
private:
};

class BlockAST : public AST {
public:
    static BlockAST create(std::vector<AST*> statements){
        BlockAST p;
        p.m_statements = statements;
        return p;
    }

    DEF_VISIT_INHERIT_AST
    
    auto statements(){
        return m_statements;
    }
private:
    std::vector<AST*> m_statements;
};

class BreakAST : public AST{
public:
    static BreakAST create(){
        return {};
    }

    DEF_VISIT_INHERIT_AST
private:
};

class ContinueAST : public AST{
public:
    static ContinueAST create(){
        return {};
    }

    DEF_VISIT_INHERIT_AST
private:
};



#define DEF_VISITOR_VIRTUAL_VISIT_AST(param) virtual AST* visit(param) = 0;

class ASTVisitor{
public:
    DEF_VISITOR_VIRTUAL_VISIT_AST(ErrorAST* error_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(ProgramAST* program_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(IfAST* if_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(ForAST* for_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(BlockAST* block_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(BreakAST* break_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(ContinueAST* continue_ast)
private:
};

#define DEF_VISITOR_OVERRIDE_VISIT_AST(param) AST* visit(param) override;

class CleanupVisitor : public ASTVisitor {
public:
    DEF_VISITOR_OVERRIDE_VISIT_AST(ErrorAST* error_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ProgramAST* program_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(IfAST* if_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ForAST* for_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BlockAST* block_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BreakAST* break_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ContinueAST* continue_ast)
private:
};
}