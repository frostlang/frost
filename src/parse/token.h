#pragma once

#include <string_view>
#include <vector>
#include <types.h>
#include <debug.h>
#include <asserts.h>

namespace Frost::Parse{

enum class TokenType : u8 {
    UNKNOWN
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

    std::string debug() override {
        return "Token!";
    }

    std::string_view& value(){
        return m_value;
    }

private:

    std::string_view m_value;

};

class TokenStream {

public:

static TokenStream create(){
    TokenStream t;
    t.m_index=0;
    return t;
}

std::vector<Token>& tokens(){
    return m_tokens;
}

void reset(){
    m_index = 0;
}

Token& prev(){
    ASSERT(m_index>0);
    return m_tokens[m_index-1];
}
Token& next(){
    ASSERT(m_index<=m_tokens.size()-1);
    return m_tokens[m_index++];
}

Token& peek(){
    ASSERT(m_index>=0 && m_index<=m_tokens.size()-1);
    return m_tokens[m_index];
}

private:
u32 m_index = 0;
std::vector<Token> m_tokens;


};

}