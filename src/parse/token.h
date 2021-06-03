#pragma once

#include <string_view>
#include <vector>
#include <types.h>
#include <debug.h>
#include <asserts.h>

namespace Frost::Parse{

enum class TokenType : u8 {
    UNKNOWN,
    UNDERSCORE,
    IDENTIFIER,

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

    PLUS,
    MINUS,
    STAR,
    DIV,
    MODULO,


};



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

    std::string debug() override {
        return "Token!";
    }

    const std::string_view& value() const {
        return m_value;
    }

    const TokenType& type(){
        return m_type;
    }

private:

    std::string_view m_value;
    TokenType m_type;

};

class TokenStream {

public:

static TokenStream create(){
    TokenStream t;
    t.m_index=0;
    return t;
}

void push(Token t){
    m_tokens.push_back(t);
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
    ASSERT(m_index>0);
    return m_tokens[m_index-1];
}

Token& next() {
    ASSERT(m_index<=m_tokens.size()-1);
    return m_tokens[m_index++];
}

Token& peek() {
    ASSERT(m_index>=0 && m_index<=m_tokens.size()-1);
    return m_tokens[m_index];
}

private:
u32 m_index = 0;
std::vector<Token> m_tokens;


};

}