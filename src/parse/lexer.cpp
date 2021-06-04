#include <parse/lexer.h>


namespace Frost::Parse{

u1 is_num(char c){
    return c>='0' && c <= '9';
}

u1 is_alpha(char c){
    return c>='a' && c <= 'z';
}

TokenStream& Lexer::lex(){

    dbg() << "lexing...\n";

    while(peek()!='\0'){

        char c = next();
        
        dbg() << "next char = " << c << "\n";

        switch(c){
            case '{': m_tokens.push(Token::create(TokenType::LCURLY)); break;
            case '}': m_tokens.push(Token::create(TokenType::RCURLY)); break;
            case '(': m_tokens.push(Token::create(TokenType::LPAREN)); break;
            case ')': m_tokens.push(Token::create(TokenType::RPAREN)); break;
            case ']': m_tokens.push(Token::create(TokenType::LBRACKET)); break;
            case '[': m_tokens.push(Token::create(TokenType::RBRACKET)); break;
            case '"': m_tokens.push(Token::create(TokenType::QUOTE)); break;
            case '\'': m_tokens.push(Token::create(TokenType::APOSTROPHE)); break;
            case '^': m_tokens.push(Token::create(TokenType::ARROW)); break;
            case '*': m_tokens.push(Token::create(TokenType::STAR)); break;
            case '+': {
                switch(peek()){
                    case '+': m_tokens.push(Token::create(TokenType::INCREMENT)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::PLUS)); break;
                }
            }
            case '-': {
                switch(peek()){
                    case '-': m_tokens.push(Token::create(TokenType::DECREMENT)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::MINUS)); break;
                }
            }
            case '/': {
                switch(peek()){
                    case '/': m_tokens.push(Token::create(TokenType::REMAINDER)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::DIV)); break;
                }
            }
            case '&': {
                switch(peek()){
                    case '&': m_tokens.push(Token::create(TokenType::LAND)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::AMPERSAND)); break;
                }
            }
            default: {
                if(is_num(c)){
                    number();
                    break;
                }else if(is_alpha(c)){
                    alpha();
                    break;
                }
            }
        }

    }
    return m_tokens;
}

void Lexer::number(){
    char c = next();
    while(is_num(c)){
        next();
    }
    m_tokens.push(Token::create(TokenType::UNKNOWN));
}

void Lexer::alpha(){
    char c = next();
    while(is_alpha(c)){
        next();
    }
    m_tokens.push(Token::create(TokenType::UNKNOWN));
}

char Lexer::current(){
    return m_unit->source()[m_index];
}
char Lexer::peek(u32 ahead){
    return m_unit->source()[m_index+ahead];
}
char Lexer::next(){
    return m_unit->source()[m_index++];
}
}