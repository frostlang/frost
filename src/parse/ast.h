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




#define DEF_VISITOR_VIRTUAL_VISIT_AST(param) virtual AST* visit(param) = 0;

class ASTVisitor{
public:
    DEF_VISITOR_VIRTUAL_VISIT_AST(ProgramAST* program_ast)
    DEF_VISITOR_VIRTUAL_VISIT_AST(BlockAST* block_ast)
private:
};

#define DEF_VISITOR_OVERRIDE_VISIT_AST(param) AST* visit(param) override;

class CleanupVisitor : public ASTVisitor {
public:
    DEF_VISITOR_OVERRIDE_VISIT_AST(ProgramAST* program_ast)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BlockAST* block_ast)
private:
};
}