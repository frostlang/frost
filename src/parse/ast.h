#pragma once

#include <vector>
#include <parse/token.h>
#include <type.h>

namespace Frost::Parse{

class ASTVisitor;


class ErrorAST;
class ProgramAST;
class ASMAST;
class IfAST;
class ForAST;
class BlockAST;
class ReturnAST;
class BreakAST;
class ContinueAST;
class ExprStmtAST;
class AssignAST;
class DeclAST;
class BinOpAST;
class Call;
class VariableAST;
class LiteralAST;
class FnAST;

class AST : public Debugable{

public:

    enum class Type : u8{
        ERROR,
        PROGRAM,
        ASM,
        EXPR_STMT,
        IF,
        FOR,
        BLOCK,
        RETURN,
        BREAK,
        CONTINUE,
        DECL,
        ASSIGN,
        BIN,
        CALL,
        GET,
        VARIABLE,
        STRING,
        LITERAL,
        FN,
        LIST,
        INITIALISER_LIST,
        STRUCT,
        INTERFACE
    };
    virtual std::string debug() {return "AST";}
    virtual Type type() const = 0;
private:

};

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


    auto statements(){
        return m_statements;
    }
private:
    std::vector<AST*> m_statements;
};

class ASMAST : public AST{
public:
    Type type() const override {
        return Type::ASM;
    }
private:
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

    ReturnAST(u1 has_value, AST* value) : m_has_value(has_value), m_value(value){}

    std::string debug() override {
        return "RETURN";
    }

    u1& has_value(){
        return m_has_value;
    }

    AST* value(){
        return m_value;
    }
private:
    u1 m_has_value;
    AST* m_value;
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
private:
};

class ExprStmtAST : public AST{
public:
    Type type() const override {
        return Type::EXPR_STMT;
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "EXPR_STMT";
        ss << "\n\t" << m_expr->debug();
        return ss.str();
    }
    ExprStmtAST(AST* expr) : m_expr(expr){}
    AST* expr(){
        return m_expr;
    }
private:
    AST* m_expr;
};

class AssignAST : public AST{
public:

    Type type() const override {
        return Type::ASSIGN;
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "ASSIGN";
        ss << "\n\t" << m_lhs->debug() << "\n\t" << m_rhs->debug();
        return ss.str();
    }
    AssignAST(AST* lhs, AST* rhs) : m_lhs(lhs), m_rhs(rhs){}
    AST* lhs(){
        return m_lhs;
    }
    AST* rhs(){
        return m_rhs;
    }
private:
    AST* m_lhs;
    AST* m_rhs;
};

class DeclAST : public AST{
public:
    Type type() const override {
        return Type::DECL;
    }
    DeclAST(Token& identifier) : m_identifier(identifier){}
    static DeclAST create(Token& identifier, Frost::Type type, u1 initialised, AST* value){
        DeclAST d(identifier);
        d.m_type = type;
        d.m_initialised = initialised;
        d.m_value = value;
        return d;
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "DECL";
        ss << "\n\t" << s(m_identifier.value());
        ss << "\n\t" << m_type.debug();
        if(m_value){
            ss << "\n\t" << m_value->debug();
        }
        return ss.str();
    }

    const Token& token(){
        return m_identifier;
    }
    Frost::Type& lit_type(){
        return m_type;
    }
    u1 initialised(){
        return m_initialised;
    }
    AST* value(){
        return m_value;
    }
    u1& requires_inference(){
        return m_requires_inference;
    }
private:
    const Token& m_identifier;
    Frost::Type m_type;
    u1 m_initialised = false;
    AST* m_value;
    u1 m_requires_inference = {false};
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

class CallAST : public AST {
public:
    
    CallAST(){}
    CallAST(AST* callee,std::vector<AST*> args) : m_callee(callee), m_args(args){}

    std::string debug() override {
        std::stringstream ss;
        ss << "CALL";
        ss << "\n\t" << m_callee->debug();
        for(auto& arg : m_args)
           ss << "\n\t" << arg->debug();
        return ss.str();
    }
    Type type() const override {
        return Type::CALL;
    }
    AST* callee(){
        return m_callee;
    }
    std::vector<AST*>& args(){
        return m_args;
    }
private:
    AST* m_callee;
    std::vector<AST*> m_args;
};

class GetAST : public AST {
public: 
    Type type() const override {
        return Type::GET;
    }
    GetAST(AST* lhs, AST* rhs) : m_lhs(lhs), m_rhs(rhs){}
    std::string debug() override {
        std::stringstream ss;
        ss << "GET";
        ss << "\n\t" << m_lhs->debug();
        ss << "\n\t" << m_rhs->debug();
        return ss.str();
    }
    AST*& lhs(){
        return m_lhs;
    }
    AST*& rhs(){
        return m_rhs;
    }
private:
    AST* m_lhs;
    AST* m_rhs;
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
private:
    Frost::Type m_type;
    const Token& m_token;
};


class StringAST : public AST {
public:
    Type type() const override {
        return Type::STRING;
    }
    
    StringAST(Token& token) : m_token(token){}
    std::string debug() override {
        std::stringstream ss;
        ss << "STR";
        //ss << "\n\t" << s(m_token.value());
        return ss.str();
    }
    static StringAST create(Token& token){
        StringAST s(token);
        return s;
    }
    const Token& token(){
        return m_token;
    }
private:
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

class FnAST : public AST {
public:

    enum class FnType{
        CONST_FN,   // e.g. main : const {}
        LAMBDA,     // e.g. do_something((){})
        METHOD,     // e.g. vec.debug : (this){}
    };

    Type type() const override {
        return Type::FN;
    }
    FnAST(){}
    
    FnAST(std::vector<AST*> params, AST* ret, AST* body) : m_params(params), m_ret(ret), m_body(body), m_fn_type(FnType::CONST_FN){}

    std::string debug() override {
        std::stringstream ss;
        ss << "FN";
        for(auto& param : m_params)
            ss << "\n\t" << param->debug();
        ss << "\n\t" << m_body->debug();
        return ss.str();
    }
    AST* ret(){
        return m_ret;
    }
    AST* body(){
        return m_body;
    }

    FnType& fn_type(){
        return m_fn_type;
    }

    std::vector<AST*>& params(){
        return m_params;
    }

    std::string& mangled_identifier(){
        return m_mangled_identifier;
    }
private:
    // the fully mangled identifier ready to be emitted
    std::string m_mangled_identifier;
    AST* m_body;
    std::vector<AST*> m_params;
    AST* m_ret;
    FnType m_fn_type;
};

class StructAST : public AST {
public:

    Type type() const override {
        return Type::STRUCT;
    }
    StructAST(){}
    
    StructAST(std::vector<AST*> decls) : m_decls(decls), m_mangled_identifier("struct_mangled_identifier"){}

    std::string debug() override {
        std::stringstream ss;
        ss << "STRUCT";
        ss << "\n\t" << m_mangled_identifier;
        for(auto& decl : m_decls)
            ss << "\n\t" << decl->debug();
        return ss.str();
    }
    std::vector<AST*>& decls(){
        return m_decls;
    }

    std::string& mangled_identifier(){
        return m_mangled_identifier;
    }
private:
    // the fully mangled identifier ready to be emitted
    std::string m_mangled_identifier;
    std::vector<AST*> m_decls;
};

}