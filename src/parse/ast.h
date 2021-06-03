#pragma once

namespace Frost::Parse{

class ASTVisitor;

class AST {

public:
    virtual AST* visit(ASTVisitor& visitor) = 0;
private:

};


class ProgramAST : public AST {
public:
    AST* visit(ASTVisitor& visitor) override;
private:
};


class ASTVisitor{
public:
    virtual AST* visit(ProgramAST* program_ast) = 0;
private:
};


class CleanupVisitor : public ASTVisitor {
public:
    AST* visit(ProgramAST* program_ast) override;
private:
};
}