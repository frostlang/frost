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
class ReturnAST;
class BreakAST;
class ContinueAST;
class DeclAST;
class BinOpAST;
class VariableAST;
class LiteralAST;




class AST : public Debugable{

public:

    enum class Type : u8{
        ERROR,
        PROGRAM,
        IF,
        FOR,
        BLOCK,
        RETURN,
        BREAK,
        CONTINUE,
        DECL,
        BIN,
        VARIABLE,
        LITERAL
    };
    virtual std::string debug() {return "AST";}
    virtual Type type() const = 0;
    virtual void* visit(ASTVisitor& visitor) = 0;
private:

};


#define DEF_VISIT_INHERIT_AST void* visit(ASTVisitor& visitor) override;

class ErrorAST : public AST {
public:
    Type type() const override {
        return Type::ERROR;
    }
    std::string debug() override {
        return "ERROR";
    }
    static ErrorAST create(){
        return {};
    }
    
    DEF_VISIT_INHERIT_AST
private:
};

class ProgramAST : public AST {
public:
    Type type() const override {
        return Type::PROGRAM;
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "PROGRAM";
        for(auto stmt : m_statements)
            ss << "\n\t" << stmt->debug();
        return ss.str();
    }
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
    Type type() const override {
        return Type::IF;
    }
    static IfAST create(){
        return {};
    }
    std::string debug() override {
        return "IF";
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
    Type type() const override {
        return Type::FOR;
    }
    static ForAST create(){
        return {};
    }

    std::string debug() override {
        return "FOR";
    }

    DEF_VISIT_INHERIT_AST
private:
};

class BlockAST : public AST {
public:
    Type type() const override {
        return Type::BLOCK;
    }
    static BlockAST create(std::vector<AST*> statements){
        BlockAST p;
        p.m_statements = statements;
        return p;
    }

    std::string debug() override {
        std::stringstream ss;
        ss << "BLOCK";
        for(auto stmt : m_statements)
            ss << "\n\t" << stmt->debug();
        return ss.str();
    }

    DEF_VISIT_INHERIT_AST
    
    auto statements(){
        return m_statements;
    }
private:
    std::vector<AST*> m_statements;
};

class ReturnAST : public AST{
public:
    Type type() const override {
        return Type::RETURN;
    }
    static ReturnAST create(){
        return {};
    }

    std::string debug() override {
        return "RETURN";
    }

    DEF_VISIT_INHERIT_AST
private:
};

class BreakAST : public AST{
public:
    Type type() const override {
        return Type::BREAK;
    }
    static BreakAST create(){
        return {};
    }

    std::string debug() override {
        return "BREAK";
    }

    DEF_VISIT_INHERIT_AST
private:
};

class ContinueAST : public AST{
public:
    Type type() const override {
        return Type::CONTINUE;
    }
    static ContinueAST create(){
        return {};
    }
    std::string debug() override {
        return "CONTINUE";
    }
    DEF_VISIT_INHERIT_AST
private:
};

class DeclAST : public AST{
public:
    Type type() const override {
        return Type::DECL;
    }
    DeclAST(Token& identifier) : m_identifier(identifier){}
    static DeclAST create(Token& identifier, Frost::Type type, AST* value){
        DeclAST d(identifier);
        d.m_type = type;
        d.m_value = value;
        return d;
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "DECL";
        ss << "\n\t" << s(m_identifier.value());
        ss << "\n\t" << m_type.debug();
        return ss.str();
    }
    DEF_VISIT_INHERIT_AST

    const Token& token(){
        return m_identifier;
    }
    Frost::Type& lit_type(){
        return m_type;
    }

private:
    const Token& m_identifier;
    Frost::Type m_type;
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

    Type type() const override {
        return Type::BIN;
    }
    static BinOpAST create(Op type, AST* lhs, AST* rhs){
        BinOpAST b;
        b.m_type = type;
        b.m_lhs = lhs;
        b.m_rhs = rhs;
        return b;
    }
    std::string debug() override {
        const char* lookup[] = {
            "or","and","|","&","==","!=",">",">=","<","<=","+","-","*","/"
        };
        std::stringstream ss;
        ss << "BIN";
        ss << "\n\t" << m_lhs->debug()
           << "\n\t" << lookup[(u8)m_type]
           << "\n\t" << m_rhs->debug();
        return ss.str();
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

    Type type() const override {
        return Type::VARIABLE;
    }
    
    VariableAST(Token& token) : m_token(token){}
    std::string debug() override {
        std::stringstream ss;
        ss << "VAR";
        ss << "\n\t" << s(m_token.value());
        return ss.str();
    }
    static VariableAST create(Token& token){
        VariableAST l(token);
        l.m_type=Frost::Type(Frost::Type::Storage::U8);
        return l;
    }
    Frost::Type& var_type(){
        return m_type;
    }
    const Token& token(){
        return m_token;
    }
    DEF_VISIT_INHERIT_AST
private:
    Frost::Type m_type;
    const Token& m_token;
};


class LiteralAST : public AST {
public:

    Type type() const override {
        return Type::LITERAL;
    }
    
    LiteralAST(Token& token) : m_token(token){}
    std::string debug() override {
        std::stringstream ss;
        ss << "LIT";
        ss << "\n\t" << s(m_token.value());
        return ss.str();
    }
    static LiteralAST create(Token& token, Frost::Type type){
        LiteralAST l(token);
        l.m_type = type;
        return l;
    }
    DEF_VISIT_INHERIT_AST

    const Token& token(){
        return m_token;
    }
    Frost::Type& lit_type(){
        return m_type;
    }
private:
    Frost::Type m_type;
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
    DEF_VISITOR_VIRTUAL_VISIT_AST(ReturnAST*)
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
    DEF_VISITOR_OVERRIDE_VISIT_AST(ReturnAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BreakAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(ContinueAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(DeclAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(BinOpAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(VariableAST*)
    DEF_VISITOR_OVERRIDE_VISIT_AST(LiteralAST*)
private:
};
}