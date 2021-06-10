#pragma once

#include <string_view>
#include <sstream>
#include <vector>
#include <types.h>
#include <debug.h>
#include <asserts.h>

namespace Frost::Parse{

enum class TokenType : u8 {
    UNKNOWN,
    UNDERSCORE,
    IDENTIFIER,

    COLON,
    SEMICOLON,
    COMMA,

    LCURLY,
    RCURLY,
    LPAREN,
    RPAREN,
    LBRACKET,
    RBRACKET,

    QUOTE,
    APOSTROPHE,
    NUMBER,
    TRUE,
    FALSE,

    ANY,
    U0,
    U1,
    U8,
    S8,
    U16,
    S16,
    U32,
    S32,
    F32,
    U64,
    S64,
    F64,
    FN,
    TYPE,
    MODULE,

    ASSIGN, // =

    PLUS,
    MINUS,
    STAR,
    DIV,
    MODULO,
    REMAINDER,

    INCREMENT,
    DECREMENT,

    LAND, // logial and
    LOR,
    LNOT,
    BOR,
    BAND,
    BNOT,

    EQUALS,
    NEQUALS,

    ARROW,
    AMPERSAND,

    BREAK,
    CONTINUE,
    RETURN,
    IF,
    ELSE,
    FOR,
    MATCH,

    STRUCT,
    INTERFACE,

    PUB,
    PRIV,
    MUT,
    CONST

};

extern const char* token_debug[];


class Position : public Frost::Debugable{

public:
    std::string debug() override{
        return "Position!";
    }
private:
    u32 m_index_start=0;
    u32 m_index_end=0;
    u32 m_line_start=0;
    u32 m_line_end=0;
};


class Token : public Frost::Debugable{


public:

    static Token create(){
        return {};
    }

    static Token create(TokenType type){
        Token t;
        t.m_type = type;
        return t;
    }

    static Token create(TokenType type, std::string_view value){
        Token t;
        t.m_type = type;
        t.m_value = value;
        return t;
    }

    std::string debug() override {
        std::stringstream ss;
        ss << "Token type="<<token_debug[(u8)m_type];
        if(m_type==TokenType::IDENTIFIER || m_type==TokenType::NUMBER)
            ss << " value=" << m_value;
        ss <<"\n";
        return ss.str();
    }

    const std::string_view& value() const {
        return m_value;
    }

    const TokenType& type() const{
        return m_type;
    }

private:

    std::string_view m_value;
    TokenType m_type;

};

class TokenStream : public Debugable{

public:

static TokenStream create(){
    TokenStream t;
    t.m_index=0;
    return t;
}

std::string debug(){
    std::stringstream ss;
    for(auto& t : m_tokens)
        ss << t.debug() << "\n";
    return ss.str();
}

void push(Token t){
    m_tokens.push_back(t);
}

u32 size(){
    return m_tokens.size();
}
u1 end() const {
    return m_index>=m_tokens.size();
}

std::vector<Token>& tokens() {
    return m_tokens;
}

void reset() {
    m_index = 0;
}

Token& prev() {
    return m_tokens[m_index-1];
}

u1 expect(TokenType type){
    return peek().type()==type;
}

Optional<Token> consume(TokenType type){
    if(peek().type()==type){
        return Optional(next());
    }
    return Optional<Token>();
}

Token& next() {
    return m_tokens[m_index++];
}

Token& peek(u32 ahead=0) {
    return m_tokens[m_index+ahead];
}

private:
u32 m_index = 0;
std::vector<Token> m_tokens;


};

}