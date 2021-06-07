#pragma once

#include <vector>
#include <parse/token.h>
#include <type.h>

namespace Frost::Parse{

class ASTVisitor;


class ErrorAST;
class ProgramAST;
class IfAST;
class ForAST;
class BlockAST;
class BreakAST;
class ContinueAST;
class DeclAST;
class BinOpAST;
class VariableAST;
class LiteralAST;




class AST {

public:
    virtual void* visit(ASTVisitor& visitor) = 0;
private:

};


#define DEF_VISIT_INHERIT_AST void* visit(ASTVisitor& visitor) override;

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

    AST* if_cond(){
        return m_if_cond;
    }
    AST* if_body(){
        return m_if_body;
    }
    AST* else_body(){
        return m_else_body;
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

class DeclAST : public AST{
public:
    DeclAST(Token& identifier) : m_identifier(identifier){}
    static DeclAST create(Token& identifier, Type type, AST* value){
        DeclAST d(identifier);
        d.m_type = type;
        d.m_value = value;
        return d;
    }
    DEF_VISIT_INHERIT_AST
private:
    const Token& m_identifier;
    Type m_type;
    AST* m_value;
};



class BinOpAST: public AST{
public:
    enum class Op : u8{
        LOR,
        LAND,
        BOR,
        BAND,
        EQ,
        NEQ,
        GT,
        GE,
        LT,
        LE,
        PLUS,
        MINUS,
        MUL,
        DIV
    };

    static BinOpAST create(Op type, AST* lhs, AST* rhs){
        BinOpAST b;
        b.m_type = type;
        b.m_lhs = lhs;
        b.m_rhs = rhs;
        return b;
    }

    DEF_VISIT_INHERIT_AST

    Op& op(){
        return m_type;
    }

    AST* lhs(){
        return m_lhs;
    }
    AST* rhs(){
        return m_rhs;
    }
private:
    AST* m_lhs;
    AST* m_rhs;
    Op m_type;
};

class VariableAST : public AST {
public:

    
    VariableAST(Token& token) : m_token(token){}

    static VariableAST create(Token& token){
        VariableAST l(token);
        return l;
    }
    DEF_VISIT_INHERIT_AST
private:
    const Token& m_token;
};


class LiteralAST : public AST {
public:

    
    LiteralAST(Token& token) : m_token(token){}

    static LiteralAST create(Token& token){
        LiteralAST l(token);
        return l;
    }
    DEF_VISIT_INHERIT_AST

    const Token& token(){
        return m_token;
    }
    Type& type(){
        return m_type;
    }
private:
    Type m_type;
    const Token& m_token;
};



#define DEF_VISITOR_VIRTUAL_VISIT_AST(param) virtual void* visit(param) = 0;

class ASTVisitor{
public:
    DEF_VISITOR_VIRTUAL_VISIT_AST(ErrorAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(ProgramAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(IfAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(ForAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(BlockAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(BreakAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(ContinueAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(DeclAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(BinOpAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(VariableAST*)
    DEF_VISITOR_VIRTUAL_VISIT_AST(LiteralAST*)
private:
};

#define DEF_VISITOR_OVERRIDE_VISIT_AST(param) void* visit(param) override;

class CleanupVisitor : public ASTVisitor {
public:
    DEF_VISITOR_OVERRIDE_VISIT_AST(ErrorAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ProgramAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(IfAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ForAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BlockAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BreakAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ContinueAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(DeclAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BinOpAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(VariableAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(LiteralAST*)
private:
};
}